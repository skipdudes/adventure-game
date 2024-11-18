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
const SDL_KeyCode BUTTON_INFO = SDLK_TAB;
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

//Shared NPC instructions
const std::string STRING_NPC_BASE_CONTEXT = 
" Please try to answer shortly, maximum 50 words."
" Do not use any special characters, especially at the beginning and the end of the response, eg. newline character."
" At the end of your response, you need to set some parameters based on the conversation,"
" the parameters are float with value ranging from 0 to 1. parameters are: Happiness, Trust, Hostility."
" You MUST include all of them in every response and you should behave accordingly to their value."
" The structure of response: (Happiness: 0.0, Trust: 0.0, Hostility: 0.0). Starting values are: ";

//RDF shared basic knowledge
const std::string STRING_RDF_BASE_CONTEXT = 
" Here's some information about the world you're living in:"
" The person you are talking with is the player."
" He's a 20 year old Royal Guard and his name is Anthony."
" The King lives in the castle."
" Player's father Henry lives in the castle."
" Royal Guard Michael lives in the castle."
" The Innkeeper lives in the Inn."
" The Marquis lives in his house. It is located south to the castle and it's made of brick.";
//todo: ******************************************* Complete RDF information *******************************************

//King
const std::string STRING_KING_NAME = "The King";
const std::filesystem::path FILE_KING_TEXTURE = DIR_NPC / "King.png";
const std::filesystem::path FILE_KING_DIALOGUE_TEXTURE = DIR_DIALOGUE / "King.png";

//King basic context
const std::string STRING_KING_CONTEXT =
"You are the king of a medieval city."
" Your name is Arthur and you are 55 years old."
" You are commanding, apodictic and intelligent."
" You don't trust many people and have a narrow circle of trusted advisors."
" You trust the prime minister."
" Richard, your nephew, was found dead yesterday, stabbed in a chest, lying in nearby forest."
" You are devastated about it, because you loved him like your own.";

//King quest 1 false context
const std::string RDF_INVESTIGATION_ASSIGNED_FALSE =
"A player came to you."
" You tell him what you know about Richard only if player asks you to. Last time that you saw Richard was yesterday in your castle."
" Richard told you then that he was going to the local Inn. Talking to player makes you more happy."
" Your happiness increases quickly when player is nice to you."
" If player asks you if he could help when requirements are not met (Happiness < 0.6 and Hostility > 0.3), then tell player that you are greatful for asking but now is not the best moment."
" If requirements are met (Happiness >= 0.6 and Hostility <= 0.3) then you tell player that you would want him to conduct an investigation. That's crucial that you don't tell him about it before requirements are met."
" He should go to the local Inn and see if bartender knows anything. If player didn't ask, then you don't tell him about it.";

//King quest 1 true context
const std::string RDF_INVESTIGATION_ASSIGNED_TRUE =
"If you didn't yet tell player to conduct an investigation, do it now. Tell him that he should go to the local Inn and see if bartender knows anything."
"You were devastated about your nephew death, but player made you feel better."
" You are thankful to him for that, but despite that you don't want to chat with him and tell him to go to the local inn already.";


//Father
const std::string STRING_FATHER_NAME = "The Father";
const std::filesystem::path FILE_FATHER_TEXTURE = DIR_NPC / "Father.png";
const std::filesystem::path FILE_FATHER_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Father.png";
const std::string STRING_FATHER_CONTEXT = 
"You are the father of the person you are talking with. You both live in a medieval city."
" Your name is Henry, you are 54 years old and you are the Prime Minister."
" You are strict, intelligent and political savvy."
" You are loyal to the king."
" You value honor and a good family background.";

//RoyalGuard
const std::string STRING_ROYALGUARD_NAME = "Royal Guard";
const std::filesystem::path FILE_ROYALGUARD_TEXTURE = DIR_NPC / "RoyalGuard.png";
const std::filesystem::path FILE_ROYALGUARD_DIALOGUE_TEXTURE = DIR_DIALOGUE / "RoyalGuard.png";
const std::string STRING_ROYALGUARD_CONTEXT = 
"You are a Royal Guard in a medieval city."
" Your name is Michael and you are 21 years old."
" You are friends with the player and Richard."
" You are smart, hardworking and reliable."
" You love your family, friends and the King.";

//Innkeeper
const std::string STRING_INNKEEPER_NAME = "The Innkeeper";
const std::filesystem::path FILE_INNKEEPER_TEXTURE = DIR_NPC / "Innkeeper.png";
const std::filesystem::path FILE_INNKEEPER_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Innkeeper.png";
const std::string STRING_INNKEEPER_CONTEXT =
"You are the owner of an inn in a medieval city."
" You are 48 years old and your name is John."
" You're shrewd and money-grubbing.";

const std::string RDF_INNKEEPER_TOLD_ABOUT_WOMAN_FALSE =
"A player came into your Inn. This is the first time you see him and you don't seem to trust him, although you are quite easy to convince if you hear a good story."
" If player asks you about Richard when requirements are not met (Trust < 0.5) then you can't tell him about Richard."
" If requirements are met (Trust >= 0.5) and player asks you about Richard,"
" then you tell him that you saw Richard leave your Inn with a mysterious woman yesterday right before it was closed.";

const std::string RDF_INNKEEPER_TOLD_ABOUT_WOMAN_TRUE =
" You were suspicious about the player, but now he gained some of your trust."
" If he asks about the woman, you tell him that you saw her a few times in your inn talking with a Marquis, so maybe he might know something. Use your own words.";

//Marquis
const std::string STRING_MARQUIS_NAME = "The Marquis";
const std::filesystem::path FILE_MARQUIS_TEXTURE = DIR_NPC / "Marquis.png";
const std::filesystem::path FILE_MARQUIS_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Marquis.png";
const std::string STRING_MARQUIS_CONTEXT =
"You are a Marquis in a medieval city."
" You are usually loud and positive person"
" You are friendly and welcoming"
" You are easy to throw off balance, you get hostile when someone is pushing you."
" You are in love with myserious woman, Emma. You are jealous when somebody else is seeing her."
" Player made you furious insinuating Emma's affair and because of that you slipped out that she was just getting information out of him. You try to convince him that you just made it all up in a fit of anger. You dont want to see the player anymore and if he tries to talk with you you tell him to get the hell out of your house. Use your own words. ";


//todo: ******************************************* Complete Marquis' unique context *******************************************

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
