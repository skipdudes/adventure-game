#include <memory>
#include <thread>
#include <cstdlib>
#include "Game.h"
#include "Batch.h"

//// ######################
//#include <string>
//#include <nlohmann/json.hpp>
//#include <iostream>
//#include "SendPrompt.h"
//using namespace std;
//// ######################

int main(int argc, char* args[])
{
	std::string scriptPath = "..\\apitest\\api_server_test\\run.bat";
	std::string scriptPathDeactivate = "..\\apitest\\api_server_test\\close.bat";
	std::thread batchThread(runBatchScript, scriptPath);

	//// ############################################################
	////						  TEST DIALOGU						

	//struct NPC {
	//	string base_context = "You are a guard in a medival city. Please try to answer shortly, maximum 50 words. Do not use any special characters at the beginning and the end of the response. At the end of your response, you need to set some parameters based on the conversation, the parameters are float with value ranging from 0 to 1. parameters are: Happiness, Anxiety, Hostility. You need to include all of them in every response and you should behave accordingly to their value. The structure of response: (Happiness: 0.0, Anxiety: 0.0, Hostility: 0.0). Starting values are: ";;
	//	float happiness = 0.95f;
	//	float anxiety = 0.1f;
	//	float hostility = 0.1f;
	//	nlohmann::json messages = nlohmann::json::array();
	//};

	//NPC current_npc;
	//string prompt;

	//while (true) {
	//	getline(cin, prompt);
	//	if (prompt == "q") break;

	//	cout << generateMessage(current_npc.messages, current_npc.base_context, current_npc.happiness, current_npc.anxiety, current_npc.hostility, prompt) << endl;
	//}








	////					  KONIEC TESTU DIALOGU
	//// ############################################################

	std::unique_ptr<Game> adventureGame = std::make_unique<Game>();

	runBatchScript(scriptPathDeactivate);

	if (batchThread.joinable())
		batchThread.join();

	return 0;
}