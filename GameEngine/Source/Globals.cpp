#include "Globals.h"
#include "Levels/Exit.h"

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