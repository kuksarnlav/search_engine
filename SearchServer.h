#pragma once
#include <vector>
#include <string>
#include <set>
#include <map>
#include <iostream>
#include "gtest/gtest.h"

struct RelativeIndex{
    size_t doc_id;
    float rank;

    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    SearchServer(InvertedIndex idx) : _index(idx){};

    std::vector<std::vector<std::pair<int, float>>> convertRelativeIndexToPair(const std::vector<std::vector<RelativeIndex>>& relativeIndexVec){
        std::vector<std::vector<std::pair<int, float>>> pairVec;
        for (const auto& innerVec : relativeIndexVec){
            std::vector<std::pair<int, float>> innerPairVec;
            for (const auto& relativeIndex : innerVec){
                innerPairVec.emplace_back(static_cast<int>(relativeIndex.doc_id), relativeIndex.rank);
            }
            pairVec.push_back(innerPairVec);
        }
        return pairVec;
    }

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input){

        std::vector<std::vector<RelativeIndex>> documentRelativeRelevance;
        for (int i = 0; i < queries_input.size(); i++){

            std::set<std::string> uniqueWords;
            {
                std::istringstream iss(queries_input[i]);
                std::string word;

                while (iss >> word){
                    uniqueWords.insert(word);
                }
            }


            std::multimap<int, std::string> uniqueWordsOccurrenceInRequest;
            for (auto it = uniqueWords.begin(); it != uniqueWords.end(); it++){
                int wordOccurrence = 0;
                for (int k = 0; k < _index.GetWordCount(*it).size(); k++){
                    wordOccurrence += _index.GetWordCount(*it)[k].count;
                }
                uniqueWordsOccurrenceInRequest.insert({wordOccurrence, *it});
            }


            std::vector<size_t> rarestWordDocuments;
            {
                auto firstMultimapElement = uniqueWordsOccurrenceInRequest.begin();

                for (int j = 0; j < _index.GetWordCount(firstMultimapElement->second).size(); j++){
                    rarestWordDocuments.emplace_back(_index.GetWordCount(firstMultimapElement->second)[j].doc_id);
                }
                for (int j = 0; j < _index.GetWordCount(firstMultimapElement->second).size(); j++){
                    for (auto it = std::next(uniqueWordsOccurrenceInRequest.begin()); it != uniqueWordsOccurrenceInRequest.end(); it++){
                        for (int k = 0; k < _index.GetWordCount(it->second).size(); k++){
                            if (rarestWordDocuments[j] == _index.GetWordCount(it->second)[k].doc_id){
                                break;
                            } else if (k == _index.GetWordCount(it->second).size() - 1){
                                rarestWordDocuments.erase(rarestWordDocuments.begin() + j);
                                if (j != _index.GetWordCount(firstMultimapElement->second).size() - 1){
                                    j--;
                                }
                            }
                        }
                    }
                }
            }


            float noMatch = -1;
            if (rarestWordDocuments.empty()){
                std::vector<RelativeIndex> rarestWordDocument;
                RelativeIndex tempIndex{0, noMatch};
                rarestWordDocument.emplace_back(tempIndex);
                documentRelativeRelevance.emplace_back(rarestWordDocument);
            } else {
                float absoluteRankMax;
                std::vector<RelativeIndex> rarestWordDocument;
                for (int j = 0; j < rarestWordDocuments.size(); j++){ // : documents
                    float absoluteRank = 0;
                    for (auto it = uniqueWords.begin(); it != uniqueWords.end(); it++){
                        int k = 0;
                        while (_index.GetWordCount(*it)[k].doc_id != rarestWordDocuments[j]){
                            k++;
                        }
                        absoluteRank += _index.GetWordCount(*it)[k].count;
                    }
                    if (absoluteRankMax < absoluteRank){
                        absoluteRankMax = absoluteRank;
                    }
                    RelativeIndex tempIndex{rarestWordDocuments[j], absoluteRank};
                    rarestWordDocument.emplace_back(tempIndex);
                }
                documentRelativeRelevance.emplace_back(rarestWordDocument);

                for (int j = 0; j < documentRelativeRelevance[i].size(); j++){
                    documentRelativeRelevance[i][j].rank /= absoluteRankMax;
                }
            }
        }

        return documentRelativeRelevance;
    }

private:
    InvertedIndex _index;
};