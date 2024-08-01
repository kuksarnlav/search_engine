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

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input){

        std::vector<int> maxAbsoluteRelevance;
        std::vector<std::set<std::string>> uniqueWords;

        for (int i = 0; i < queries_input.size(); i++){
            maxAbsoluteRelevance.emplace_back(0);
            std::istringstream iss(queries_input[i]);
            std::string word;
            std::set<std::string> words;
            while (iss >> word){
                maxAbsoluteRelevance[i]++;
                words.insert(word);
            }
            uniqueWords.emplace_back(words);
        }

        std::vector<std::multimap<int, std::string>> uniqueWordsOccurrenceInAllRequests;

        for (int i = 0; i < uniqueWords.size(); i++){
            std::multimap<int, std::string> requestUniqueWordsOccurrenceInOneRequest;
            for (auto it = uniqueWords[i].begin(); it != uniqueWords[i].end(); ++it){
                int wordOccurrence = 0;
                for (int j = 0; j < _index.GetWordCount(*it).size(); j++){
                    wordOccurrence += _index.GetWordCount(*it)[j].count;
                }
                requestUniqueWordsOccurrenceInOneRequest.insert({wordOccurrence, *it});
            }
            uniqueWordsOccurrenceInAllRequests.emplace_back(requestUniqueWordsOccurrenceInOneRequest);
        }

        size_t noMatch = -2;
        std::vector<std::vector<RelativeIndex>> documentRelativeRelevance;

        for (int i = 0; i < uniqueWordsOccurrenceInAllRequests.size(); i++){ // for all requests
            std::vector<int> rarestWordDocuments;
            auto firstVectorElement = uniqueWordsOccurrenceInAllRequests.begin();
            auto firstMultimapElement = firstVectorElement->begin();
            for (int j = 0; j < _index.GetWordCount(firstMultimapElement->second).size(); j++){
                rarestWordDocuments.emplace_back(_index.GetWordCount(firstMultimapElement->second)[j].doc_id);
            }

            for (int j = 0; j < _index.GetWordCount(firstMultimapElement->second).size(); j++){ // here request unique words length, that mean 2, 'cause milk is in all 2 docs
                for (auto it = std::next(uniqueWordsOccurrenceInAllRequests[i].begin()); it != uniqueWordsOccurrenceInAllRequests[i].end(); it++){ // here we get all other words to check
                    for (int k = 0; k < _index.GetWordCount(it->second).size(); k++){ // here we get array of Entry struct, we need to check doc_id's in rarestWordDocuments and that array
                        if (rarestWordDocuments[j] == _index.GetWordCount(it->second)[k].doc_id){
                            break;
                        } else if (k == _index.GetWordCount(it->second).size() - 1){
                            rarestWordDocuments[j] = noMatch;
                        }
                    }
                }
            }

            for (int j = 0; j < rarestWordDocuments.size(); j++){
                if (rarestWordDocuments[j] == noMatch){
                    rarestWordDocuments.erase(rarestWordDocuments.begin() + j);
                    j--;
                }
            }

            float absoluteRankMax = (float)noMatch;

            if (rarestWordDocuments.empty()){

                std::vector<RelativeIndex> rarestWordDocument;
                RelativeIndex tempIndex{noMatch, (float)noMatch};
                rarestWordDocument.emplace_back(tempIndex);

                documentRelativeRelevance.emplace_back(rarestWordDocument);
            } else {
                for (size_t j = 0; j < rarestWordDocuments.size(); j++){
                    std::vector<RelativeIndex> rarestWordDocument;
                    for (auto it = uniqueWordsOccurrenceInAllRequests[i].begin(); it != uniqueWordsOccurrenceInAllRequests[i].end(); it++){

                        int k = 0;
                        float absoluteRank;
                        while (_index.GetWordCount(it->second)[k].doc_id != j){
                            absoluteRank = _index.GetWordCount(it->second)[k].count;
                            k++;
                        }
                        if (absoluteRankMax < absoluteRank){
                            absoluteRankMax = absoluteRank;
                        }

                        RelativeIndex tempIndex{j, absoluteRank};
                        rarestWordDocument.emplace_back(tempIndex);
                    }
                    documentRelativeRelevance.emplace_back(rarestWordDocument);
                }
            }

            for (int j = 0; j < documentRelativeRelevance.size(); j++){
                for (int k = 0; k < documentRelativeRelevance[j].size(); k++){
                    documentRelativeRelevance[j][k].rank /= absoluteRankMax;
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