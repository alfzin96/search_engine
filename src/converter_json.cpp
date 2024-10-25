#include <fstream>
#include <iostream>
#include "converter_json.h"


std::vector<std::string> ConverterJSON::getTextDocuments()
{
    std::ifstream file("../config/config.json");
    if (!file.is_open()) throw ConfigFileMissing();
    nlohmann::json config;
    try
    {
        file >> config;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "config.json parsing error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (config.find("config") == config.end()) throw ConfigFileEmpty();
    std::vector<std::string> documents;
    documents.reserve(config["files"].size());
    for (const auto& filepath : config["files"])
    {
        try
        {
            std::ifstream file(filepath);
            if (!file.is_open()) throw DocFileMissing();
            std::string doc_file;
            std::getline(file, doc_file, '\0');
            documents.push_back(doc_file);
        }
        catch(const DocFileMissing& e)
        {
            std::cerr << "file " << filepath << " does not exist!\n";
        }
    }
    return documents;
}

size_t ConverterJSON::getResponsesLimit()
{
    std::ifstream file("../config/config.json");
    nlohmann::json config;
    try
    {
        file >> config;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "config.json parsing error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    auto max_responses_it = config.find("config")->find("max_responses");
    if (max_responses_it == config.find("config")->end()) return 5;
    else return max_responses_it.value();
}

std::vector<std::string> ConverterJSON::getRequests()
{
    std::ifstream file("../config/requests.json");
    if (!file.is_open()) throw RequestsFileMissing();
    nlohmann::json requests;
    try
    {
        file >> requests;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "requests.json parsing error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::vector<std::string> vecrequests;
    vecrequests.reserve(requests["requests"].size());
    for (const auto& req : requests["requests"])
    {
        vecrequests.push_back(req);
    }
    return vecrequests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers)
{
    std::ofstream file("../output/answers.json");
    nlohmann::json out;
    for (size_t i = 0; i < answers.size(); ++i)
    {
        if (answers[i].empty())
            out["answers"]["request" + std::to_string(i + 1)]["result"] = false;

        else if (answers[i].size() == 1) {
            out["answers"]["request" + std::to_string(i + 1)]["result"] = true;
            out["answers"]["request" + std::to_string(i + 1)]["docid"] = answers[i].front().doc_id;
            out["answers"]["request" + std::to_string(i + 1)]["rank"] = answers[i].front().rank;
        }  

        else for (const auto& rel_idx : answers[i])
        {
            out["answers"]["request" + std::to_string(i + 1)]["result"] = true;
            out["answers"]["request" + std::to_string(i + 1)]["relevance"].push_back({{"docid", rel_idx.doc_id}, {"rank", rel_idx.rank}});
        }
    }
    
    file << std::setw(4) << out << std::endl;
}









