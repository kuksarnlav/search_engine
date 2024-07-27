#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

class ConverterJSON{
public:
    ConverterJSON() = default;
    void initAnswersJSON(){
        std::ofstream file("answers.json");

        nlohmann::json request001ResultAttributeFilling;
        request001ResultAttributeFilling["result"] = "true";
        nlohmann::json request001RelevanceAttributeFilling;
        nlohmann::json temp;
        temp["docid"] = 0;
        temp["rank"] = 0.000;
        request001RelevanceAttributeFilling[0] = temp;
        temp["docid"] = 1;
        temp["rank"] = 0.000;
        request001RelevanceAttributeFilling[1] = temp;
        nlohmann::json request001AttributeFilling;
        request001AttributeFilling["request001"] = {request001ResultAttributeFilling, request001RelevanceAttributeFilling};

        nlohmann::json request002ResultAttributeFilling;
        request002ResultAttributeFilling["result"] = "false";
        nlohmann::json request002AttributeFilling;
        request002AttributeFilling["request002"] = {request002ResultAttributeFilling};

        nlohmann::json answers;
        answers["answers"] = {request001AttributeFilling, request002AttributeFilling};
        file << answers.dump(4);
    }

    /*std::vector<std::vector<std::pair<int, float>>> getAnswers(){
                        // add functional later
        std::vector<std::string> textDocuments = GetTextDocuments();

        std::vector<std::string> requestStrokes;
        std::ifstream file("requests.json");
        nlohmann::json requestsJSON;
        file >> requestsJSON;

        for (int i = 0; i < requestsJSON["requests"].size(); i++){
            requestStrokes.push_back(requestsJSON["requests"][i]);
        }

        for (int i = 0; i < requestStrokes.size(); i++){
            std::istringstream iss(requestStrokes[i]);
            std::string word;

            while (iss >> word) {
                std::cout << word << std::endl;
            }


        }
    }*/

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

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers){ // Положить в файл answers.json результаты поисковых запросов
        //
    }
};