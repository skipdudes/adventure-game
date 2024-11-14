#pragma once
#include "Level.h"

class Outro : public Level
{
public:
	static Outro* get();

	bool enter();
	bool exit();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

private:
	static Outro gLevelOutro;
	Outro();
};