#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <sstream>
#include "gtest/gtest.h"

struct Entry{
    size_t doc_id, count = 0;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex{
public:
    InvertedIndex() = default;

    InvertedIndex(const InvertedIndex& other){
        docs = other.docs;
        freq_dictionary = other.freq_dictionary;
    }

    void UpdateDocumentBase(std::vector<std::string> input_docs){
        docs = input_docs;

        std::vector<std::thread> threads;
        for (int i = 0; i < docs.size(); i++){
            threads.emplace_back(&InvertedIndex::ProcessDocument, this, docs[i], i);
        }

        for (auto& thread : threads){
            thread.join();
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word){
        return freq_dictionary[word];
    }

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::mutex mtx;

    void ProcessDocument(const std::string& doc, int doc_id) {
        std::istringstream iss(doc);
        std::string word;

        while (iss >> word){
            std::lock_guard<std::mutex> lock(mtx);
            if (freq_dictionary[word].size() != doc_id + 1){
                Entry insertedStructure;
                insertedStructure.doc_id = doc_id;
                insertedStructure.count++;
                freq_dictionary[word].push_back(insertedStructure);
            } else{
                freq_dictionary[word][doc_id].count++;
            }
        }
    }
};


/*
using namespace std;

void TestInvertedIndexFunctionality(const vector<string>& docs, const vector<string>& requests, const std::vector<vector<Entry>>& expected){
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (auto& request : requests){
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic){
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
            {
                {0, 1}
            }, {
                {0, 1}, {1, 3}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2){
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuchino"};
    const vector<vector<Entry>> expected = {
            {
                {0, 4}, {1, 1}, {2, 5}
            }, {
                {0, 2}, {1, 2}, {2, 5}
            }, {
                {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
            {

            }, {
                {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}*/
