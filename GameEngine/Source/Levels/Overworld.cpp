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

	mHouseWallInserted = false;
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

	//NPCs
	//Only if after the 3rd quest and been to the castle
	if (g_RDF_marquisToldAboutWoman && g_additional_playerEnteredCastle)
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
	//Only if after the 3rd quest and been to the castle
	if (g_RDF_marquisToldAboutWoman && g_additional_playerEnteredCastle)
		gRoyalGuard->setPosition(918, 851);

	//NPCs colliders
	for (std::shared_ptr<NPC>& npc : mNPCs)
		mWalls.push_back(npc->getCollider());

	//Lock Marquis' house before convincing the Guard
	if ((g_RDF_marquisToldAboutWoman == true)
		&& (g_additional_playerEnteredCastle == true)
		&& (g_RDF_isGuardConvinced == false))
	{
		mWalls.push_back(SDL_Rect{ 864, 768, 48, 48 });
		mHouseWallInserted = true;

		LOG_INFO("<wall> Locked Marquis house (enter)");
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

	LOG_INFO("Successfully entered Overworld level");
	return true;
}

bool Overworld::exit()
{
	//Get rid of extra wall
	if (mHouseWallInserted)
	{
		mWalls.pop_back();
		mHouseWallInserted = false;

		LOG_INFO("<wall> Unlocked Marquis house (exit)");
	}

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

	LOG_INFO("Exiting Overworld level");
	return true;
}

void Overworld::handleEvents(SDL_Event& e)
{
	//Handle quit button
	handleQuitGameplay(e);

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

void Overworld::update()
{
	//Player
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

	//NPCs
	for (std::shared_ptr<NPC>& npc : mNPCs)
	{
		npc->checkIfAbleToTalk();
		npc->updateDialogue();
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

	//NPCs
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->render(camera);

	//Player
	gPlayer->render(camera);

	//Specific NPCs prompts
	//Only if after the 3rd quest and been to the castle
	if (g_RDF_marquisToldAboutWoman && g_additional_playerEnteredCastle)
		gRoyalGuard->renderDialoguePrompt(110);

	//NPCs Dialogue
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->renderDialogue();

	//Quest
	renderCurrentQuest();

	//Render quit prompt
	renderQuitGameplayPrompt();
}