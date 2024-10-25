#pragma once
#include <SDL.h>
#include <memory>
#include "Fonts/Font.h"
#include "Player/Player.h"
#include "Levels/Level.h"

//Renderer
extern SDL_Renderer* gRenderer;

//Fonts
extern std::unique_ptr<Font> gFontSmall;
extern std::unique_ptr<Font> gFontMedium;
extern std::unique_ptr<Font> gFontLarge;

//Player
extern std::unique_ptr<Player> gPlayer;

//Levels
void setNextState(Level* nextLevel);
void changeState();
extern Level* gCurrentLevel;
extern Level* gNextLevel;
bool checkCollision(SDL_Rect a, SDL_Rect b);
