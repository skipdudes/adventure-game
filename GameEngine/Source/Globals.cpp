#include "Globals.h"
#include "Levels/Exit.h"
#include "Constants.h"
#include "Logger.h"

SDL_Renderer* gRenderer = nullptr;

std::unique_ptr<Font> gFontSmall;
std::unique_ptr<Font> gFontMedium;
std::unique_ptr<Font> gFontLarge;

std::unique_ptr<Player> gPlayer;

std::shared_ptr<NPC> gKing;
std::shared_ptr<NPC> gFather;
std::shared_ptr<NPC> gRoyalGuard;
std::shared_ptr<NPC> gInnkeeper;
std::shared_ptr<NPC> gMarquis;

Level* gCurrentLevel = nullptr;
Level* gNextLevel = nullptr;

bool gDisplayQuestPrompt = false;
const std::string* gCurrentQuestPrompt = nullptr;

//Game events
bool g_RDF_isInvestigationAssigned = false;
bool g_RDF_innkeeperToldAboutWoman = false;
bool g_RDF_marquisToldAboutWoman = false;
bool g_RDF_fatherToldAboutWoman = false;
bool g_RDF_isGuardConvinced = false;
bool g_RDF_marquisToldAboutInnkeeper = false;
bool g_RDF_innkeeperToldAboutRebels = false;
bool g_RDF_joinedTheRebellion = false;

bool g_additional_playerEnteredMarquisHouse = false;
bool g_additional_playerEnteredCastle = false;

void setNextState(Level* nextLevel)
{
    if (gNextLevel != Exit::get())
    {
        gNextLevel = nextLevel;
    }
}

void changeState()
{
    if (gNextLevel != nullptr)
    {
        gCurrentLevel->exit();
        gNextLevel->enter();

        gCurrentLevel = gNextLevel;
        gNextLevel = nullptr;
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if (bottomA <= topB)
		return false;
	if (topA >= bottomB)
		return false;
	if (rightA <= leftB)
		return false;
	if (leftA >= rightB)
		return false;

	return true;
}

void renderPrompt(int xOffset, std::string text, int linesNumber, Uint8 r, Uint8 g, Uint8 b)
{
	int padding = 2;
	int content = 32 * linesNumber;

	//Outer box
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect outerBox = { 0, SCREEN_HEIGHT - (2 * padding + content), SCREEN_WIDTH, (2 * padding + content) };
	SDL_RenderFillRect(gRenderer, &outerBox);

	//Inner box
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect innerBox = { padding, SCREEN_HEIGHT - (padding + content), SCREEN_WIDTH - (2 * padding), content };
	SDL_RenderFillRect(gRenderer, &innerBox);

	//Prompt
	gFontMedium->setColor(r, g, b);
	gFontMedium->renderText(xOffset, SCREEN_HEIGHT - (content - 2 * padding), text);
}

void renderQuestPrompt(std::string text)
{
	int padding = 2;

	//Store previous buffer address
	SDL_Texture* previousBuffer = SDL_GetRenderTarget(gRenderer);

	//Get lines number
	SDL_Texture* textTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (textTexture == nullptr)
	{
		LOG_WARNING("Text buffer texture could not be created, SDL Error: " + std::string(SDL_GetError()));
		return;
	}
	SDL_SetRenderTarget(gRenderer, textTexture);
	int linesNumber = gFontSmall->renderDialogueText(4 * padding, 4 * padding, text, SCREEN_WIDTH - (4 * padding));
	SDL_SetRenderTarget(gRenderer, previousBuffer);

	int content = 24 * linesNumber;

	//Outer box
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect outerBox = { 0, 0, SCREEN_WIDTH, (2 * padding + content) };
	SDL_RenderFillRect(gRenderer, &outerBox);

	//Inner box
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect innerBox = { padding, padding, SCREEN_WIDTH - (2 * padding), content };
	SDL_RenderFillRect(gRenderer, &innerBox);

	//Prompt
	gFontSmall->setColor(0xFF, 0xFF, 0xFF);
	gFontSmall->renderDialogueText(4 * padding, 4 * padding, text, SCREEN_WIDTH - (4 * padding));

	//Dealloc memory
	SDL_DestroyTexture(textTexture);
	textTexture = nullptr;
}

void renderCurrentQuest()
{
	if (gDisplayQuestPrompt)
	{
		renderQuestPrompt(*gCurrentQuestPrompt);
	}
}

void toggleQuestPrompt(SDL_Event& e)
{
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_INFO))
	{
		gDisplayQuestPrompt = !gDisplayQuestPrompt;

		if (gDisplayQuestPrompt)
			LOG_INFO("Display current quest prompt: ON");
		else
			LOG_INFO("Display current quest prompt: OFF");
	}
}