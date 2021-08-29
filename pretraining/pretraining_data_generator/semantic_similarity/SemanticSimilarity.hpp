#ifndef SEM_SIMILARITY
#define SEM_SIMILARITY

#include <string>
#include <set>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>
#include <boost/random/uniform_real.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <numeric>
#include <random>
// #include "./matrix_mult.cuh"
// #include <cuda_runtime.h>
#include <iostream>
#include <codecvt>
#include <fstream>

enum ScoreType {SEM_F1,SEM_Precision,SEM_Recall};

typedef struct
{
    float* weightVectors;
    float* ngramVectors;

} SemanticVectors;

SemanticVectors* loadVectors(std::string vectorsAddress,std::string wordsAddress,std::string hash2IndexAddress,std::string ngramsVectorAddress);


class SemanticSimilarity
{
private:
    /* data */
    float* weightVectors;
    float* oovVector;
    float* ngramVectors;
    int ngram_size;
    int maximumWordCount;
    int vectorSize;
    int total_tokens;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    boost::uniform_real<float> random_gen;
    std::mt19937 gen;
    std::map<std::wstring,int> stringToVectorIndexMap;
    std::map<uint32_t,int> ngarmsHash2Index;
    std::vector<std::pair<int,int>> sentencesStartEndIndex;
    std::set<std::wstring> stopwords;
    float* extractDocumentMatrix(std::vector<std::wstring>& sentences);
    float getF1SimScore(float* result,std::pair<int,int> sentence);
    float getRecallSimScore(float* result,std::pair<int,int> sentence);
    float getPrecisionSimScore(float* result,std::pair<int,int> sentence);
    void similarityMultiply(float* matrix,float* out,int input_size,int vector_size);
    uint32_t hash(std::wstring str);
public:
    SemanticSimilarity(SemanticVectors* vectors,std::string wordsAddress,std::string stopwordsAddress,std::string hash2IndexAddress,int max_words_per_sentence = 512);
    std::map<std::wstring,int> getWordsIndex(){return this->stringToVectorIndexMap;}
    float* makeVector(std::wstring oov);
    float* getVectors(){return this->weightVectors;}
    std::set<int> getMostSimilarSentencesToDocUsingSemantics(std::vector<std::wstring>& sentences,int numberOfMaskedSentences,ScoreType type,bool ignoreNext = true);
};


#endif