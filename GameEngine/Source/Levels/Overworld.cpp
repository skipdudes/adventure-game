#include "Overworld.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Castle.h"

Overworld Overworld::gLevelOverworld;

Overworld::Overworld()
{
	mLevelTexture = std::make_unique<Texture>();

	//Trigger
	mTriggerCastle = { 768, 96, 64, 32 };

	//Castle
	mWalls.push_back(SDL_Rect{ 704, 0, 64, 128 });
	mWalls.push_back(SDL_Rect{ 768, 0, 64, 96 });
	mWalls.push_back(SDL_Rect{ 832, 0, 64, 128 });

	//Trees
	mWalls.push_back(SDL_Rect{ 480, 96, 32, 96 }); 
	mWalls.push_back(SDL_Rect{ 128, 160, 32, 96 }); 
	mWalls.push_back(SDL_Rect{ 128, 512, 32, 96 }); 
	mWalls.push_back(SDL_Rect{ 512, 576, 32, 96 }); 
	mWalls.push_back(SDL_Rect{ 832, 544, 32, 96 }); 
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
		LOG_ERROR("Could not load overworld level texture");
		return false;
	}

	//Came from Castle
	if (gCurrentLevel == Castle::get())
	{
		gPlayer->setPosition(782, 128);
	}
	else
	{
		gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) / 2);
	}

	LOG_INFO("Successfully entered overworld level");
	return true;
}

bool Overworld::exit()
{
	//Free allocated memory
	mLevelTexture->free();

	LOG_INFO("Exiting overworld level");
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
}

void Overworld::render()
{
	SDL_Rect camera =
	{
		(gPlayer->getCollider().x + Player::PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2,
		(gPlayer->getCollider().y + Player::PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	};
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}

	//Background
	mLevelTexture->render(0, 0, &camera);

	//Objects
	gPlayer->render(camera);
}