#include "SendPrompt.h"
#include "Logger.h"
#include <httplib.h>

std::string sendPrompt(const nlohmann::json& messages) {
    httplib::Client cli("http://localhost:8000");


    nlohmann::json requestData = {
        {"messages", messages}
    };


    auto res = cli.Post("/generate/", requestData.dump(), "application/json");

    if (res && res->status == 200)
        return res->body;
    else 
        std::cerr << "Error: " << (res ? std::to_string(res->status) : "Connection failed") << std::endl;
    

    //warning C4715: 'sendPrompt': not all control paths return a value
    return "";
}


std::string generateMessage(nlohmann::json& messages, std::string base_context, float& happiness, float& anxiety, float& hostility, std::string prompt) {
    std::string context = base_context + paramsToString(happiness, anxiety, hostility);
    std::string response;
    std::string originalResponse;

    //if it's a first message and messages array is empty - then push system message, else - overwrite it 
    if (messages.empty()) 
        pushSystemMessage(messages, context); 
    else if (messages[0]["role"] == "system")
        messages[0]["content"] = context;
    else
        std::cerr << "Error: " << "No system role found when generating message" << std::endl;

    //Check if response if valid, generate again if needed
    do
    {
        //Generate response
        pushUserMessage(messages, prompt);
        response = sendPrompt(messages);


        //Remove language model's unnecessary characters
        if (!response.empty() && response.front() == '"' && response.back() == '"')
            response = response.substr(1, response.length() - 2);
        if (!response.empty() && response.front() == '\\' && response.back() == '"')
            response = response.substr(2, response.length() - 4);

        originalResponse = response;
    } while (!updateParametersFromResponse(response, happiness, anxiety, hostility));


    //Push model's response if valid
    pushAssistantMessage(messages, originalResponse);
    
    return response;
}

bool updateParametersFromResponse(std::string& response, float& npc_happiness, float& npc_anxiety, float& npc_hostility) {
    std::regex pattern(R"(\b(Happiness|Anxiety|Hostility):\s*([0-9]*\.?[0-9]+))");
    std::smatch match;

    float happiness = 0.0;
    float anxiety = 0.0;
    float hostility = 0.0;

    //Get parameters from string
    std::string::const_iterator searchStart(response.cbegin());
    while (std::regex_search(searchStart, response.cend(), match, pattern)) 
    {
        std::string attribute = match[1];
        float value = std::stof(match[2]);

        if (attribute == "Happiness")
            happiness = value;
        else if (attribute == "Anxiety")
            anxiety = value;
        else if (attribute == "Hostility")
            hostility = value;

        searchStart = match.suffix().first;
    }

    //Check if parameters are valid
    if (happiness == 0 && anxiety == 0 && hostility == 0)
    {
        LOG_INFO("Generated response is invalid. Regenerating...");
        return false;
    }

    //Log full message with stats
    LOG_INFO("Generated response: " + response);


    //Stats
    std::regex params_pattern(R"(\s*\(Happiness:\s*[0-9]*\.?[0-9]+,\s*Anxiety:\s*[0-9]*\.?[0-9]+,\s*Hostility:\s*[0-9]*\.?[0-9]+\)\s*)");
    response = std::regex_replace(response, params_pattern, "");

    npc_happiness = happiness;
    npc_anxiety = anxiety;
    npc_hostility = hostility;

    return true;
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

std::string paramsToString(float happiness, float anxiety, float hostility) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << happiness << ", " << anxiety << ", " << hostility;

    return oss.str();
}