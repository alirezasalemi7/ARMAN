#include "./SemanticSimilarity.hpp"
#include <fstream>
#include <sstream>
#include <string>

SemanticVectors* loadVectors(std::string vectorsAddress,std::string wordsAddress,std::string hash2IndexAddress,std::string ngramsVectorAddress) {
    SemanticVectors* vectors = new SemanticVectors;
    std::wifstream wordsFile;
    std::locale utf8( std::locale(), new std::codecvt_utf8_utf16<wchar_t> );
    wordsFile.imbue(utf8);
    wordsFile.open(wordsAddress);
    int vec_size,words_count,i=1;
    std::wstring str;
    std::getline(wordsFile,str);
    std::wstringstream stream(str);
    stream >> words_count >> vec_size;
    vectors->weightVectors = new float[vec_size*(words_count+1)];
    std::ifstream vector_file(vectorsAddress,std::ios::binary);
    float temp;
    for (int i = 0; i < vec_size*(words_count+1); i++){
        if (i < vec_size){
            vectors->weightVectors[i] = 0.0;
        }
        else{
            vector_file.read(reinterpret_cast<char*>(&temp), sizeof(float));
            vectors->weightVectors[i] = temp;
        }
    }
    std::ifstream hash2IndexFile(hash2IndexAddress);
    std::string temp_str;
    uint32_t hash;
    int index,count = 0;
    while (std::getline(hash2IndexFile,temp_str)){
        count += 1;
    }
    vectors->ngramVectors = new float[count * vec_size];
    std::ifstream ngram_vector_file(vectorsAddress,std::ios::binary);
    for (int i = 0; i < vec_size*(count); i++){
        ngram_vector_file.read(reinterpret_cast<char*>(&temp), sizeof(float));
        vectors->ngramVectors[i] = temp;
    }
    std::cout << "vectors loaded.\n";
    return vectors;
}

SemanticSimilarity::SemanticSimilarity(SemanticVectors* vectors,std::string wordsAddress,std::string stopwordsAddress,std::string hash2IndexAddress,int max_words_per_sentence) : random_gen(0,1) {
    std::wifstream wordsFile;
    std::locale utf8( std::locale(), new std::codecvt_utf8_utf16<wchar_t> );
    wordsFile.imbue(utf8);
    wordsFile.open(wordsAddress);
    int vec_size,words_count,i=1;
    std::wstring str;
    std::getline(wordsFile,str);
    std::wstringstream stream(str);
    stream >> words_count >> vec_size;
    this->vectorSize = vec_size;
    this->ngram_size = 5;
    this->maximumWordCount = max_words_per_sentence;
    this->total_tokens = 0;
    this->oovVector = new float[vec_size];
    while(std::getline(wordsFile,str)){
        this->stringToVectorIndexMap[str] = i++;
    }
    this->weightVectors = vectors->weightVectors;
    std::wifstream stopWordsFile;
    stopWordsFile.imbue(utf8);
    stopWordsFile.open(stopwordsAddress);
    std::wstring line;
    while (std::getline(stopWordsFile,line)){
        this->stopwords.insert(line);
    }
    std::ifstream hash2IndexFile(hash2IndexAddress);
    std::string temp_str;
    uint32_t hash;
    int index,count = 0;
    while (std::getline(hash2IndexFile,temp_str)){
        std::stringstream stream(temp_str);
        stream >> hash >> index;
        this->ngarmsHash2Index[hash] = index;
        count += 1;
    }
    this->ngramVectors = vectors->ngramVectors;
}

void SemanticSimilarity::similarityMultiply(float* matrix,float* out,int input_size,int vector_size){
    float temp_sum;
    for (int i = 0; i < input_size; i++){
        for (int j = 0; j < input_size; j++){
            temp_sum = 0.0;
            if (i < this->total_tokens && j < this->total_tokens){
                #pragma omp simd reduction(+:temp_sum)
                for (int k = 0; k < vector_size; k++){
                    temp_sum += matrix[i*vector_size + k] * matrix[j*vector_size + k];
                }
            }
            out[i*input_size + j] = temp_sum;
        }
    }
}

std::set<int> SemanticSimilarity::getMostSimilarSentencesToDocUsingSemantics(std::vector<std::wstring>& sentences,int numberOfMaskedSentences,ScoreType type,bool ignoreNext){
    this->sentencesStartEndIndex.clear();
    this->sentencesStartEndIndex.shrink_to_fit();
    this->total_tokens = 0;
    // std::cout << "here\n";
    float* matrix = this->extractDocumentMatrix(sentences);
    float* result = new float[this->maximumWordCount*this->maximumWordCount];
    // std::cout << "here\n";
    this->similarityMultiply(matrix,result,this->maximumWordCount,this->vectorSize);
    std::vector<float> scores;
    scores.reserve(this->sentencesStartEndIndex.size());
    for (int i = 0; i < this->sentencesStartEndIndex.size(); i++){
        if (type == SEM_Precision){
            scores.push_back(this->getPrecisionSimScore(result,this->sentencesStartEndIndex[i]));
        }
        else if (type == SEM_Recall){
            scores.push_back(this->getRecallSimScore(result,this->sentencesStartEndIndex[i]));
        }
        else{
            scores.push_back(this->getF1SimScore(result,this->sentencesStartEndIndex[i]));
        }
    }
    delete[] matrix;
    delete[] result;
    std::vector<int> indices(sentences.size(),0);
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&scores](std::size_t left, std::size_t right){return scores[left] > scores[right];});
    std::set<int> returnIndices;
    if (ignoreNext){
        int added_count = 0;
        bool first = true;
        float random_prob,outlier_sum = 0,outlier_mean = 0.0,outlier_thresh = 0.0, outlier_square = 0.0;
        for (int i = 0;i < sentences.size();i++){
            if (i < numberOfMaskedSentences){
                outlier_sum += scores[indices[i]];
                outlier_square = scores[indices[i]]*scores[indices[i]];
            }
            else if (first){
                first = false;
                outlier_mean = outlier_sum/numberOfMaskedSentences;
                outlier_thresh = outlier_mean - 3 * std::sqrt(outlier_square/numberOfMaskedSentences - outlier_mean*outlier_mean);
            }
            random_prob = this->random_gen(this->gen);
            if (((returnIndices.find(indices[i]+1) == returnIndices.end()) && (returnIndices.find(indices[i]-1) == returnIndices.end())) || (random_prob > 0.999 && scores[indices[i]] > outlier_thresh)){
                returnIndices.insert(indices[i]);
                added_count += 1;
            }
            if (added_count == numberOfMaskedSentences){
                break;
            }
        }
    }
    else {
        for (int i = 0;i < numberOfMaskedSentences;i++){
            returnIndices.insert(indices[i]);
        }
    }
    return returnIndices;
}

float SemanticSimilarity::getF1SimScore(float* result,std::pair<int,int> sentence){
    float precision = this->getPrecisionSimScore(result,sentence);
    float recall = this->getRecallSimScore(result,sentence);
    return 2*recall*precision/(recall+precision);
}

float SemanticSimilarity::getRecallSimScore(float* result,std::pair<int,int> sentence){
    float sum_total = 0.0;
    for (int i = sentence.first; i < sentence.second; i++){
        float max_temp = -100000;
        for (int j = 0; j < this->total_tokens; j++){
            if (j < sentence.first || j >= sentence.second){
                if (result[i*this->maximumWordCount+j] > max_temp){
                    max_temp = result[i*this->maximumWordCount+j];
                }
            }
        }
        sum_total += max_temp;
    }
    return sum_total/(sentence.second-sentence.first);
}

float SemanticSimilarity::getPrecisionSimScore(float* result,std::pair<int,int> sentence){
    float sum_total = 0.0;
    for (int i = 0; i < this->total_tokens; i++){
        float max_temp = -100000;
        for (int j = sentence.first; j < sentence.second; j++){
            if (i < sentence.first || i >= sentence.second){
                if (result[i*this->maximumWordCount+j] > max_temp){
                    max_temp = result[i*this->maximumWordCount+j];
                }
            }
            else{
                max_temp = 0;
            }
        }
        sum_total += max_temp;
    }
    return sum_total/(this->total_tokens-(sentence.second-sentence.first));
}

float* SemanticSimilarity::extractDocumentMatrix(std::vector<std::wstring>& sentences){
    int start = 0,token_index;
    std::wstring str;
    std::wstring temp;
    float* matrix = new float[this->maximumWordCount*this->vectorSize];
    float* temp_address;
    memset(matrix,0,this->maximumWordCount*this->vectorSize*sizeof(float));
    for (int i = 0; i < sentences.size(); i++){
        str = sentences[i];
        std::pair<int,int> boundaryPair;
        boundaryPair.first = start;
        std::wregex e(L"[\\‌.،:;-=/,<>*&^%$#@+~\"؛\\-`_\\)(}{?!!؟\\s۱۲۳۴۵۶۷۸۹۰1234567890]+");
        std::wsregex_token_iterator it(str.begin(), str.end(), e, -1);
        std::wsregex_token_iterator end;
        while (it != end){
            if (start > this->maximumWordCount - 1){
                break;
            }
            if (this->stopwords.find(*it) != this->stopwords.end()){
                it++;
                continue;
            }
            token_index = this->stringToVectorIndexMap[*it];
            if (token_index!=0){
                temp_address = this->weightVectors+token_index*this->vectorSize;
            }
            else{
                temp_address = this->makeVector(*it);
                if (temp_address == nullptr){
                    it++;
                    continue;
                }
            }
            memcpy(matrix+start*this->vectorSize,temp_address,sizeof(float)*this->vectorSize);
            this->total_tokens += 1;
            start++;
            it++;
        }
        boundaryPair.second = start;
        this->sentencesStartEndIndex.push_back(boundaryPair);
    }
    return matrix;
}
        

float* SemanticSimilarity::makeVector(std::wstring oov){
    memset(this->oovVector,0,sizeof(float)*this->vectorSize);
    oov = L"<" + oov + L">";
    if (oov.size() <= this->ngram_size){
        return nullptr;
    }
    int count = 0,token_index;
    uint32_t hash_value;
    float* temp_sub;
    for (int i = 0; i < oov.size()-this->ngram_size + 1; i++){
        std::wstring temp = oov.substr(i,this->ngram_size);
        hash_value = this->hash(temp) % 2000000;
        token_index = this->ngarmsHash2Index[hash_value];
        if (this->ngarmsHash2Index.find(hash_value) != this->ngarmsHash2Index.end()){
            temp_sub = this->ngramVectors+token_index*this->vectorSize;
            for (int j = 0; j < this->vectorSize; j++){
                this->oovVector[j] += temp_sub[j];
            }
            count += 1;  
        }
    }
    if (count != 0){
        float mag = 0.0;
        for (int j = 0; j < this->vectorSize; j++){
            this->oovVector[j] /= count;
            mag += this->oovVector[j]*this->oovVector[j];
        }
        mag = sqrt(mag);
        for (int j = 0; j < this->vectorSize; j++){
            this->oovVector[j] /= mag;
        }
    }
    else{
        return nullptr;
    }
    return this->oovVector;
}

uint32_t SemanticSimilarity::hash(std::wstring str){
    // std::string temp = this->converter.to_bytes(str);
    uint32_t h = 2166136261;
    for (size_t i = 0; i < str.size(); i++) {
        h = h ^ uint32_t(str[i]);
        h = h * 16777619;
    }
    return h;
}