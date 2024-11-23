#include "Outro.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Menu.h"

Outro Outro::gLevelOutro;

Outro::Outro()
{

}

Outro* Outro::get()
{
	return &gLevelOutro;
}

bool Outro::enter()
{
	//Stop the player
	gPlayer->stop();

	LOG_INFO("Successfully entered Outro level");
	return true;
}

bool Outro::exit()
{
	LOG_INFO("Exiting Outro level");
	return true;
}

void Outro::handleEvents(SDL_Event& e)
{
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_CONFIRM))
	{
		setNextState(Menu::get());
	}
}

void Outro::update()
{

}

void Outro::render()
{
	int padding = 16;
	gFontMedium->setColor(0xFF, 0xFF, 0xFF);

	//Depending on the side taken
	if (g_final_playerSidedWithInnkeeper)
	{
		//Header
		gFontLarge->setColor(0xFF, 0x00, 0x00);
		gFontLarge->renderText(150, 55, STRING_OUTRO_A_HEADER);

		//Text
		gFontMedium->renderDialogueText(padding, 100, STRING_OUTRO_A_TEXT, SCREEN_WIDTH - padding);
	}
	else if (g_final_playerSidedWithKing)
	{
		//Header
		gFontLarge->setColor(0x00, 0xFF, 0x00);
		gFontLarge->renderText(170, 55, STRING_OUTRO_B_HEADER);

		//Text
		gFontMedium->renderDialogueText(padding, 100, STRING_OUTRO_B_TEXT, SCREEN_WIDTH - padding);
	}

	//Prompt
	renderPrompt(189, NEXT_LEVEL_PROMPT, 1, 0xFF, 0xFF, 0xFF);
}