#include "SendPrompt.h"
#include "Logger.h"
#include <httplib.h>

std::string sendPrompt(const nlohmann::json& messages) {
    httplib::Client cli("http://localhost:7000");


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

    //warning C4715: 'sendPrompt': not all control paths return a value
    return "";
}

std::string generateMessage(nlohmann::json& messages, std::string prompt) {
    pushUserMessage(messages, prompt);

    std::string response = sendPrompt(messages);

    if (!response.empty() && response.front() == '"' && response.back() == '"') {
        response = response.substr(1, response.length() - 2);
    }

    pushAssistantMessage(messages, response);

    return response;
}

void pushSystemMessage(nlohmann::json& messages, std::string msg) {
    nlohmann::json systemMessage = {
        {"role", "system"},
        {"content", msg}
    };

    messages.push_back(systemMessage);
}

void pushUserMessage(nlohmann::json& messages, std::string msg) {
    nlohmann::json systemMessage = {
        {"role", "user"},
        {"content", msg}
    };

    messages.push_back(systemMessage);
}

void pushAssistantMessage(nlohmann::json& messages, std::string msg) {
    nlohmann::json systemMessage = {
        {"role", "assistant"},
        {"content", msg}
    };

    messages.push_back(systemMessage);
}