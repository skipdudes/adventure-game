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
	mTriggerOverworld = { 432, 718, 96, 2 };

	//Walls
	mWalls.push_back(SDL_Rect{ 0, 48, 48, 624 }); //left
	mWalls.push_back(SDL_Rect{ 912, 48, 48, 624 }); //right
	mWalls.push_back(SDL_Rect{ 0, 672, 432, 48 }); //bottom left
	mWalls.push_back(SDL_Rect{ 528, 672, 432, 48 }); //bottom right

	//Top furniture
	mWalls.push_back(SDL_Rect{ 144, 0, 48, 48 });
	mWalls.push_back(SDL_Rect{ 240, 0, 48, 48 });
	mWalls.push_back(SDL_Rect{ 499, 0, 106, 48 }); //fireplace
	mWalls.push_back(SDL_Rect{ 672, 0, 48, 48 });
	mWalls.push_back(SDL_Rect{ 768, 0, 48, 48 });

	//Piano
	mWalls.push_back(SDL_Rect{ 167, 192, 97, 48 });

	//Desk
	mWalls.push_back(SDL_Rect{ 672, 192, 96, 2 });
	mWalls.push_back(SDL_Rect{ 703, 158, 34, 34 }); //armchair

	//Right furniture
	mWalls.push_back(SDL_Rect{ 876, 96, 36, 96 });

	//Table
	mWalls.push_back(SDL_Rect{ 624, 384, 96, 96 });
}

House* House::get()
{
	return &gLevelHouse;
}

bool House::enter()
{
	//If player just finished innkeeper quest and still haven't been to Marquis' house
	if (g_RDF_innkeeperToldAboutWoman == true && g_additional_playerEnteredMarquisHouse == false) 
	{
		g_additional_playerEnteredMarquisHouse = true;
		//************ TODO ************
		// ustaw opis questa
		// Get Marquis to talk about the woman. Find out why Richard was seeing her. (Hostility >= 70%).
		LOG_INFO("Quest set to: Get Marquis to talk about the woman. Find out why Richard was seeing her. (Hostility >= 70%).");
	}

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
	gMarquis->setPosition(535, 163);

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
				gPlayer->mIsTalking = true;

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
				//Release the player
				gPlayer->mIsTalking = false;

				//Dialogue
				npc->mDialogue.reset();

				LOG_INFO("Player ended dialogue with " + npc->getName());
			}
		}
	}

	//Toggle rendering current quest
	toggleQuestPrompt(e);
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
	gMarquis->renderDialoguePrompt(108);

	//NPCs Dialogue
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->renderDialogue();

	//Quest
	renderCurrentQuest();
}