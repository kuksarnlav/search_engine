#pragma once
#include <vector>
#include <string>
#include <set>
#include <map>
#include <iostream>

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

        std::vector<std::vector<RelativeIndex>> docRelevance;
        for (int i = 0; i < queries_input.size(); i++){

            std::set<std::string> uniqueWords;
            {
                std::istringstream iss(queries_input[i]);
                std::string word;

                while (iss >> word){
                    uniqueWords.insert(word);
                }
            }


            std::multimap<int, std::string> uniqueWordsOccurrence;
            for (auto it = uniqueWords.begin(); it != uniqueWords.end(); it++){
                int wordOccurrence = 0;
                for (int k = 0; k < _index.GetWordCount(*it).size(); k++){
                    wordOccurrence += _index.GetWordCount(*it)[k].count;
                }
                uniqueWordsOccurrence.insert({wordOccurrence, *it});
            }


            std::vector<size_t> rarestWordDocs;
            {
                auto firstMultimapElement = uniqueWordsOccurrence.begin();

                for (int j = 0; j < _index.GetWordCount(firstMultimapElement->second).size(); j++){
                    rarestWordDocs.emplace_back(_index.GetWordCount(firstMultimapElement->second)[j].doc_id);
                }
                for (int j = 0; j < _index.GetWordCount(firstMultimapElement->second).size(); j++){
                    for (auto it = std::next(uniqueWordsOccurrence.begin()); it != uniqueWordsOccurrence.end(); it++){
                        for (int k = 0; k < _index.GetWordCount(it->second).size(); k++){
                            if (rarestWordDocs[j] == _index.GetWordCount(it->second)[k].doc_id){
                                break;
                            } else if (k == _index.GetWordCount(it->second).size() - 1){
                                rarestWordDocs.erase(rarestWordDocs.begin() + j);
                                if (j != _index.GetWordCount(firstMultimapElement->second).size() - 1){
                                    j--;
                                }
                            }
                        }
                    }
                }
            }


            float noMatch = -1;
            if (rarestWordDocs.empty()){
                std::vector<RelativeIndex> rarestWordDocument;
                RelativeIndex tempIndex{0, noMatch};
                rarestWordDocument.emplace_back(tempIndex);
                docRelevance.emplace_back(rarestWordDocument);
            } else {
                float absoluteRankMax;
                std::vector<RelativeIndex> rarestWordDoc;
                for (int j = 0; j < rarestWordDocs.size(); j++){
                    float absoluteRank = 0;
                    for (auto it = uniqueWords.begin(); it != uniqueWords.end(); it++){
                        int k = 0;
                        while (_index.GetWordCount(*it)[k].doc_id != rarestWordDocs[j]){
                            k++;
                        }
                        absoluteRank += _index.GetWordCount(*it)[k].count;
                    }
                    if (absoluteRankMax < absoluteRank){
                        absoluteRankMax = absoluteRank;
                    }
                    RelativeIndex tempIndex{rarestWordDocs[j], absoluteRank};
                    rarestWordDoc.emplace_back(tempIndex);
                }
                docRelevance.emplace_back(rarestWordDoc);

                for (int j = 0; j < docRelevance[i].size(); j++){
                    docRelevance[i][j].rank /= absoluteRankMax;
                }
            }
        }

        return docRelevance;
    }

private:
    InvertedIndex _index;
};