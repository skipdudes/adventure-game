#include "Inn.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Overworld.h"

Inn Inn::gLevelInn;

Inn::Inn()
{
	mLevelTexture = std::make_unique<Texture>();

	//Trigger to Overworld
	mTriggerOverworld = { 0, 718, 960, 2 };

	//Walls
}

Inn* Inn::get()
{
	return &gLevelInn;
}

bool Inn::enter()
{
	//Load background
	if (!mLevelTexture->load(FILE_INN_TEXTURE.string()))
	{
		LOG_ERROR("Could not load inn level texture");
		return false;
	}

	//Load Innkeeper
	if (!gInnkeeper->load())
	{
		LOG_ERROR("Could not load the king");
		return false;
	}

	//Player starting position
	gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) - 4);

	//Innkeeper
	gInnkeeper->setPosition((LEVEL_WIDTH - gInnkeeper->getWidth()) / 2, 179);
	mWalls.push_back(gInnkeeper->getCollider());

	LOG_INFO("Successfully entered inn level");
	return true;
}

bool Inn::exit()
{
	//Free allocated memory
	mLevelTexture->free();

	//Innkeeper
	gInnkeeper->free();
	mWalls.pop_back();

	LOG_INFO("Exiting Inn level");
	return true;
}

void Inn::handleEvents(SDL_Event& e)
{
	//Player
	if (!gInnkeeper->mCurrentlyTalkingTo)
		gPlayer->handleEvents(e);

	//Dialogue
	if (gInnkeeper->mCurrentlyTalkingTo && !(gInnkeeper->mDialogue == nullptr))
		gInnkeeper->mDialogue->handleEvents(e);

	//NPC
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_CONFIRM))
	{
		if (gInnkeeper->mAbleToTalk && !gInnkeeper->mCurrentlyTalkingTo)
		{
			//Player
			gPlayer->stop();

			//NPC
			gInnkeeper->mCurrentlyTalkingTo = true;
			gInnkeeper->mRecentlyTalkedTo = true;

			//Dialogue
			gInnkeeper->mDialogue = std::make_unique<Dialogue>(gInnkeeper);
			if (!(gInnkeeper->mDialogue->load()))
				LOG_ERROR("Could not load Dialogue variables");

			LOG_INFO("Player started dialogue with The Innkeeper");
		}
	}
	else if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_QUIT))
	{
		if (gInnkeeper->mAbleToTalk && gInnkeeper->mCurrentlyTalkingTo && !gInnkeeper->mThinking)
		{
			//NPC
			gInnkeeper->mCurrentlyTalkingTo = false;

			//Dialogue
			gInnkeeper->mDialogue.reset();

			LOG_INFO("Player ended dialogue with The Innkeeper");
		}
	}
}

void Inn::update()
{
	//Player
	gPlayer->move(LEVEL_WIDTH, LEVEL_HEIGHT, mWalls);

	//Enter overworld
	if (checkCollision(gPlayer->getCollider(), mTriggerOverworld))
		setNextState(Overworld::get());

	//NPC
	if (checkCollision(gPlayer->getCollider(), gInnkeeper->getDialogueCollider()))
		gInnkeeper->mAbleToTalk = true;
	else
	{
		gInnkeeper->mAbleToTalk = false;
		gInnkeeper->mRecentlyTalkedTo = false;
	}

	//Dialogue
	if (gInnkeeper->mCurrentlyTalkingTo && !(gInnkeeper->mDialogue == nullptr))
		gInnkeeper->mDialogue->update();
}

void Inn::render()
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

	//NPC
	gInnkeeper->render(camera);

	//Player
	gPlayer->render(camera);

	//Prompts
	if (gInnkeeper->mAbleToTalk && !gInnkeeper->mCurrentlyTalkingTo && !gInnkeeper->mRecentlyTalkedTo)
		renderPrompt(127, BEGIN_DIALOGUE_PROMPT + STRING_INNKEEPER_NAME, 1, 0xFF, 0xFF, 0xFF);

	//Dialogue
	if (gInnkeeper->mCurrentlyTalkingTo && !(gInnkeeper->mDialogue == nullptr))
		gInnkeeper->mDialogue->render();
}