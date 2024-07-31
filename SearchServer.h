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

    //std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input){
    void search(const std::vector<std::string>& queries_input){
        std::vector<std::vector<RelativeIndex>> documentRelativeRelevance;

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

        std::vector<std::multimap<int, std::string>> uniqueWordsOccurrence;

        for (int i = 0; i < uniqueWords.size(); i++){
            std::multimap<int, std::string> requestUniqueWordsOccurrence;
            for (auto it = uniqueWords[i].begin(); it != uniqueWords[i].end(); ++it){
                int wordOccurrence = 0;
                for (int j = 0; j < _index.GetWordCount(*it).size(); j++){
                    wordOccurrence += _index.GetWordCount(*it)[j].count;
                }
                requestUniqueWordsOccurrence.insert({wordOccurrence, *it});
            }
            uniqueWordsOccurrence.emplace_back(requestUniqueWordsOccurrence);
        }

        std::vector<size_t> rarestWordDocuments;
        for (int i = 0; i < uniqueWordsOccurrence.size(); i++){
            //uniqueWordsOccurrence[i];
        }
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