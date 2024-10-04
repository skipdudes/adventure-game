#include <memory>
#include "Source/Game.h"

int main(int argc, char* args[])
{
	std::unique_ptr<Game> adventureGame = std::make_unique<Game>("Shadows of the Crown", 640, 480);

	return 0;
}