#include "House.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "Overworld.h"

House House::gLevelHouse;

House::House()
{
	mLevelTexture = std::make_unique<Texture>();

	//Trigger to Overworld
	mTriggerOverworld = { 256, 478, 128, 2 };

	//Temp walls
	mWalls.push_back(SDL_Rect{ 0, 0, 640, 32 }); //top
	mWalls.push_back(SDL_Rect{ 0, 32, 32, 352 }); //left
	mWalls.push_back(SDL_Rect{ 608, 32, 32, 352 }); //right
	mWalls.push_back(SDL_Rect{ 0, 384, 256, 96 }); //bottom left
	mWalls.push_back(SDL_Rect{ 384, 384, 256, 96 }); //bottom right
}

House* House::get()
{
	return &gLevelHouse;
}

bool House::enter()
{
	//Load background
	if (!mLevelTexture->load(FILE_HOUSE_TEXTURE.string()))
	{
		LOG_ERROR("Could not load House level texture");
		return false;
	}

	//NPCs
	mNPCs.push_back(gMarquis);

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
	gMarquis->setPosition((LEVEL_WIDTH - gMarquis->getWidth()) / 2, 64);

	//NPCs colliders
	for (std::shared_ptr<NPC>& npc : mNPCs)
		mWalls.push_back(npc->getCollider());

	//Player starting position
	gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) - 4);

	LOG_INFO("Successfully entered House level");
	return true;
}

bool House::exit()
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

	LOG_INFO("Exiting House level");
	return true;
}

void House::handleEvents(SDL_Event& e)
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
}

void House::update()
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

void House::render()
{
	//Static camera
	SDL_Rect camera = { 0, 0, LEVEL_WIDTH, LEVEL_HEIGHT };

	//Background
	mLevelTexture->render(0, 0, &camera);

	//NPCs
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->render(camera);

	//Player
	gPlayer->render(camera);

	//Specific NPCs prompts
	gMarquis->renderDialoguePrompt(108);

	//NPCs Dialogue
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->renderDialogue();
}