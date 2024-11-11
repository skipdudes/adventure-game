#include "Overworld.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Castle.h"
#include "Inn.h"
#include "House.h"

Overworld Overworld::gLevelOverworld;

Overworld::Overworld()
{
	mLevelTexture = std::make_unique<Texture>();

	//Trigger
	mTriggerCastle = { 1128, 123, 48, 56 };
	mTriggerInn = { 192, 192, 48, 36 };
	mTriggerHouse = { 864, 768, 48, 36 };

	//Castle
	mWalls.push_back(SDL_Rect{ 816, 0, 312, 192 });
	mWalls.push_back(SDL_Rect{ 1128, 0, 48, 123 });
	mWalls.push_back(SDL_Rect{ 1176, 0, 312, 192 });

	//Inn
	mWalls.push_back(SDL_Rect{ 96, 0, 96, 240 });
	mWalls.push_back(SDL_Rect{ 192, 0, 48, 192 });
	mWalls.push_back(SDL_Rect{ 240, 0, 96, 240 });
	
	//House
	mWalls.push_back(SDL_Rect{ 720, 624, 144, 192 });
	mWalls.push_back(SDL_Rect{ 864, 624, 48, 144 });
	mWalls.push_back(SDL_Rect{ 912, 624, 144, 192 });
}

Overworld* Overworld::get()
{
	return &gLevelOverworld;
}

bool Overworld::enter()
{
	//Load background
	if (!mLevelTexture->load(FILE_OVERWORLD_TEXTURE.string()))
	{
		LOG_ERROR("Could not load Overworld level texture");
		return false;
	}

	//Came from Castle
	if (gCurrentLevel == Castle::get())
	{
		gPlayer->setPosition(1134, 181);
	}
	//Came from Inn
	else if (gCurrentLevel == Inn::get())
	{
		gPlayer->setPosition(197, 229);
	}
	//Came from House
	else if (gCurrentLevel == House::get())
	{
		gPlayer->setPosition(869, 805);
	}
	else
	{
		gPlayer->setPosition(533, 420);
	}

	LOG_INFO("Successfully entered Overworld level");
	return true;
}

bool Overworld::exit()
{
	//Free allocated memory
	mLevelTexture->free();

	LOG_INFO("Exiting Overworld level");
	return true;
}

void Overworld::handleEvents(SDL_Event& e)
{
	gPlayer->handleEvents(e);
}

void Overworld::update()
{
	gPlayer->move(LEVEL_WIDTH, LEVEL_HEIGHT, mWalls);

	//Enter castle
	if (checkCollision(gPlayer->getCollider(), mTriggerCastle))
	{
		setNextState(Castle::get());
	}
	//Enter Inn
	else if (checkCollision(gPlayer->getCollider(), mTriggerInn))
	{
		setNextState(Inn::get());
	}
	//Enter House
	else if (checkCollision(gPlayer->getCollider(), mTriggerHouse))
	{
		setNextState(House::get());
	}
}

void Overworld::render()
{
	//Center camera around the player
	SDL_Rect camera =
	{
		(gPlayer->getCollider().x + Player::PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2,
		(gPlayer->getCollider().y + Player::PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2,
		SCREEN_WIDTH, SCREEN_HEIGHT
	};
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > LEVEL_WIDTH - camera.w) camera.x = LEVEL_WIDTH - camera.w;
	if (camera.y > LEVEL_HEIGHT - camera.h) camera.y = LEVEL_HEIGHT - camera.h;

	//Background
	mLevelTexture->render(0, 0, &camera);

	//Objects
	gPlayer->render(camera);
}