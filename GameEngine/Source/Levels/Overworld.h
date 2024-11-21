#pragma once
#include "Level.h"
#include "../Texture.h"
#include "../NPC/NPC.h"
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

	//Public, so UpdateBools() can access it
	bool mHouseWallInserted;
	std::vector<SDL_Rect> mWalls;

private:
	const static int LEVEL_WIDTH = 1536;
	const static int LEVEL_HEIGHT = 1152;

	SDL_Rect mTriggerCastle;
	SDL_Rect mTriggerInn;
	SDL_Rect mTriggerHouse;
	
	std::vector<std::shared_ptr<NPC>> mNPCs;

	static Overworld gLevelOverworld;
	Overworld();
	std::unique_ptr<Texture> mLevelTexture;
};