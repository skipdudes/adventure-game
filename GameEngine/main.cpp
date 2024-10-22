#include <memory>
#include <thread>
#include <cstdlib>
#include "Source/Game.h"
#include "Source/Logger.h"
#include "Source/Batch.h"

int main(int argc, char* args[])
{

	std::string scriptPath = "..\\apitest\\api_server_test\\run.bat";
	std::string scriptPathDeactivate = "..\\apitest\\api_server_test\\close.bat";
	std::thread batchThread(runBatchScript, scriptPath);

	std::unique_ptr<Game> adventureGame = std::make_unique<Game>("Shadows of the Crown", 640, 480);

	runBatchScript(scriptPathDeactivate);

	if (batchThread.joinable()) batchThread.join();
	 

	return 0;
}