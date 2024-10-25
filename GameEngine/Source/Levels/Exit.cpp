#include "Exit.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"

Exit Exit::gLevelExit;

Exit::Exit()
{

}

Exit* Exit::get()
{
	return &gLevelExit;
}

bool Exit::enter()
{
	LOG_INFO("Successfully entered exit level");
	return true;
}

bool Exit::exit()
{
	return true;
}

void Exit::handleEvents(SDL_Event& e)
{

}

void Exit::update()
{

}

void Exit::render()
{

}