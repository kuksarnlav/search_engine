#include <iostream>
#include "nlohmann/json.hpp"
#include <fstream>

class ConverterJSON{
public:
    ConverterJSON() = default;
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

        nlohmann::json request001ResultAttributeFilling;
        request001ResultAttributeFilling["result"] = "true";
        nlohmann::json request001RelevanceAttributeFilling;
        nlohmann::json temp;
        temp["docid"] = 0;
        temp["rank"] = 0.989;
        request001RelevanceAttributeFilling[0] = temp;
        temp["docid"] = 1;
        temp["rank"] = 0.897;
        request001RelevanceAttributeFilling[1] = temp;
        temp["docid"] = 2;
        temp["rank"] = 0.750;
        request001RelevanceAttributeFilling[2] = temp;
        temp["docid"] = 3;
        temp["rank"] = 0.670;
        request001RelevanceAttributeFilling[3] = temp;
        temp["docid"] = 4;
        temp["rank"] = 0.561;
        request001RelevanceAttributeFilling[4] = temp;
        nlohmann::json request001AttributeFilling;
        request001AttributeFilling["request001"] = {request001ResultAttributeFilling, request001RelevanceAttributeFilling};

        nlohmann::json request002ResultAttributeFilling;
        request002ResultAttributeFilling["result"] = "true";
        nlohmann::json request002RelevanceAttributeFilling;
        temp["docid"] = 0;
        temp["rank"] = 0.769;
        request002RelevanceAttributeFilling = temp;
        nlohmann::json request002AttributeFilling;
        request002AttributeFilling["request002"] = {request002ResultAttributeFilling, request002RelevanceAttributeFilling};

        nlohmann::json request003ResultAttributeFilling;
        request003ResultAttributeFilling["result"] = "false";
        nlohmann::json request003AttributeFilling;
        request003AttributeFilling["request003"] = {request003ResultAttributeFilling};

        nlohmann::json answers;
        answers["answers"] = {request001AttributeFilling, request002AttributeFilling, request003AttributeFilling};
        file << answers.dump(4);
    }

    std::vector<std::string> GetTextDocuments(){
        std::vector<std::string> textDocuments;

        std::ifstream file("config.json");
        nlohmann::json configJSON;
        file >> configJSON;

        for (int i = 0; i < configJSON["files"].size(); i++){
            textDocuments.push_back(configJSON["files"][i]);
        }

        return textDocuments;
    }

    int GetResponsesLimit(){
        std::ifstream file("config.json");
        nlohmann::json configJSON;
        file >> configJSON;

        return configJSON["config"]["max_responses"];
    }

    std::vector<std::string> GetRequests(){
        std::vector<std::string> requests;

        std::ifstream file("requests.json");
        nlohmann::json requestsJSON;
        file >> requestsJSON;

        for (int i = 0; i < requestsJSON["requests"].size(); i++){
            requests.push_back(requestsJSON["requests"][i]);
        }

        for (int i = 0; i < requests.size(); i++){
            std::cout << requests[i] << ' ';
        }

        return requests;
    }
// Положить в файл answers.json результаты поисковых запросов
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers){
        ;
    }
};

int main() {
    ConverterJSON json;
    json.initAnswersJSON();
    json.initConfigJSON();
    json.initRequestsJSON();

}
