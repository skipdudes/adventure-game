#include "Menu.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Intro.h"
#include "Authors.h"
#include "Exit.h"
#include <algorithm>

Menu Menu::gLevelMenu;

Menu::Menu()
	:mChosenOption(START)
{
	mLevelTexture = std::make_unique<Texture>();

	mOptions[START].text = OPTION_START;
	mOptions[AUTHORS].text = OPTION_AUTHORS;
	mOptions[QUIT].text = OPTION_QUIT;

	mOptions[mChosenOption].chosen = true;
}

Menu* Menu::get()
{
	return &gLevelMenu;
}

bool Menu::enter()
{
	//Load background
	if (!mLevelTexture->load(FILE_MENU_TEXTURE.string()))
	{
		LOG_ERROR("Could not load menu level texture");
		return false;
	}

	LOG_INFO("Successfully entered menu level");
	return true;
}

bool Menu::exit()
{
	//Free allocated memory
	mLevelTexture->free();

	LOG_INFO("Exiting menu level");
	return true;
}

void Menu::handleEvents(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case BUTTON_MOVE_UP:
			mOptions[mChosenOption].chosen = false;
			mChosenOption = std::max(0, mChosenOption - 1);
			mOptions[mChosenOption].chosen = true;
			break;

		case BUTTON_MOVE_DOWN:
			mOptions[mChosenOption].chosen = false;
			mChosenOption = std::min(TOTAL_OPTIONS - 1, mChosenOption + 1);
			mOptions[mChosenOption].chosen = true;
			break;

		case BUTTON_CONFIRM:
			switch (mChosenOption)
			{
			case START: setNextState(Intro::get()); break;
			case AUTHORS: setNextState(Authors::get()); break;
			case QUIT: setNextState(Exit::get()); break;
			}
			break;
		}
	}
}

void Menu::update()
{

}

void Menu::render()
{
	mLevelTexture->render(0, 0);

	//Render title
	gFontLarge->setColor(0x00, 0x00, 0x00);
	gFontLarge->renderText(175, 156, TITLE);

	//Render each option
	renderOption(START, 263, 237);
	renderOption(AUTHORS, 281, 263);
	renderOption(QUIT, 272, 289);
}

void Menu::renderOption(int index, int x, int y)
{
	if (mOptions[index].chosen)
		gFontMedium->setColor(0xFF, 0xFF, 0xFF);
	else
		gFontMedium->setColor(0x00, 0x00, 0x00);

	gFontMedium->renderText(x, y, mOptions[index].text);
}