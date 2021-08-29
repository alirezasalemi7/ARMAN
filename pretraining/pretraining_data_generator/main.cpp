
#include "./rouge/rouge.hpp"
#include "./semantic_similarity/SemanticSimilarity.hpp"
#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include <boost/random/uniform_real.hpp>
#include <cstdlib>
#include <numeric>
#include <random>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <codecvt>
#include <omp.h>
#include "./utils/debug.hpp"

#define MAX_SENTENCE 20
#define MIN_SENTENCE 3


float masked_sentence_ratio = 0.3;
float shuffle_length_ration = 0.4;
std::wstring mask_token = L"<mask>";
std::wstring sentence_sepration_pattern = L"[!\\.?⸮؟]+[ \n]*";
float MSR_rate = 0.9;
float SH_rate = 1.0;
int input_size = 1000;
bool reorder_sentence = false;
bool single_mask = false;
float probs_cum_sum[4] = {0.8,0.8,1.0,1.0};
std::string type = "rouge";


void save_results(std::string directory,std::vector<std::wstring>& inputs,std::vector<std::wstring>& targets){
    boost::filesystem::create_directory("./"+directory);
    std::locale utf8( std::locale(), new std::codecvt_utf8_utf16<wchar_t> );
    std::wofstream inputFile;
    inputFile.imbue(utf8);
    inputFile.open(directory+"/inputs.txt",std::ios_base::app);
    std::wofstream targetFile;
    targetFile.imbue(utf8);
    targetFile.open(directory+"/targets.txt",std::ios_base::app);
    for (int i = 0;i < inputs.size();i++){
        inputFile << inputs[i] << "\n";
        targetFile << targets[i] << "\n";
    }
    inputFile.close();
    targetFile.close();
}

void selectSentences(){

}

void seprateSentences(std::vector<std::wstring>& corpus,std::vector<std::wstring>& sentences,std::wstring corpus_sentence,std::wregex reg){
    std::wsregex_token_iterator it_sentence(corpus_sentence.begin(), corpus_sentence.end(), reg, -1);
    std::wsregex_token_iterator it_sep(corpus_sentence.begin(), corpus_sentence.end(), reg, 0);
    std::wsregex_token_iterator end;
    std::wstring temp_sentence;
    std::wstring temp_sep;
    while (it_sentence != end){
        temp_sentence = *it_sentence;
        if (it_sep!=end){
            temp_sep = *it_sep;
            it_sep++;
        }
        else{
            temp_sep = L"";
        }
        sentences.push_back(temp_sentence + temp_sep);
        it_sentence++;
    }
}

void shuffleSentence(std::wstring& str,boost::uniform_real<float>& random_gen,std::mt19937& gen,float shuffle_length_max_ratio){
    std::wregex e(L"[\\.،:;-=/,<>*&^%$#@+~\"؛\\-`_\\)(}{?!!؟\\s۱۲۳۴۵۶۷۸۹۰1234567890]+");
    std::wsregex_token_iterator it_words(str.begin(), str.end(), e, -1);
    std::wsregex_token_iterator it_others(str.begin(), str.end(), e, 0);
    std::wsregex_token_iterator end;
    std::vector<std::wstring> words,others;
    while (it_words!=end) {
        words.push_back(*it_words);
        it_words++;
    }
    while (it_others!=end) {
        others.push_back(*it_others);
        it_others++;
    }
    float start_prob = random_gen(gen),shuffle_length_prob = random_gen(gen);
    int start_index = (int) (start_prob * (words.size()*(1-shuffle_length_max_ratio)));
    int size = words.size();
    int shuffle_length = std::max((int)(words.size() * shuffle_length_max_ratio * shuffle_length_prob),std::min(2,size));
    // std::cout << start_index << " " << shuffle_length << " " << words.size() << " " << others.size() << "\n";
    std::shuffle(words.begin()+start_index,words.begin()+std::min(size,start_index+shuffle_length),std::default_random_engine {});
    str = L"";
    int index = 0;
    if (others.size() == words.size() + 1){
        str = str + others[0];
        index = 1;
    }
    float put_token = random_gen(gen);
    for (int i = 0; i < words.size(); i++){
        str += words[i] + others[i+index];
    }
}

void shuffle_sentences(std::vector<std::wstring>& sentences){
    auto rng = std::default_random_engine {};
    std::shuffle(sentences.begin(),sentences.end(),rng);
}

void mask_sentence_for_dataset(Rouge* rouge,SemanticSimilarity* similarity,std::vector<std::wstring>& corpus,std::vector<std::wstring>& inputs,std::vector<std::wstring>& targets){
    // std::cout << "here4\n";
    std::wstring corpus_sentence;
    boost::uniform_real<float> random_gen(0,1);
    std::mt19937 gen;
    std::wregex reg(sentence_sepration_pattern);

    for (int i = 0;i < corpus.size();i++){
        // std::cout << corpus.size() << " " << i << "\n";
        corpus_sentence = corpus[i];
        std::vector<std::wstring> sentences;
        seprateSentences(corpus,sentences,corpus_sentence,reg);
        // std::cout << sentences.size() << "\n";
        if (sentences.size() == 0){
            continue;
        }
        std::set<int> indices;
        // std::cout << "here3\n";
        float mask_order_rand = random_gen(gen);
        if (type == "greedy_rouge"){
            indices = rouge->getMostSimilarSentencesToDocUsingGreedyRouge(sentences,std::max({1,(int)(sentences.size()*masked_sentence_ratio)}),F1);
        }
        else if(type == "rouge") {
            indices = rouge->getMostSimilarSentencesToDocUsingRouge(sentences,std::max({1,(int)(sentences.size()*masked_sentence_ratio)}),F1);
        }
        else if(type == "rouge_duplicate"){
            indices = rouge->getMostSimilarSentencesToDocUsingRougeWithDuplicates(sentences,std::max({1,(int)(sentences.size()*masked_sentence_ratio)}),F1,false);
        }
        else if(type == "semantic" || (type == "semantic_with_reordering" && mask_order_rand < MSR_rate)){
            indices = similarity->getMostSimilarSentencesToDocUsingSemantics(sentences,std::max({1,(int)(sentences.size()*masked_sentence_ratio)}),SEM_F1,!single_mask);
        }
        else if(type == "semantic_with_reordering" && mask_order_rand >= MSR_rate){
            // std::cout << "here\n";
            indices = similarity->getMostSimilarSentencesToDocUsingSemantics(sentences,std::max({1,(int)(sentences.size()*masked_sentence_ratio)}),SEM_F1,false);
            // std::cout << "here2\n";
        }
        std::wstring input = L"", target = L"";
        bool prev_mask = false;
        for (int i = 0;i<sentences.size();i++){
            if (indices.find(i) != indices.end()){
                target = target + L" " + sentences[i];
                float rand = random_gen(gen);
                if (type == "semantic_with_reordering" && mask_order_rand >= MSR_rate){
                    continue;
                }
                if (rand < probs_cum_sum[0]){
                    //mask
                    if (!(prev_mask && single_mask)){
                        // std::cout << "here\n";
                        input = input + mask_token;
                    }
                    prev_mask = true;
                }
                else if(rand < probs_cum_sum[1]){
                    prev_mask = false;
                    //random: sentence shuffling
                }
                else if(rand < probs_cum_sum[2]){
                    prev_mask = false;
                    float rand = random_gen(gen);
                    if (reorder_sentence && (rand <= SH_rate)){
                        // reorder sentence
                        std::wstring temp = sentences[i];
                        shuffleSentence(temp,random_gen,gen,shuffle_length_ration);
                        input = input + temp;
                    }
                    else{
                        // do nothing    
                        input = input + sentences[i]; 
                    }
                }
                else{
                    prev_mask = false;
                    //remove
                }
            }
            else{
                prev_mask = false;
                input = input + sentences[i];
            }
        }
        if (type == "semantic_with_reordering" && mask_order_rand >= MSR_rate){
            input = L"";
            shuffle_sentences(sentences);
            for (int i = 0;i<sentences.size();i++) {
                input = input + sentences[i];
            }
        }
        inputs.push_back(input);
        targets.push_back(target);
    }
}

int main(int argc,char** argv){
    std::string corpusAddr(argv[1]);
    std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
    std::wifstream corpusFile;
    corpusFile.imbue(utf8);
    corpusFile.open(corpusAddr);
    bool end = true;
    int total = 0;
    
    if (argc > 3){
        type = argv[3]; // type
    }
    if (argc > 4){
        probs_cum_sum[0] = std::stof(argv[4]);
    }
    if (argc > 5){
        probs_cum_sum[1] = std::stof(argv[5]);
    }
    if (argc > 6){
        probs_cum_sum[2] = std::stof(argv[6]);
    }
    if (argc > 7){
        probs_cum_sum[3] = std::stof(argv[7]);
    }
    if (argc > 8){
        single_mask = true;
    }
    if (argc > 9){
        reorder_sentence = true;
    }
    if (argc > 10){
        masked_sentence_ratio = std::stof(argv[10]);
    }
    if (argc > 11){
        shuffle_length_ration = std::stof(argv[11]);
    }
    if (argc > 12){
        MSR_rate = std::stof(argv[12]);
    }
    if (argc > 13){
        SH_rate = std::stof(argv[13]);
    }
    if (argc > 14){
        input_size = std::stoi(argv[14]);
    }
    SemanticVectors* vectors;
    if (type == "semantic" || type == "semantic_with_reordering"){
        vectors = loadVectors("vectors.vec","words.txt","hash2index.txt","ngram.vec");
    }
    #pragma omp parallel shared(end,corpusFile,input_size,reorder_sentence,total,type,vectors,probs_cum_sum) num_threads(2)
    {
    std::wstring line;
    std::vector<std::wstring> corpus;
    end = false;
    Rouge* rouge;
    SemanticSimilarity* similarity;
    if (type == "semantic" || type == "semantic_with_reordering"){
        similarity = new SemanticSimilarity(vectors,"words.txt","stop_words.txt","hash2index.txt");
    }
    else{
        rouge = new Rouge("stop_words.txt");
    }
    do {
        corpus.clear();
        corpus.shrink_to_fit();
        corpus.reserve(input_size);
        #pragma omp critical
        {
        if (end == false){
            while (std::getline(corpusFile,line)){
                corpus.push_back(line);
                if (corpus.size() > input_size){
                    end = false;
                    break;
                }
                else {
                    end = true;
                }
            }
        }
        }
        // std::cout<<corpus.size()<<std::endl;
        std::vector<std::wstring> inputs,targets;
        targets.reserve(input_size);
        inputs.reserve(input_size);
        mask_sentence_for_dataset(rouge,similarity,corpus,inputs,targets);
        #pragma omp atomic
        total += inputs.size();
        #pragma omp critical
        {
        save_results(argv[2],inputs,targets);
        }
        inputs.clear();
        targets.clear();
        inputs.shrink_to_fit();
        targets.shrink_to_fit();
    } while(!end);
    }
    std::wcout << total << std::endl;
    corpusFile.close();
}
