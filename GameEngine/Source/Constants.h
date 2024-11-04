#pragma once
#include <SDL.h>
#include <string>
#include <filesystem>

//Game properties
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FPS = 60;
const int MIN_FRAMETIME = 1000 / FPS;	//Measured in ms
const int MAX_UPDATETIME = 10;			//Measured in ms
const std::string TITLE = "Shadows of The Crown";
const std::string VERSION = "version 0.1a";

//Buttons
const SDL_KeyCode BUTTON_MOVE_UP = SDLK_UP;
const SDL_KeyCode BUTTON_MOVE_DOWN = SDLK_DOWN;
const SDL_KeyCode BUTTON_MOVE_LEFT = SDLK_LEFT;
const SDL_KeyCode BUTTON_MOVE_RIGHT = SDLK_RIGHT;

const SDL_KeyCode BUTTON_CONFIRM = SDLK_RETURN;
const SDL_KeyCode BUTTON_QUIT = SDLK_ESCAPE;
const SDL_KeyCode BUTTON_FULLSCREEN = SDLK_F4;

//Files and directories
//Logger
const std::filesystem::path DIR_LOGS = "Logs";
const std::filesystem::path FILE_LOG = DIR_LOGS / "latest.log";

//Data
const std::filesystem::path DIR_DATA = "Data";

//Fonts
const std::filesystem::path DIR_FONTS = DIR_DATA / "Fonts";
const std::filesystem::path FILE_FONT_SMALL_TEXTURE = DIR_FONTS / "TinyUnicode.png";
const std::filesystem::path FILE_FONT_MEDIUM_TEXTURE = DIR_FONTS / "CelticTime.png";
const std::filesystem::path FILE_FONT_LARGE_TEXTURE = DIR_FONTS / "PixNull.png";

//Textures
const std::filesystem::path DIR_TEXTURES = DIR_DATA / "Textures";

//Player
const std::filesystem::path DIR_PLAYER = DIR_TEXTURES / "Player";
const std::filesystem::path FILE_PLAYER_TEXTURE = DIR_PLAYER / "Player.png";

//NPC
const std::filesystem::path DIR_NPC = DIR_TEXTURES / "NPC";
const std::filesystem::path FILE_KING_TEXTURE = DIR_NPC / "King.png";

//Levels
const std::filesystem::path DIR_LEVELS = DIR_TEXTURES / "Levels";
const std::filesystem::path FILE_MENU_TEXTURE = DIR_LEVELS / "Menu.png";
const std::filesystem::path FILE_AUTHORS_TEXTURE = DIR_LEVELS / "Authors.png";
const std::filesystem::path FILE_OVERWORLD_TEXTURE = DIR_LEVELS / "Overworld.png";
const std::filesystem::path FILE_CASTLE_TEXTURE = DIR_LEVELS / "Castle.png";

//Menu options
const std::string OPTION_START = "Start game";
const std::string OPTION_AUTHORS = "Authors";
const std::string OPTION_QUIT = "End game";

//Authors level text
const std::string AUTHORS = "Authors";
const std::string AUTHOR_MARCIN = "Marcin Chetnik";
const std::string AUTHOR_ANDRZEJ = "Andrzej Woroniecki";
const std::string AUTHOR_MARTA = "Marta Makowska";
const std::string OPTION_BACK = "Press ENTER to go back";

//Dialogue
const std::string BEGIN_DIALOGUE_PROMPT = "Press ENTER to speak with ";
const std::string STRING_KING_NAME = "The King";
const std::string DIALOGUE_START_PROMPT = "Enter text. Press ESC to quit";
const std::string DIALOGUE_HELP_PROMPT = "Press ENTER to confirm, press ESC to quit";
const std::string DIALOGUE_TOO_LONG = "Too many characters!";
const std::string DIALOGUE_HELP_NPC_TURN = "Press ENTER to reply, press ESC to quit";
const std::filesystem::path DIR_DIALOGUE = DIR_TEXTURES / "Dialogue";
const std::filesystem::path FILE_PLAYER_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Player.png";
const std::filesystem::path FILE_KING_DIALOGUE_TEXTURE = DIR_DIALOGUE / "King.png";
