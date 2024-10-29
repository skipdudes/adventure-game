#include "Castle.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Overworld.h"

Castle Castle::gLevelCastle;

Castle::Castle()
{
	mLevelTexture = std::make_unique<Texture>();

	//Trigger
	mTriggerOverworld = { 256, 478, 128, 2 };

	//Walls
	mWalls.push_back(SDL_Rect{ 0, 0, 32, 480 });
	mWalls.push_back(SDL_Rect{ 608, 0, 32, 480 });
	mWalls.push_back(SDL_Rect{ 32, 384, 224, 96 });
	mWalls.push_back(SDL_Rect{ 384, 384, 224, 96 });
	mWalls.push_back(SDL_Rect{ 32, 0, 576, 32 });
}

Castle* Castle::get()
{
	return &gLevelCastle;
}

bool Castle::enter()
{
	//Load background
	if (!mLevelTexture->load(FILE_CASTLE_TEXTURE.string()))
	{
		LOG_ERROR("Could not load castle level texture");
		return false;
	}

	//Load King
	if (!gKing->load(FILE_KING_TEXTURE.string()))
	{
		LOG_ERROR("Could not load the king");
		return false;
	}

	gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) - 4);

	gKing->setPosition((LEVEL_WIDTH - gKing->getWidth()) / 2, 32);
	mWalls.push_back(gKing->getCollider());

	LOG_INFO("Successfully entered castle level");
	return true;
}

bool Castle::exit()
{
	//Free allocated memory
	mLevelTexture->free();
	gKing->free();
	mWalls.pop_back();

	LOG_INFO("Exiting Castle level");
	return true;
}

void Castle::handleEvents(SDL_Event& e)
{
	gPlayer->handleEvents(e);

	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_CONFIRM))
	{
		if (gKing->mAbleToTalk)
			LOG_INFO("Player wants to speak with the King!");
	}
}

void Castle::update()
{
	gPlayer->move(LEVEL_WIDTH, LEVEL_HEIGHT, mWalls);

	//Enter overworld
	if (checkCollision(gPlayer->getCollider(), mTriggerOverworld))
		setNextState(Overworld::get());

	//King dialogue
	if (checkCollision(gPlayer->getCollider(), gKing->getDialogueCollider()))
		gKing->mAbleToTalk = true;
	else
		gKing->mAbleToTalk = false;
}

void Castle::render()
{
	SDL_Rect camera = { 0, 0, LEVEL_WIDTH, LEVEL_HEIGHT };

	//Background
	mLevelTexture->render(0, 0, &camera);

	gKing->render(camera);

	//Objects
	gPlayer->render(camera);
}