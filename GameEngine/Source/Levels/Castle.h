#pragma once
#include "Level.h"
#include "../Texture.h"
#include <memory>
#include <vector>

class Castle : public Level
{
public:
	static Castle* get();

	bool enter();
	bool exit();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

private:
	const static int LEVEL_WIDTH = 960;
	const static int LEVEL_HEIGHT = 720;

	SDL_Rect mTriggerOverworld;
	std::vector<SDL_Rect> mWalls;

	static Castle gLevelCastle;
	Castle();
	std::unique_ptr<Texture> mLevelTexture;
};