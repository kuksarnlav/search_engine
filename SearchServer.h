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

    std::vector<std::vector<std::pair<int, float>>> convertRelativeIndexToPair(const std::vector<std::vector<RelativeIndex>>& relativeIndexVec){ // not tested yet // mb
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

            std::set<std::string> uniqueWords; // unique words in request into std::set
            std::istringstream iss(queries_input[i]);
            std::string word;
            while (iss >> word){
                uniqueWords.insert(word);
            }


            std::multimap<int, std::string> uniqueWordsOccurrenceInRequest; // №request -> (frequency_in_all_requests - word) into std::map
            for (auto it = uniqueWords.begin(); it != uniqueWords.end(); it++){
                int wordOccurrence = 0;
                for (int k = 0; k < _index.GetWordCount(*it).size(); k++){
                    wordOccurrence += _index.GetWordCount(*it)[k].count;
                }
                uniqueWordsOccurrenceInRequest.insert({wordOccurrence, *it});
            }


            std::vector<size_t> rarestWordDocuments; // №document with all request words
            auto firstMultimapElement = uniqueWordsOccurrenceInRequest.begin();
            for (int j = 0; j < _index.GetWordCount(firstMultimapElement->second).size(); j++){ // rarest word №document
                rarestWordDocuments.emplace_back(_index.GetWordCount(firstMultimapElement->second)[j].doc_id);
            }


            for (int j = 0; j < _index.GetWordCount(firstMultimapElement->second).size(); j++){ // deleting №document without request word
                for (auto it = std::next(uniqueWordsOccurrenceInRequest.begin()); it != uniqueWordsOccurrenceInRequest.end(); it++){
                    for (int k = 0; k < _index.GetWordCount(it->second).size(); k++){
                        if (rarestWordDocuments[j] == _index.GetWordCount(it->second)[k].doc_id){
                            break;
                        } else if (k == _index.GetWordCount(it->second).size() - 1){
                            rarestWordDocuments.erase(rarestWordDocuments.begin() + j);
                            j--;
                        }
                    }
                }
            }


            float noMatch = -2;
            if (rarestWordDocuments.empty()){
                std::vector<RelativeIndex> rarestWordDocument;
                RelativeIndex tempIndex{0, noMatch};
                rarestWordDocument.emplace_back(tempIndex);
            } else {
                float absoluteRankMax;

                for (int j = 0; j < rarestWordDocuments.size(); j++){ // : documents
                    std::vector<RelativeIndex> rarestWordDocument;
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
                    documentRelativeRelevance.emplace_back(rarestWordDocument);
                }

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

/*using namespace std;

TEST(TestCaseSearchServer, TestSimple) {
    const vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const vector<string> request = {"milk water", "sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
        {
            {2, 1}, {0, 0.7}, {1, 0.3}
        },
        {

        }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5){
    const vector<string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };

    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
        {
            {7, 1}, {14, 1}, {0, 0.666666687}, {1, 0.666666687}, {2, 0.666666687}
        }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}*/