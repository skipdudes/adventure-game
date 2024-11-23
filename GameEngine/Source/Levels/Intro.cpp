#include "Intro.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Castle.h"

Intro Intro::gLevelIntro;

Intro::Intro()
{

}

Intro* Intro::get()
{
	return &gLevelIntro;
}

bool Intro::enter()
{
	LOG_INFO("Successfully entered Intro level");
	return true;
}

bool Intro::exit()
{
	LOG_INFO("Exiting Intro level");
	return true;
}

void Intro::handleEvents(SDL_Event& e)
{
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_CONFIRM))
	{
		setNextState(Castle::get());
	}
}

void Intro::update()
{

}

void Intro::render()
{
	//Header
	gFontLarge->setColor(0xFF, 0x00, 0x00);
	gFontLarge->renderText(272, 65, STRING_INTRO_HEADER);

	//Text
	int padding = 16;
	gFontMedium->setColor(0xFF, 0xFF, 0xFF);
	gFontMedium->renderDialogueText(padding, 120, STRING_INTRO_TEXT, SCREEN_WIDTH - padding);

	//Prompt
	renderPrompt(189, NEXT_LEVEL_PROMPT, 1, 0xFF, 0xFF, 0xFF);
}