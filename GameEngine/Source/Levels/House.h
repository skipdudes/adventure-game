#pragma once
#include "Level.h"
#include "../Texture.h"
#include "../NPC/NPC.h"
#include <memory>
#include <vector>

class House : public Level
{
public:
	static House* get();

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
	std::vector<std::shared_ptr<NPC>> mNPCs;

	static House gLevelHouse;
	House();
	std::unique_ptr<Texture> mLevelTexture;
};