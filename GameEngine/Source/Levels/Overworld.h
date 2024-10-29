#pragma once
#include "Level.h"
#include "../Texture.h"
#include <memory>
#include <vector>

class Overworld : public Level
{
public:
	static Overworld* get();

	bool enter();
	bool exit();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

private:
	const static int LEVEL_WIDTH = 960;
	const static int LEVEL_HEIGHT = 704;

	SDL_Rect mTriggerCastle;
	std::vector<SDL_Rect> mWalls;

	static Overworld gLevelOverworld;
	Overworld();
	std::unique_ptr<Texture> mLevelTexture;
};