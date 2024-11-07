#pragma once
#include <string>
#include <nlohmann/json.hpp>

std::string sendPrompt(const nlohmann::json& messages);
std::string generateMessage(nlohmann::json& messages, std::string prompt);
void pushSystemMessage(nlohmann::json& messages, std::string msg);
void pushUserMessage(nlohmann::json& messages, std::string msg);
void pushAssistantMessage(nlohmann::json& messages, std::string msg);
