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
	mTriggerOverworld = { 432, 718, 96, 2 };

	//Walls
	mWalls.push_back(SDL_Rect{ 0, 48, 48, 624 }); //left
	mWalls.push_back(SDL_Rect{ 912, 48, 48, 624 }); //right
	mWalls.push_back(SDL_Rect{ 0, 672, 432, 48 }); //bottom left
	mWalls.push_back(SDL_Rect{ 528, 672, 432, 48 }); //bottom right

	//Bar
	mWalls.push_back(SDL_Rect{ 48, 0, 432, 336 });

	//Holes
	mWalls.push_back(SDL_Rect{ 528, 48, 48, 2 });
	mWalls.push_back(SDL_Rect{ 816, 432, 48, 2 });

	//Fireplace
	mWalls.push_back(SDL_Rect{ 720, 48, 48, 48 });

	//Tables 1
	mWalls.push_back(SDL_Rect{ 107, 488, 26, 2 });
	mWalls.push_back(SDL_Rect{ 96, 528, 48, 2 });
	mWalls.push_back(SDL_Rect{ 107, 584, 26, 2 });

	//Tables 2
	mWalls.push_back(SDL_Rect{ 299, 488, 26, 2 });
	mWalls.push_back(SDL_Rect{ 288, 528, 48, 2 });
	mWalls.push_back(SDL_Rect{ 299, 584, 26, 2 });

	//Tables 3
	mWalls.push_back(SDL_Rect{ 731, 488, 26, 2 });
	mWalls.push_back(SDL_Rect{ 720, 528, 48, 2 });
	mWalls.push_back(SDL_Rect{ 731, 584, 26, 2 });

	//Tables 4
	mWalls.push_back(SDL_Rect{ 635, 344, 26, 2 });
	mWalls.push_back(SDL_Rect{ 672, 336, 48, 2 });
	mWalls.push_back(SDL_Rect{ 731, 344, 26, 2 });

	//Tables 5
	mWalls.push_back(SDL_Rect{ 635, 200, 26, 2 });
	mWalls.push_back(SDL_Rect{ 672, 192, 48, 2 });
	mWalls.push_back(SDL_Rect{ 731, 200, 26, 2 });
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
		LOG_ERROR("Could not load Inn level texture");
		return false;
	}

	//NPCs
	mNPCs.push_back(gInnkeeper);

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
	gInnkeeper->setPosition(343, 261);

	//NPCs colliders
	for (std::shared_ptr<NPC>& npc : mNPCs)
		mWalls.push_back(npc->getCollider());

	//Player starting position
	gPlayer->setPosition((LEVEL_WIDTH - Player::PLAYER_WIDTH) / 2, (LEVEL_HEIGHT - Player::PLAYER_HEIGHT) - 4);

	LOG_INFO("Successfully entered Inn level");
	return true;
}

bool Inn::exit()
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

	LOG_INFO("Exiting Inn level");
	return true;
}

void Inn::handleEvents(SDL_Event& e)
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

void Inn::update()
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

void Inn::render()
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
	gInnkeeper->renderDialoguePrompt(98);

	//NPCs Dialogue
	for (std::shared_ptr<NPC>& npc : mNPCs)
		npc->renderDialogue();

	//Quest
	renderCurrentQuest();
}