#include <iostream>
#include "nlohmann/json.hpp"
#include <fstream>

class ConverterJSON{
    void initConfigJSON(){
        std::ofstream file("config.json");

        nlohmann::json configAttributeFilling;
        configAttributeFilling["name"] = "search_engine";
        configAttributeFilling["version"] = 0.1;
        configAttributeFilling["max_responses"] = 5;

        // N of words in a file <= 1000, latin lowercase, N of letters in a word <= 100, >=1 spaces between words
        // if a file not found, then drop an error, but the program continues
        nlohmann::json filesAttributeFilling;
        filesAttributeFilling[0] = "resources/file001.txt";
        filesAttributeFilling[1] = "resources/file002.txt";
        filesAttributeFilling[2] = "resources/file003.txt";

        nlohmann::json config;
        config["config"] = configAttributeFilling;
        config["files"] = filesAttributeFilling;
        file << config.dump(4);
    }
    void initRequestsJSON(){
        std::ofstream file("requests.json");

        // N of requests <= 1000
        // N of words in a request <= 10 && >= 1, only latin letters, >=1 spaces between words
        nlohmann::json requestsAttributeFilling;
        requestsAttributeFilling[0] = "some words";
        requestsAttributeFilling[1] = "some words";
        requestsAttributeFilling[2] = "some words";

        nlohmann::json requests;
        requests["requests"] = requestsAttributeFilling;
        file << requests.dump(4);
    }
    void initAnswersJSON(){
        std::ofstream file("answers.json");


    }
};

int main() {
    ;
}
