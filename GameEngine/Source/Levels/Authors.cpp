#include "Authors.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Menu.h"

Authors Authors::gLevelAuthors;

Authors::Authors()
{
	mLevelTexture = std::make_unique<Texture>();
}

Authors* Authors::get()
{
	return &gLevelAuthors;
}

bool Authors::enter()
{
	//Load background
	if (!mLevelTexture->load(FILE_AUTHORS_TEXTURE.string()))
	{
		LOG_ERROR("Could not load authors level texture");
		return false;
	}

	LOG_INFO("Successfully entered authors level");
	return true;
}

bool Authors::exit()
{
	//Free allocated memory
	mLevelTexture->free();

	LOG_INFO("Exiting authors level");
	return true;
}

void Authors::handleEvents(SDL_Event& e)
{
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_CONFIRM))
	{
		setNextState(Menu::get());
	}
}

void Authors::update()
{

}

void Authors::render()
{
	mLevelTexture->render(0, 0);

	//Render header
	gFontLarge->setColor(0x00, 0x00, 0x00);
	gFontLarge->renderText(272, 146, AUTHORS);

	//Render authors
	gFontMedium->setColor(0x00, 0x00, 0x00);
	gFontMedium->renderText(246, 197, AUTHOR_MARCIN);
	gFontMedium->renderText(226, 223, AUTHOR_ANDRZEJ);
	gFontMedium->renderText(234, 249, AUTHOR_MARTA);

	//Render prompt
	gFontMedium->renderText(192, 310, OPTION_BACK);

	//Render version
	gFontSmall->setColor(0x00, 0x00, 0x00);
	gFontSmall->renderText(538, 466, VERSION);
}