#include "Intro.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Overworld.h"

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
		setNextState(Overworld::get());
	}
}

void Intro::update()
{

}

void Intro::render()
{
	std::string temp = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam in lorem sapien. Nunc sit amet diam sed enim malesuada aliquet non quis enim. Nam varius ac velit sed lacinia. Sed id condimentum turpis, nec iaculis justo. Aenean porttitor hendrerit odio, scelerisque tempor mi tincidunt ut. Proin commodo vitae justo ut molestie. Etiam imperdiet tortor eget ex dictum, nec finibus sem gravida. Sed sed lectus id magna ultricies auctor sed et dui. Vestibulum bibendum quis nisl vel eleifend.";
	int padding = 16;

	gFontMedium->setColor(0xFF, 0xFF, 0xFF);
	gFontMedium->renderDialogueText(padding, 100, temp, SCREEN_WIDTH - padding);

	//Prompt
	renderPrompt(189, NEXT_LEVEL_PROMPT, 1, 0xFF, 0xFF, 0xFF);
}