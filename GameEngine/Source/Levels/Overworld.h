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
	const static int LEVEL_WIDTH = 1536;
	const static int LEVEL_HEIGHT = 1152;

	SDL_Rect mTriggerCastle;
	SDL_Rect mTriggerInn;
	SDL_Rect mTriggerHouse;
	std::vector<SDL_Rect> mWalls;

	static Overworld gLevelOverworld;
	Overworld();
	std::unique_ptr<Texture> mLevelTexture;
};