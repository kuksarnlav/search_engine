#pragma once
#include "ConverterJSON.h"
#include <string>
#include <vector>
#include <map>

struct Entry{
    size_t doc_id, count = 0;
};

class InvertedIndex{
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs){ //into arguments, we'll put the GetTextDocuments()
        docs = input_docs;

        for (int i = 0; i < docs.size(); i++){
            std::istringstream iss(docs[i]);
            std::string word;

            while (iss >> word){ // here we get a single word
                if (freq_dictionary[word].size() != i + 1){
                    Entry insertedStructure;
                    insertedStructure.doc_id = i;
                    insertedStructure.count++;
                    freq_dictionary[word].push_back(insertedStructure);
                } else{
                    freq_dictionary[word][i].count++;
                }
            }
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word){
        ;
    }
private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};