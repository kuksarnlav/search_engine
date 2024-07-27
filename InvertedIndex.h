#include <string>
#include <vector>
#include <map>

struct Entry {
    size_t doc_id, count;
};

class InvertedIndex{
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};