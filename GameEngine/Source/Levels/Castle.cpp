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
	mTriggerOverworld = { 360, 718, 240, 2 };

	//Castle walls
	mWalls.push_back(SDL_Rect{ 0, 0, 960, 48 }); //top
	mWalls.push_back(SDL_Rect{ 0, 48, 24, 624 }); //left
	mWalls.push_back(SDL_Rect{ 936, 48, 24, 624 }); //right
	mWalls.push_back(SDL_Rect{ 0, 672, 360, 48 }); //bottom left
	mWalls.push_back(SDL_Rect{ 600, 672, 360, 48 }); //bottom right

	//Furniture
	mWalls.push_back(SDL_Rect{ 143, 48, 96, 48 });
	mWalls.push_back(SDL_Rect{ 312, 48, 48, 48 });
	mWalls.push_back(SDL_Rect{ 600, 48, 48, 48 });
	mWalls.push_back(SDL_Rect{ 719, 48, 96, 48 });

	//Throne
	mWalls.push_back(SDL_Rect{ 436, 147, 88, 93 });
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
	if (!gKing->load())
	{
		LOG_ERROR("Could not load the king");
		return false;
	}

	//Player starting position
	gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) - 4);

	//King
	gKing->setPosition((LEVEL_WIDTH - gKing->getWidth()) / 2, 179);
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
			//Player
			gPlayer->stop();

			//King
			gKing->mCurrentlyTalkingTo = true;
			gKing->mRecentlyTalkedTo = true;

			//Dialogue
			gKing->mDialogue = std::make_unique<Dialogue>(gKing);
			if (!(gKing->mDialogue->load()))
				LOG_ERROR("Could not load Dialogue variables");

			LOG_INFO("Player started dialogue with The King");
		}
	}
	else if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_QUIT))
	{
		if (gKing->mAbleToTalk && gKing->mCurrentlyTalkingTo && !gKing->mThinking)
		{
			//King
			gKing->mCurrentlyTalkingTo = false;

			//Dialogue
			gKing->mDialogue.reset();

			LOG_INFO("Player ended dialogue with The King");
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

	//King
	gKing->render(camera);
		
	//Player
	gPlayer->render(camera);

	//Prompts
	if (gKing->mAbleToTalk && !gKing->mCurrentlyTalkingTo && !gKing->mRecentlyTalkedTo)
		renderPrompt(127, BEGIN_DIALOGUE_PROMPT + STRING_KING_NAME, 1, 0xFF, 0xFF, 0xFF);
		
	//Dialogue
	if (gKing->mCurrentlyTalkingTo && !(gKing->mDialogue == nullptr))
		gKing->mDialogue->render();
}