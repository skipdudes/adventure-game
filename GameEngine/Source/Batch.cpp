#include "Batch.h"
#include "Logger.h"

void runBatchScript(const std::string& scriptPath) {

	int result = system(scriptPath.c_str());

	if (result != 0) {
		LOG_ERROR("Failed to launch the script " + scriptPath);
	}
	else {
		LOG_INFO("Script launched successfully " + scriptPath);
	}
		
}