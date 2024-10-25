#include "Overworld.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"

Overworld Overworld::gLevelOverworld;

Overworld::Overworld()
{
	mLevelTexture = std::make_unique<Texture>();
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

	gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) / 2);

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
	gPlayer->move(LEVEL_WIDTH, LEVEL_HEIGHT);
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