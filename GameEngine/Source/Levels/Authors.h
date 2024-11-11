#pragma once
#include "Level.h"
#include "../Texture.h"
#include <memory>

class Authors : public Level
{
public:
	static Authors* get();

	bool enter();
	bool exit();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

private:
	static Authors gLevelAuthors;
	Authors();
	std::unique_ptr<Texture> mLevelTexture;

	//Special char
	SDL_Rect mSpecialChar1 = { 353, 211, 2, 2 };
	SDL_Rect mSpecialChar2 = { 355, 213, 2, 2 };
};