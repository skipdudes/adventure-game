#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>

using namespace std;

string sendPrompt(string user_prompt, string system_prompt, string assistant_prompt) {
    httplib::Client cli("http://localhost:8000");


    nlohmann::json requestData = {
        {"user_prompt", user_prompt},
        {"system_prompt", system_prompt},
        {"assistant_prompt", assistant_prompt}
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

    getline(cin, prompt);

    string response = sendPrompt(prompt, "you are a robot. sometimes you say beep-boop. your answer should be 15 words at most", "");
    response.erase(remove(response.begin(), response.end(), '"'), response.end());

    cout << response;

    return 0;
}
