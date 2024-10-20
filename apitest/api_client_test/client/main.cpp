#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>

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

int main() {

    string prompt;
    string context = "you are a robot. sometimes you say beep-boop. Please try to answer shortly, maximum 50 words.";
    nlohmann::json messages = nlohmann::json::array();

    nlohmann::json systemMessage = {
        {"role", "system"},
        {"content", context}
    };
    messages.push_back(systemMessage);

    while (true) {
        getline(cin, prompt);
        if (prompt == "q") break;

        nlohmann::json userMessage = {
            {"role", "user"},
            {"content", prompt}
        };
        messages.push_back(userMessage);

        string response = sendPrompt(messages);

        if (!response.empty() && response.front() == '"' && response.back() == '"') {
            response = response.substr(1, response.length() - 2);
        }

        cout << response << endl;

        nlohmann::json assistantMessage = {
            {"role", "assistant"},
            {"content", response}
        };
        messages.push_back(assistantMessage);

    }



    return 0;
}
