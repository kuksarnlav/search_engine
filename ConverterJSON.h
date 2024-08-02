#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

class ConverterJSON{
public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments(){
        std::vector<std::string> textDocuments;

        std::ifstream file("config.json");
        nlohmann::json configJSON;
        file >> configJSON;

        for (int i = 0; i < configJSON["files"].size(); i++){
            std::string filePath = configJSON["files"][i];
            std::ifstream file(filePath);
            std::string line;
            if (file.is_open()){
                std::getline(file, line);
            } else{
                std::cerr << "Unable to open file" << std::endl;
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

        std::vector<nlohmann::json> answersVector;
        int pairI = 0;
        for (const auto& innerVector : answers){
            nlohmann::json requestRelevanceAttributeFilling;
            for (const auto& pair : innerVector){
                nlohmann::json requestAttributeFilling;
                nlohmann::json requestResultAttributeFilling;

                if (pair.second >= 0){
                    requestResultAttributeFilling["result"] = "true";
                    requestAttributeFilling["request" + std::to_string(pairI + 1)] = {requestResultAttributeFilling};

                    nlohmann::json temp;
                    temp["docid"] = pair.first;
                    temp["rank"] = pair.second;
                    requestRelevanceAttributeFilling[pairI] = temp;
                    requestAttributeFilling["request" + std::to_string(pairI + 1)].push_back(requestRelevanceAttributeFilling);
                } else {
                    requestResultAttributeFilling["result"] =  "false";
                    requestAttributeFilling["request" + std::to_string(pairI + 1)] = {requestResultAttributeFilling};
                }

                answersVector.push_back(requestAttributeFilling);
                pairI++;
            }
        }

        nlohmann::json answersJSON;
        answersJSON["answers"] = answersVector;
        file << answersJSON.dump(4);
    }
};