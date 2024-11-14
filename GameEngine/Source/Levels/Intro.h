#pragma once
#include "Level.h"

class Intro : public Level
{
public:
	static Intro* get();

	bool enter();
	bool exit();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

private:
	static Intro gLevelIntro;
	Intro();
};