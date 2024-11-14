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
const std::filesystem::path DIR_DIALOGUE = DIR_TEXTURES / "Dialogue";

//Player
const std::filesystem::path DIR_PLAYER = DIR_TEXTURES / "Player";
const std::filesystem::path FILE_PLAYER_TEXTURE = DIR_PLAYER / "Player.png";
const std::filesystem::path FILE_PLAYER_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Player.png";

//NPC
const std::filesystem::path DIR_NPC = DIR_TEXTURES / "NPC";
const std::string STRING_HAPPINESS = "Happiness";
const std::string STRING_TRUST = "Trust";
const std::string STRING_HOSTILITY = "Hostility";
const std::string STRING_NPC_BASE_CONTEXT = 
" Please try to answer shortly, maximum 50 words."
" Do not use any special characters, especially at the beginning and the end of the response. eg. newline character"
" At the end of your response, you need to set some parameters based on the conversation,"
" the parameters are float with value ranging from 0 to 1. parameters are: Happiness, Trust, Hostility."
" You MUST include all of them in every response and you should behave accordingly to their value."
" The structure of response: (Happiness: 0.0, Trust: 0.0, Hostility: 0.0). Starting values are: ";
//const std::string STRING_RDF_BASE_CONTEXT = "The King lives in the castle";
//todo -> rozbuduj kontekst RDF, wspolny dla wszystkich NPC

//King
const std::string STRING_KING_NAME = "The King";
const std::filesystem::path FILE_KING_TEXTURE = DIR_NPC / "King.png";
const std::filesystem::path FILE_KING_DIALOGUE_TEXTURE = DIR_DIALOGUE / "King.png";
const std::string STRING_KING_CONTEXT = "You are the king of a medieval city.";
//todo -> rozbuduj kontekst

//Father
const std::string STRING_FATHER_NAME = "The Father";
const std::filesystem::path FILE_FATHER_TEXTURE = DIR_NPC / "Father.png";
const std::filesystem::path FILE_FATHER_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Father.png";
const std::string STRING_FATHER_CONTEXT = "You are the father of the person you are talking with. You both live in a medieval city.";
//todo -> rozbuduj kontekst

//RoyalGuard
const std::string STRING_ROYALGUARD_NAME = "Royal Guard";
const std::filesystem::path FILE_ROYALGUARD_TEXTURE = DIR_NPC / "RoyalGuard.png";
const std::filesystem::path FILE_ROYALGUARD_DIALOGUE_TEXTURE = DIR_DIALOGUE / "RoyalGuard.png";
const std::string STRING_ROYALGUARD_CONTEXT = "You are a royal guard in a medieval city.";
//todo -> rozbuduj kontekst

//Innkeeper
const std::string STRING_INNKEEPER_NAME = "The Innkeeper";
const std::filesystem::path FILE_INNKEEPER_TEXTURE = DIR_NPC / "Innkeeper.png";
const std::filesystem::path FILE_INNKEEPER_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Innkeeper.png";
const std::string STRING_INNKEEPER_CONTEXT = "You are the owner of an inn in a medieval city.";
//todo -> rozbuduj kontekst

//Marquis
const std::string STRING_MARQUIS_NAME = "The Marquis";
const std::filesystem::path FILE_MARQUIS_TEXTURE = DIR_NPC / "Marquis.png";
const std::filesystem::path FILE_MARQUIS_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Marquis.png";
const std::string STRING_MARQUIS_CONTEXT = "You are a Marquis in a medieval city.";
//todo -> rozbuduj kontekst

//Levels
const std::filesystem::path DIR_LEVELS = DIR_TEXTURES / "Levels";
const std::filesystem::path FILE_MENU_TEXTURE = DIR_LEVELS / "Menu.png";
const std::filesystem::path FILE_AUTHORS_TEXTURE = DIR_LEVELS / "Authors.png";
const std::filesystem::path FILE_OVERWORLD_TEXTURE = DIR_LEVELS / "Overworld.png";
const std::filesystem::path FILE_CASTLE_TEXTURE = DIR_LEVELS / "Castle.png";
const std::filesystem::path FILE_INN_TEXTURE = DIR_LEVELS / "Inn.png";
const std::filesystem::path FILE_HOUSE_TEXTURE = DIR_LEVELS / "House.png";

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

//Go to next level prompt
const std::string NEXT_LEVEL_PROMPT = "Press ENTER to continue";

//Dialogue
const std::string BEGIN_DIALOGUE_PROMPT = "Press ENTER to speak with ";
const std::string DIALOGUE_START_PROMPT = "Enter text. Press ESC to quit";
const std::string DIALOGUE_HELP_PROMPT = "Press ENTER to confirm, press ESC to quit";
const std::string DIALOGUE_TOO_LONG = "Too many characters!";
const std::string DIALOGUE_HELP_NPC_THINKING = "Wait for the response";
const std::string DIALOGUE_HELP_NPC_TURN = "Press ENTER to reply, press ESC to quit";

//Game events
//const std::string STRING_RDF_PLAYER_FOUND_BODY = "The person you are speaking with has found the body";
//todo -> reprezentacja tekstowa booli
