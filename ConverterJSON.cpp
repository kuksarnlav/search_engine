#include "ConverterJSON.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

ConverterJSON::ConverterJSON(){
    std::ifstream fileInit("config.json");
    if (!fileInit.is_open()) {
        std::cerr << "Can't open config.json!\n";
    }
    nlohmann::json configJSON;
    fileInit >> configJSON;

    if (configJSON["config"] == nullptr) {
        std::cerr << "config file is empty\n";
    }
    if (configJSON["config"]["name"] == nullptr) {
        std::cerr << "name in config file is missing\n";
    }
    if (configJSON["config"]["max_responses"] == nullptr) {
        std::cerr << "max_responses in config file is missing\n";
    }
    if (configJSON["config"]["version"] == nullptr) {
        std::cerr << "version in config file is missing\n";
    }

    std::cout << "Starting " << configJSON["config"]["name"].get<std::string>() << " v. " << configJSON["config"]["version"] << '\n';
}

std::vector<std::string> ConverterJSON::GetTextDocuments(){
    std::vector<std::string> textDocuments;

    std::ifstream fileConfig("config.json");
    if (!fileConfig.is_open()) {
        std::cerr << "Can't open config.json!\n";
    }
    nlohmann::json configJSON;
    fileConfig >> configJSON;

    for (int i = 0; i < configJSON["files"].size(); i++){
        std::string filePath = configJSON["files"][i];
        std::ifstream file(filePath);
        std::string line;
        if (file.is_open()){
            std::getline(file, line);
        } else{
            std::cerr << "Unable to open file\n";
        }
        file.close();
        textDocuments.push_back(line);
    }

    return textDocuments;
}

int ConverterJSON::GetResponsesLimit(){
    std::ifstream file("config.json");
    if (!file.is_open()) {
        std::cerr << "Can't open config.json!\n";
    }
    nlohmann::json configJSON;
    file >> configJSON;

    return configJSON["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::GetRequests(){
    std::vector<std::string> requests;

    std::ifstream file("requests.json");
    if (!file.is_open()) {
        std::cerr << "Can't open config.json!\n";
    }
    nlohmann::json requestsJSON;
    file >> requestsJSON;

    for (int i = 0; i < requestsJSON["requests"].size(); i++){
        requests.push_back(requestsJSON["requests"][i]);
    }

    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers){
    std::ofstream file("answers.json");
    if (!file.is_open()) {
        std::cerr << "Can't open answers.json!\n";
    }

    std::ifstream fileInit("config.json");
    if (!fileInit.is_open()) {
        std::cerr << "Can't open config.json!\n";
    }
    nlohmann::json configJSON;
    fileInit >> configJSON;

    for (auto& innerVector : answers) {
        std::sort(innerVector.begin(), innerVector.end(), [](const auto& a, const auto& b){return a.second > b.second;});
        if (innerVector.size() > configJSON["config"]["max_responses"]){
            innerVector.erase(innerVector.begin() + configJSON["config"]["max_responses"], innerVector.end());
        }
    }

    int iRequest = 0;
    nlohmann::json requestAttributeFilling;
    for (const auto& request : answers){
        int iDocument = 0;
        nlohmann::json requestDocRelevance;
        nlohmann::json requestResultAttribute;
        bool isRequestSucceed = true;
        for (const auto& relativeIndex : request){

            if (relativeIndex.second >= 0){
                requestResultAttribute["result"] = "true";
                nlohmann::json temp;
                temp["docid"] = relativeIndex.first;
                temp["rank"] = relativeIndex.second;
                requestDocRelevance[iDocument] = {temp};
            } else {
                isRequestSucceed = false;
                requestResultAttribute["result"] = "false";
            }

            iDocument++;
        }

        std::string requestName;
        if (iRequest > 99){
            requestName = "request" + std::to_string(iRequest + 1);
        } else if (iRequest > 9){
            requestName = "request0" + std::to_string(iRequest + 1);
        } else{
            requestName = "request00" + std::to_string(iRequest + 1);
        }

        if (isRequestSucceed){
            requestAttributeFilling[requestName] = {requestResultAttribute, requestDocRelevance};
        } else {
            requestAttributeFilling[requestName] = {requestResultAttribute};
        }

        iRequest++;
    }

    nlohmann::json answersJSON;
    answersJSON["answers"] = requestAttributeFilling;
    file << answersJSON.dump(4);
}