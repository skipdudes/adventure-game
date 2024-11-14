#pragma once
#include <string>
#include <nlohmann/json.hpp>

std::string sendPrompt(const nlohmann::json& messages);
std::string generateMessage(nlohmann::json& messages, std::string base_context, float& happiness, float& trust, float& hostility, std::string prompt);
bool updateParametersFromResponse(std::string& response, float& npc_happiness, float& npc_trust, float& npc_hostility);
void pushSystemMessage(nlohmann::json& messages, std::string msg);
void pushUserMessage(nlohmann::json& messages, std::string msg);
void pushAssistantMessage(nlohmann::json& messages, std::string msg);
std::string paramsToString(float happiness, float trust, float hostility);
