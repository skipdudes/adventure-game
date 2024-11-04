#include "Castle.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Overworld.h"

Castle Castle::gLevelCastle;

Castle::Castle()
{
	mLevelTexture = std::make_unique<Texture>();

	//Trigger to Overworld
	mTriggerOverworld = { 256, 478, 128, 2 };

	//Walls (castle walls)
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

	//Player starting position
	gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) - 4);

	//King
	gKing->setPosition((LEVEL_WIDTH - gKing->getWidth()) / 2, 32);
	mWalls.push_back(gKing->getCollider());

	LOG_INFO("Successfully entered castle level");
	return true;
}

bool Castle::exit()
{
	//Free allocated memory
	mLevelTexture->free();

	//King
	gKing->free();
	mWalls.pop_back();

	LOG_INFO("Exiting Castle level");
	return true;
}

void Castle::handleEvents(SDL_Event& e)
{
	//Player
	if (!gKing->mCurrentlyTalkingTo)
		gPlayer->handleEvents(e);

	//Dialogue
	if (gKing->mCurrentlyTalkingTo && !(gKing->mDialogue == nullptr))
		gKing->mDialogue->handleEvents(e);

	//King
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_CONFIRM))
	{
		if (gKing->mAbleToTalk && !gKing->mCurrentlyTalkingTo)
		{
			LOG_INFO("Player started dialogue with The King");

			//Player
			gPlayer->stop();

			//King
			gKing->mCurrentlyTalkingTo = true;
			gKing->mRecentlyTalkedTo = true;

			//Dialogue
			gKing->mDialogue = std::make_unique<Dialogue>(gKing->getName(), gKing->getDialogueTexturePath());
			if (!(gKing->mDialogue->load()))
				LOG_ERROR("Could not load Dialogue variables");
		}
	}
	else if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_QUIT))
	{
		if (gKing->mAbleToTalk && gKing->mCurrentlyTalkingTo)
		{
			LOG_INFO("Player ended dialogue with The King");

			//King
			gKing->mCurrentlyTalkingTo = false;

			//Dialogue
			gKing->mDialogue.reset();
		}
	}
}

void Castle::update()
{
	//Player
	gPlayer->move(LEVEL_WIDTH, LEVEL_HEIGHT, mWalls);

	//Enter overworld
	if (checkCollision(gPlayer->getCollider(), mTriggerOverworld))
		setNextState(Overworld::get());

	//King
	if (checkCollision(gPlayer->getCollider(), gKing->getDialogueCollider()))
		gKing->mAbleToTalk = true;
	else
	{
		gKing->mAbleToTalk = false;
		gKing->mRecentlyTalkedTo = false;
	}

	//Dialogue
	if (gKing->mCurrentlyTalkingTo && !(gKing->mDialogue == nullptr))
		gKing->mDialogue->update();
}

void Castle::render()
{
	SDL_Rect camera = { 0, 0, LEVEL_WIDTH, LEVEL_HEIGHT };

	//Background
	mLevelTexture->render(0, 0, &camera);

	//King
	gKing->render(camera);
		
	//Player
	gPlayer->render(camera);

	//Prompts
	if (gKing->mAbleToTalk && !gKing->mCurrentlyTalkingTo && !gKing->mRecentlyTalkedTo)
		renderPrompt(127, BEGIN_DIALOGUE_PROMPT + STRING_KING_NAME);
	
	//Dialogue
	if (gKing->mCurrentlyTalkingTo && !(gKing->mDialogue == nullptr))
		gKing->mDialogue->render(camera);
}