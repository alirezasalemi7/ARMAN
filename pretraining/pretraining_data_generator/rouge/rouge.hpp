#ifndef ROUGE_HPP
#define ROUGE_HPP

#include <string>
#include <set>
#include <cmath>
#include <regex>
#include <algorithm>
#include <fstream>
#include <map>
#include <vector>
#include <numeric>
#include <iostream>
#include <boost/random/uniform_real.hpp>
#include <random>


enum rougeType {F1,Recall,Presicion};

class Rouge
{
private:
    boost::uniform_real<float> random_gen;
    std::mt19937 gen;
    std::map<std::wstring,int> countMapSimpleRouge;
    std::map<std::wstring,int> countMapselectedGreedyRouge;
    int total_unique;
    int total_with_respect_to_selected_target;
    int just_in_sentence_greedy;
    std::set<std::wstring> stopwords;
    std::set<std::wstring>* getunigrams(std::wstring);
    std::vector<std::wstring>* getunigramsWithDuplicate(std::wstring);
    int countIntersect(std::vector<std::set<std::wstring>*>&,int);
    int countIntersectWithDuplicate(std::vector<std::vector<std::wstring>*>&,int);
    int countIntersectGreedy(std::vector<std::set<std::wstring>*>&,int);
    int getNumberOfSharedUnigrams(std::set<std::wstring>&,std::set<std::wstring>&);
    float f1(std::vector<std::set<std::wstring>*>&,int,rougeType type);
    float f1WithDuplicate(std::vector<std::vector<std::wstring>*>&,int,rougeType type);
    float f1Greedy(std::vector<std::set<std::wstring>*>&,int,rougeType type);
    void submitSelectedSetGreedyRouge(std::set<std::wstring>*);
    void tearDown(std::vector<std::set<std::wstring>*>&);
    void tearDown(std::vector<std::vector<std::wstring>*>&);
public:
    Rouge(std::string);
    float getSimilarityScore(std::wstring reference,std::wstring hypothisis,rougeType type);
    std::set<int> getMostSimilarSentencesToDocUsingRouge(std::vector<std::wstring>& sentences,int numberOfMaskedSentences,rougeType type);
    std::set<int> getMostSimilarSentencesToDocUsingRougeWithDuplicates(std::vector<std::wstring>& sentences,int numberOfMaskedSentences,rougeType type,bool ignoreNext = true);
    std::set<int> getMostSimilarSentencesToDocUsingGreedyRouge(std::vector<std::wstring>& sentences,int numberOfMaskedSentences,rougeType type);
};


#endif