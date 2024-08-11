#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <sstream>

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

    void ProcessDocument(const std::string& doc, int doc_id){
        std::istringstream iss(doc);
        std::string word;

        while (iss >> word){
            if (freq_dictionary[word].empty()){
                Entry insertedStructure;
                insertedStructure.doc_id = doc_id;
                insertedStructure.count++;
                freq_dictionary[word].emplace_back(insertedStructure);
            } else {
                for (int j = 0; j < freq_dictionary[word].size(); j++){
                    if (freq_dictionary[word][j].doc_id == doc_id){
                        freq_dictionary[word][j].count++;
                        break;
                    } else if (j == freq_dictionary[word].size() - 1){
                        Entry insertedStructure;
                        insertedStructure.doc_id = doc_id;
                        insertedStructure.count++;
                        freq_dictionary[word].emplace_back(insertedStructure);
                        break;
                    }
                }
            }
        }
    }
};