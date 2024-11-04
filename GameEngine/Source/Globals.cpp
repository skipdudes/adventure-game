#include "Globals.h"
#include "Levels/Exit.h"
#include "Constants.h"

SDL_Renderer* gRenderer = nullptr;
std::unique_ptr<Font> gFontSmall;
std::unique_ptr<Font> gFontMedium;
std::unique_ptr<Font> gFontLarge;
std::unique_ptr<Player> gPlayer;
std::unique_ptr<NPC> gKing;
Level* gCurrentLevel = nullptr;
Level* gNextLevel = nullptr;

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

void renderPrompt(int xOffset, std::string text, int linesNumber)
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
	gFontMedium->setColor(0xFF, 0xFF, 0xFF);
	gFontMedium->renderText(xOffset, SCREEN_HEIGHT - (content - 2 * padding), text);
}