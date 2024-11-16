#pragma once
#include <SDL.h>
#include <memory>
#include "Fonts/Font.h"
#include "Player/Player.h"
#include "Levels/Level.h"
#include "NPC/NPC.h"

//Renderer
extern SDL_Renderer* gRenderer;

//Fonts
extern std::unique_ptr<Font> gFontSmall;
extern std::unique_ptr<Font> gFontMedium;
extern std::unique_ptr<Font> gFontLarge;

//Player
extern std::unique_ptr<Player> gPlayer;

//NPC
extern std::shared_ptr<NPC> gKing;
extern std::shared_ptr<NPC> gFather;
extern std::shared_ptr<NPC> gRoyalGuard;
extern std::shared_ptr<NPC> gInnkeeper;
extern std::shared_ptr<NPC> gMarquis;

//Levels
void setNextState(Level* nextLevel);
void changeState();
extern Level* gCurrentLevel;
extern Level* gNextLevel;
bool checkCollision(SDL_Rect a, SDL_Rect b);

//Game events
//extern bool g_RDF_PlayerFoundBody;
//todo -> game events bools
//+ per individual npc

//HUD
void renderPrompt(int xOffset, std::string text, int linesNumber, Uint8 r, Uint8 g, Uint8 b);
void renderQuestPrompt(std::string text);
void renderCurrentQuest();
void toggleQuestPrompt(SDL_Event& e);
extern bool gDisplayQuestPrompt;
