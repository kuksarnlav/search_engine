#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <sstream>

struct Entry {
    size_t doc_id, count = 0;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    InvertedIndex(const InvertedIndex& other);

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::mutex mtx;

    void ProcessDocument(const std::string& doc, int doc_id);
};