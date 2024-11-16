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
	mWalls.push_back(SDL_Rect{ 0, 48, 24, 624 }); //left
	mWalls.push_back(SDL_Rect{ 936, 48, 24, 624 }); //right
	mWalls.push_back(SDL_Rect{ 0, 672, 360, 48 }); //bottom left
	mWalls.push_back(SDL_Rect{ 600, 672, 360, 48 }); //bottom right

	//Furniture
	mWalls.push_back(SDL_Rect{ 143, 0, 96, 48 });
	mWalls.push_back(SDL_Rect{ 312, 0, 48, 48 });
	mWalls.push_back(SDL_Rect{ 408, 0, 48, 48 });
	mWalls.push_back(SDL_Rect{ 504, 0, 48, 48 });
	mWalls.push_back(SDL_Rect{ 600, 0, 48, 48 });
	mWalls.push_back(SDL_Rect{ 719, 0, 96, 48 });

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
		LOG_ERROR("Could not load Castle level texture");
		return false;
	}

	//NPCs
	mNPCs.push_back(gKing);
	mNPCs.push_back(gFather);
	mNPCs.push_back(gRoyalGuard);

	for (std::shared_ptr<NPC>& npc : mNPCs)
	{
		//Load
		if (!npc->load())
		{
			LOG_ERROR("Could not load " + npc->getName());
			return false;
		}
	}

	//Individual NPCs position
	gKing->setPosition((LEVEL_WIDTH - gKing->getWidth()) / 2, 179);
	gFather->setPosition((LEVEL_WIDTH - gFather->getWidth()) / 3, (LEVEL_HEIGHT - gFather->getHeight()) / 2);
	gRoyalGuard->setPosition(2 * (LEVEL_WIDTH - gRoyalGuard->getWidth()) / 3, (LEVEL_HEIGHT - gRoyalGuard->getHeight()) / 2);

	//NPCs colliders
	for (std::shared_ptr<NPC>& npc : mNPCs)
		mWalls.push_back(npc->getCollider());

	//Player starting position
	gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) - 4);

	LOG_INFO("Successfully entered Castle level");
	return true;
}

bool Castle::exit()
{
	//NPCs
	for (std::shared_ptr<NPC>& npc : mNPCs)
	{
		//Free and remove collider
		npc->free();
		mWalls.pop_back();
	}
	mNPCs.clear();

	//Free level texture
	mLevelTexture->free();

	LOG_INFO("Exiting Castle level");
	return true;
}

void Castle::handleEvents(SDL_Event& e)
{
	//Player
	bool talkingTo = false;
	for (std::shared_ptr<NPC>& npc : mNPCs)
	{
		if (npc->mCurrentlyTalkingTo)
			talkingTo = true;
		break;
	}
	if (!talkingTo)
		gPlayer->handleEvents(e);

	//Dialogue
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->handleDialogueEvents(e);

	//Start dialogue
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_CONFIRM))
	{
		for (std::shared_ptr<NPC>& npc : mNPCs)
		{
			if (npc->startedDialogue())
			{
				//Stop the player
				gPlayer->stop();

				//Dialogue
				npc->mDialogue = std::make_unique<Dialogue>(npc);
				if (!(npc->mDialogue->load()))
					LOG_ERROR("Could not load dialogue variables");

				LOG_INFO("Player started dialogue with " + npc->getName());
			}
		}
	}
	//End dialogue
	else if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_QUIT))
	{
		for (std::shared_ptr<NPC>& npc : mNPCs)
		{
			if (npc->endedDialogue())
			{
				//Dialogue
				npc->mDialogue.reset();

				LOG_INFO("Player ended dialogue with " + npc->getName());
			}
		}
	}

	//Toggle rendering current quest
	toggleQuestPrompt(e);
}

void Castle::update()
{
	//Player
	gPlayer->move(LEVEL_WIDTH, LEVEL_HEIGHT, mWalls);

	//Enter overworld
	if (checkCollision(gPlayer->getCollider(), mTriggerOverworld))
		setNextState(Overworld::get());

	//NPCs
	for (std::shared_ptr<NPC>& npc : mNPCs)
	{
		npc->checkIfAbleToTalk();
		npc->updateDialogue();
	}
}

void Castle::render()
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

	//NPCs
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->render(camera);
		
	//Player
	gPlayer->render(camera);

	//Specific NPCs prompts
	gKing->renderDialoguePrompt(127);
	gFather->renderDialoguePrompt(113);
	gRoyalGuard->renderDialoguePrompt(110);
		
	//NPCs Dialogue
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->renderDialogue();

	//Quest
	renderCurrentQuest();
}