#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <regex>

using namespace std;

string sendPrompt(const nlohmann::json& messages) {
    httplib::Client cli("http://localhost:8000");


    nlohmann::json requestData = {
        {"messages", messages}
    };


    auto res = cli.Post("/generate/", requestData.dump(), "application/json");

    if (res && res->status == 200) {
        return res->body;
    }
    else {
        std::cerr << "Error: " << (res ? std::to_string(res->status) : "Connection failed") << std::endl;
    }
}

void updateParametersFromResponse(string& response, float& npc_happiness, float& npc_anxiety, float& npc_hostility) {
    std::regex pattern(R"(\b(Happiness|Anxiety|Hostility):\s*([0-9]*\.?[0-9]+))");
    std::smatch match;

    float happiness = 0.0;
    float anxiety = 0.0;
    float hostility = 0.0;

    std::string::const_iterator searchStart(response.cbegin());
    while (std::regex_search(searchStart, response.cend(), match, pattern)) {
        std::string attribute = match[1];
        float value = std::stof(match[2]);

        if (attribute == "Happiness") {
            happiness = value;
        }
        else if (attribute == "Anxiety") {
            anxiety = value;
        }
        else if (attribute == "Hostility") {
            hostility = value;
        }

        searchStart = match.suffix().first;
    }

    //std::regex params_pattern(R"(\s*\(Happiness:\s*[0-9]*\.?[0-9]+,\s*Anxiety:\s*[0-9]*\.?[0-9]+,\s*Hostility:\s*[0-9]*\.?[0-9]+\)\s*)");
    //response = std::regex_replace(response, params_pattern, "");

    npc_happiness = happiness;
    npc_anxiety = anxiety;
    npc_hostility = hostility;
}

string paramsToString(float happiness, float anxiety, float hostility) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << happiness << ", " << anxiety << ", " << hostility;

    return oss.str();
}



int main() {


    struct NPC {
        string base_context;
        float happiness;
        float anxiety;
        float hostility;
        nlohmann::json messages = nlohmann::json::array();
    };

    NPC king;

    king.base_context = "You are a guard in a medival city. Please try to answer shortly, maximum 50 words. Do not use any special characters at the beginning and the end of the response. At the end of your response, you need to set some parameters based on the conversation, the parameters are float with value ranging from 0 to 1. parameters are: Happiness, Anxiety, Hostility. You need to include all of them in every response and you should behave accordingly to their value. The structure of response: (Happiness: 0.0, Anxiety: 0.0, Hostility: 0.0). Starting values are: ";

    king.happiness = 0.95f;
    king.anxiety = 0.1f;
    king.hostility = 0.1f;

    string parameters = paramsToString(king.happiness, king.anxiety, king.hostility);

    string prompt;
    string context = king.base_context + parameters;

    nlohmann::json systemMessage = {
        {"role", "system"},
        {"content", context}
    };
    king.messages.push_back(systemMessage);

    while (true) {
        getline(cin, prompt);
        if (prompt == "q") break;

        nlohmann::json userMessage = {
            {"role", "user"},
            {"content", prompt}
        };
        king.messages.push_back(userMessage);

        string response = sendPrompt(king.messages);

        if (!response.empty() && response.front() == '"' && response.back() == '"') {
            response = response.substr(1, response.length() - 2);
        }

        if (!response.empty() && response.front() == '\\' && response.back() == '"') {
            response = response.substr(2, response.length() - 4);
        }

        updateParametersFromResponse(response, king.happiness, king.anxiety, king.hostility);

        parameters = paramsToString(king.happiness, king.anxiety, king.hostility);

        king.messages[0]["content"] = king.base_context + parameters;

        cout << response << endl;


        nlohmann::json assistantMessage = {
            {"role", "assistant"},
            {"content", response}
        };
        king.messages.push_back(assistantMessage);

    }



    return 0;
}
