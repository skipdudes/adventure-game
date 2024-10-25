#pragma once
#include "Level.h"
#include "../Texture.h"
#include <memory>

class Exit : public Level
{
public:
	static Exit* get();

	bool enter();
	bool exit();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

private:
	static Exit gLevelExit;
	Exit();
};