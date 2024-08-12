#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class ConverterJSON{
public:
    ConverterJSON(){
        std::ifstream fileInit("config.json");
        nlohmann::json configJSON;
        fileInit >> configJSON;

        if (configJSON["config"] == nullptr) {
            std::cerr << "config file is empty\n";
        }

        std::cout << "Starting " << configJSON["config"]["name"].get<std::string>() << '\n';
    }

    std::vector<std::string> GetTextDocuments(){
        std::vector<std::string> textDocuments;

        std::ifstream fileConfig("config.json");
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

        return requests;
    }

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers){
        std::ofstream file("answers.json");

        int iRequest = 0;
        nlohmann::json requestAttributeFilling;
        for (const auto& request : answers){
            int iDocument = 0;
            nlohmann::json requestDocumentRelevance;
            nlohmann::json requestResultAttribute;
            bool isRequestSucceed = true;
            for (const auto& relativeIndex : request){

                if (relativeIndex.second >= 0){
                    requestResultAttribute["result"] = "true";
                    nlohmann::json temp;
                    temp["docid"] = relativeIndex.first;
                    temp["rank"] = relativeIndex.second;
                    requestDocumentRelevance[iDocument] = {temp};
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
                requestAttributeFilling[requestName] = {requestResultAttribute, requestDocumentRelevance};
            } else {
                requestAttributeFilling[requestName] = {requestResultAttribute};
            }

            iRequest++;
        }

        nlohmann::json answersJSON;
        answersJSON["answers"] = requestAttributeFilling;
        file << answersJSON.dump(4);
    }
};