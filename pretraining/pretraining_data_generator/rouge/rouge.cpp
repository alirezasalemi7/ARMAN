#include"rouge.hpp"
#include <algorithm>
#include <boost/random/uniform_real.hpp>
#include <random>

Rouge::Rouge(std::string stopwords_address) : random_gen(0,1) {
    std::wifstream file(stopwords_address);
    this->total_with_respect_to_selected_target = 0;
    this->total_unique = 0;
    std::wstring line;
    while (std::getline(file,line)){
        this->stopwords.insert(line);
    }
}

std::set<std::wstring>* Rouge::getunigrams(std::wstring str){
    std::set<std::wstring>* unigrams = new std::set<std::wstring>();
    std::wregex e(L"[\\.،:)(}{?!!؟\\s]+");
    std::wsregex_token_iterator i(str.begin(), str.end(), e, -1);
    std::wsregex_token_iterator end;
    std::wstring currentToken;
    while (i != end){
        currentToken = *i++;
        if (this->stopwords.find(currentToken) == this->stopwords.end()){
            if (unigrams->find(currentToken) == unigrams->end()){
                if (this->countMapSimpleRouge.find(currentToken) == this->countMapSimpleRouge.end()){
                    this->total_unique += 1;
                    this->countMapSimpleRouge[currentToken] = 1;
                }
                else{
                    this->countMapSimpleRouge[currentToken] = this->countMapSimpleRouge[currentToken] + 1;
                }
                
            }
            unigrams->insert(currentToken);
        }
    }
    return unigrams;
}

std::vector<std::wstring>* Rouge::getunigramsWithDuplicate(std::wstring str){
    std::vector<std::wstring>* unigrams = new std::vector<std::wstring>();
    std::wregex e(L"[\\.،:)(}{?!!؟\\s]+");
    std::wsregex_token_iterator i(str.begin(), str.end(), e, -1);
    std::wsregex_token_iterator end;
    std::wstring currentToken;
    while (i != end){
        currentToken = *i++;
        if (this->stopwords.find(currentToken) == this->stopwords.end()){
            this->total_unique += 1;
            if (this->countMapSimpleRouge.find(currentToken) == this->countMapSimpleRouge.end()){
                this->countMapSimpleRouge[currentToken] = 1;
            }
            else{
                this->countMapSimpleRouge[currentToken] = this->countMapSimpleRouge[currentToken] + 1;
            }    
            unigrams->push_back(currentToken);
        }
    }
    return unigrams;
}


int Rouge::getNumberOfSharedUnigrams(std::set<std::wstring>& set1,std::set<std::wstring>& set2){
    std::vector<std::wstring> result;
    std::set_intersection(set1.begin(),set1.end(),set2.begin(),set2.end(),std::back_inserter(result));
    return result.size();
}

float Rouge::getSimilarityScore(std::wstring reference,std::wstring hypothesis,rougeType type){
    std::set<std::wstring>* refSet = getunigrams(reference);
    std::set<std::wstring>* hypSet = getunigrams(hypothesis);
    int intersectionCount = getNumberOfSharedUnigrams(*refSet,*hypSet);
    float f1,recall,presicion;
    recall = ((float)intersectionCount) / ((float) refSet->size());
    if (type == Recall){
        return recall;
    }
    presicion = ((float)intersectionCount) / ((float) hypSet->size());
    if (type == Presicion){
        return presicion;
    }
    if (type == F1){
        if (presicion + recall == 0){
            return 0.0;
        }
        return 2*presicion*recall/(presicion + recall);
    }
    return 0.0;
}

int Rouge::countIntersect(std::vector<std::set<std::wstring>*>& sentences,int i){
    std::set<std::wstring>* hyp = sentences[i];
    int intersect_count = 0;
    int just_in_sentence = 0;
    int temp;
    std::set<std::wstring>::iterator it = hyp->begin();
    while(it != hyp->end()){
        temp = this->countMapSimpleRouge[*it++];
        if (temp > 1){
            intersect_count += 1;
        }
        else{
            just_in_sentence += 1;
        }
    }
    this->total_with_respect_to_selected_target = this->total_unique - just_in_sentence;
    return intersect_count;
}

int Rouge::countIntersectWithDuplicate(std::vector<std::vector<std::wstring>*>& sentences,int i){
    std::vector<std::wstring>* hyp = sentences[i];
    int intersect_count = 0;
    int inSentence = 0;
    std::map<std::wstring,int> tempMap;
    std::vector<std::wstring>::iterator it = hyp->begin();
    while(it != hyp->end()){
        tempMap[*it] = tempMap[*it] + 1;
        it++;
    }
    std::map<std::wstring,int>::iterator mapIt = tempMap.begin();
    while(mapIt != tempMap.end()){
        inSentence += mapIt->second;
        intersect_count += std::min(mapIt->second,this->countMapSimpleRouge[mapIt->first]-mapIt->second);
        mapIt++;
    }
    this->total_with_respect_to_selected_target = this->total_unique - inSentence;
    return intersect_count;
}


int Rouge::countIntersectGreedy(std::vector<std::set<std::wstring>*>& sentences,int i){
    std::set<std::wstring>* hyp = sentences[i];
    int intersect_count = 0;
    int just_in_sentence = 0;
    int temp;
    int in_selected_set_count = 0;
    std::set<std::wstring>::iterator it = hyp->begin();
    while(it != hyp->end()){
        temp = this->countMapSimpleRouge[*it];
        if (this->countMapselectedGreedyRouge.find(*it) != this->countMapselectedGreedyRouge.end()){
            in_selected_set_count = this->countMapselectedGreedyRouge[*it];
        }
        else{
            in_selected_set_count = 0;
        }
        if (temp > 1 + in_selected_set_count){
            intersect_count += 1;
        }
        else{
            just_in_sentence += 1;
        }
        it++;
    }
    this->total_with_respect_to_selected_target = this->total_unique - just_in_sentence - this->just_in_sentence_greedy;
    return intersect_count;
}

void Rouge::submitSelectedSetGreedyRouge(std::set<std::wstring>* sentence){
    std::set<std::wstring>::iterator it = sentence->begin();
    while (it != sentence->end()){
        if (this->countMapselectedGreedyRouge.find(*it) != this->countMapselectedGreedyRouge.end()){
            this->countMapselectedGreedyRouge[*it] += 1;
        }
        else{
            this->countMapselectedGreedyRouge[*it] = 1;
        }
        if (this->countMapSimpleRouge[*it] <= 1 + this->countMapselectedGreedyRouge[*it]){
            this->just_in_sentence_greedy += 1;
        }
        it++;
    }
}

float Rouge::f1(std::vector<std::set<std::wstring>*>& sentences_unigrams,int i,rougeType type){
    int refCount,hypCount,intersectionCount;
    float presicion,recall;
    hypCount = sentences_unigrams[i]->size();
    intersectionCount = this->countIntersect(sentences_unigrams,i);
    refCount = this->total_with_respect_to_selected_target;
    recall = refCount == 0 ? 0.0 : ((float)intersectionCount) / ((float)refCount);
    if (type == Recall){
        return recall;
    }
    presicion = hypCount == 0 ? 0.0 :((float)intersectionCount) / ((float)hypCount);
    if (type == Presicion){
        return presicion;
    }
    if (type == F1){
        if (presicion + recall == 0){
            return 0.0;
        }
        else{
            return 2*presicion*recall/(presicion + recall);
        }
    }
    return 0.0;
}

float Rouge::f1WithDuplicate(std::vector<std::vector<std::wstring>*>& sentences_unigrams,int i,rougeType type){
    int refCount,hypCount,intersectionCount;
    float presicion,recall;
    hypCount = sentences_unigrams[i]->size();
    intersectionCount = this->countIntersectWithDuplicate(sentences_unigrams,i);
    refCount = this->total_with_respect_to_selected_target;
    recall = refCount == 0 ? 0.0 : ((float)intersectionCount) / ((float)refCount);
    if (type == Recall){
        return recall;
    }
    presicion = hypCount == 0 ? 0.0 :((float)intersectionCount) / ((float)hypCount);
    if (type == Presicion){
        return presicion;
    }
    if (type == F1){
        if (presicion + recall == 0){
            return 0.0;
        }
        else{
            return 2*presicion*recall/(presicion + recall);
        }
    }
    return 0.0;
}


float Rouge::f1Greedy(std::vector<std::set<std::wstring>*>& sentences_unigrams,int i,rougeType type){
    int refCount,hypCount,intersectionCount;
    float presicion,recall;
    hypCount = this->countMapselectedGreedyRouge.size();
    intersectionCount = this->countIntersectGreedy(sentences_unigrams,i);
    refCount = this->total_with_respect_to_selected_target;
    recall = refCount == 0 ? 0.0 : ((float)intersectionCount) / ((float)refCount);
    if (type == Recall){
        return recall;
    }
    presicion = hypCount == 0 ? 0.0 :((float)intersectionCount) / ((float)hypCount);
    if (type == Presicion){
        return presicion;
    }
    if (type == F1){
        if (presicion + recall == 0){
            return 0.0;
        }
        else{
            return 2*presicion*recall/(presicion + recall);
        }
    }
    return 0.0;
}


std::set<int> Rouge::getMostSimilarSentencesToDocUsingRouge(std::vector<std::wstring>& sentences,int numberOfMaskedSentences,rougeType type){
    this->countMapSimpleRouge.clear();
    this->total_unique = 0;
    std::vector<std::set<std::wstring>*> sentences_unigrams;
    sentences_unigrams.reserve(sentences.size());
    int totalCount = 0;
    for (int i=0;i<sentences.size();i++){
        sentences_unigrams.push_back(this->getunigrams(sentences[i]));
        totalCount += sentences_unigrams[i]->size();
    }
    std::vector<float> scores;
    scores.reserve(sentences_unigrams.size());
    int refCount,hypCount,intersectionCount;
    float f1,presicion,recall;
    for (int i=0;i<sentences_unigrams.size();i++){
        scores.push_back(this->f1(sentences_unigrams,i,type));
    }
    std::vector<int> indices(sentences.size(),0);
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&scores](std::size_t left, std::size_t right){return scores[left] > scores[right];});
    std::set<int> returnIndices;
    for (int i = 0;i < numberOfMaskedSentences;i++){
        returnIndices.insert(indices[i]);
    }
    this->tearDown(sentences_unigrams);
    return returnIndices;
}

std::set<int> Rouge::getMostSimilarSentencesToDocUsingRougeWithDuplicates(std::vector<std::wstring>& sentences,int numberOfMaskedSentences,rougeType type,bool ignoreNext){
    this->countMapSimpleRouge.clear();
    this->total_unique = 0;
    std::vector<std::vector<std::wstring>*> sentences_unigrams;
    sentences_unigrams.reserve(sentences.size());
    int totalCount = 0;
    for (int i=0;i<sentences.size();i++){
        sentences_unigrams.push_back(this->getunigramsWithDuplicate(sentences[i]));
        totalCount += sentences_unigrams[i]->size();
    }
    std::vector<float> scores;
    scores.reserve(sentences_unigrams.size());
    int refCount,hypCount,intersectionCount;
    float f1,presicion,recall;
    for (int i=0;i<sentences_unigrams.size();i++){
        scores.push_back(this->f1WithDuplicate(sentences_unigrams,i,type));
    }
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
    this->tearDown(sentences_unigrams);
    return returnIndices;
}

std::set<int> Rouge::getMostSimilarSentencesToDocUsingGreedyRouge(std::vector<std::wstring>& sentences,int numberOfMaskedSentences,rougeType type){
    this->countMapSimpleRouge.clear();
    this->total_unique = 0;
    this->countMapselectedGreedyRouge.clear();
    this->just_in_sentence_greedy = 0;
    std::vector<int> selected_indices;
    std::vector<std::set<std::wstring>*> sentences_unigrams;
    sentences_unigrams.reserve(sentences.size());
    int totalCount = 0;
    for (int i=0;i<sentences.size();i++){
        sentences_unigrams.push_back(this->getunigrams(sentences[i]));
        totalCount += sentences_unigrams[i]->size();
    }
    std::vector<float> scores;
    scores.reserve(sentences_unigrams.size());
    int refCount,hypCount,intersectionCount;
    float f1,presicion,recall;
    for (int i=0;i<sentences_unigrams.size();i++){
        scores.push_back(this->f1(sentences_unigrams,i,type));
    }
    std::vector<int> indices_sorted(sentences.size(),0);
    std::iota(indices_sorted.begin(), indices_sorted.end(), 0);
    std::sort(indices_sorted.begin(), indices_sorted.end(), [&scores](std::size_t left, std::size_t right){return scores[left] > scores[right];});
    std::set<int> greedy_selected_sentences;
    float bestF1 = -1.0,newF1;
    for (int i = 0; i < numberOfMaskedSentences; i++){
        newF1 = this->f1Greedy(sentences_unigrams,indices_sorted[i],type);
        if (newF1 > bestF1){
            bestF1 = newF1;
            greedy_selected_sentences.insert(indices_sorted[i]);
            this->submitSelectedSetGreedyRouge(sentences_unigrams[i]);
        }
    }
    this->tearDown(sentences_unigrams);
    return greedy_selected_sentences;
}

void Rouge::tearDown(std::vector<std::set<std::wstring>*>& sentences){
    for (int i = 0; i < sentences.size(); i++){
        delete sentences[i];
    }
}

void Rouge::tearDown(std::vector<std::vector<std::wstring>*>& sentences){
    for (int i = 0; i < sentences.size(); i++){
        delete sentences[i];
    }
}