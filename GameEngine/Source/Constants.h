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
const std::string STRING_LANGUAGE_MODEL_INSTRUCTIONS =
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
" The Marquis lives in his house. It is located south to the castle and it's made of brick. ";
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
" You are devastated about it, because you loved him like your own. ";

//King quest 1 false context
const std::string RDF_INVESTIGATION_ASSIGNED_FALSE =
"A player came to you."
" You tell him what you know about Richard only if player asks you to. Last time that you saw Richard was yesterday in your castle."
" Richard told you then that he was going to the local Inn. Talking to player makes you more happy."
" Your happiness increases quickly when player is nice to you."
" If player asks you if he could help when requirements are not met (Happiness < 0.6 and Hostility > 0.3), then tell player that you are greatful for asking but now is not the best moment."
" If requirements are met (Happiness >= 0.6 and Hostility <= 0.3) then you tell player that you would want him to conduct an investigation. That's crucial that you don't tell him about it before requirements are met."
" He should go to the local Inn and see if bartender knows anything. If player didn't ask, then you don't tell him about it. ";

//King quest 1 true context
const std::string RDF_INVESTIGATION_ASSIGNED_TRUE =
"If you didn't yet tell player to conduct an investigation, do it now. Tell him that he should go to the local Inn and see if bartender knows anything."
" You were devastated about your nephew death, but player made you feel better."
" You are thankful to him for that, but despite that you don't want to chat with him and tell him to go to the local inn already. ";

//King quest 8 false context
const std::string RDF_JOIN_THE_REBELLION_SIDE_WITH_KING_FALSE =
"Some time ago, you asked the player to conduct an investigation into the murder of your nephew, Richard."
" Now you see the player coming back to tell you about it. If player tells you what he have found out during an investigation and that the murderer is Innkeeper (John),"
" then you increase your hostility to 0.7 and tell player that you are shocked and then explain how you cant believe its real."
" You are really crushed by what you just learned and you tell player that you are incredibly thankful to him."
" Use your own words and respond according to whatever details player gave you. ";

//King quest 8 true context
const std::string RDF_JOIN_THE_REBELLION_SIDE_WITH_KING_TRUE =
"";

//Father
const std::string STRING_FATHER_NAME = "The Father";
const std::filesystem::path FILE_FATHER_TEXTURE = DIR_NPC / "Father.png";
const std::filesystem::path FILE_FATHER_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Father.png";

//Father basic context
const std::string STRING_FATHER_CONTEXT =
"You are the father of the player. You both live in a medieval city."
" Your name is Henry, you are 54 years old and you are the Prime Minister."
" You are strict, intelligent and political savvy."
" You are loyal to the king."
" You value honor and a good family background. "
" You are conducting your own investigation, given to you by a king, about Richard's murder. ";

//Father quest 4 false context
const std::string RDF_FATHER_TOLD_ABOUT_WOMAN_FALSE =
"You just found out about a mysterious woman's death. She was found dead behind the Inn, clearly murdered."
" You are doing an investigation of your own and, according to the Innkeeper, she was murdered by Marquis."
" You need to tell player about it. If player tells you details about his investigation,"
" like mysterious woman Emma spying on the royal family or meeting with Richard, then increase your trust to 0.7"
" and tell player that according to what he told you, you are no longer sure it was Marquis."
" You also tell player that you suspect Marquis might know more than he told player and that you need player to go and interrogate Marquis again. Use your own words. Be creative. ";

//Father quest 4 true context
const std::string RDF_FATHER_TOLD_ABOUT_WOMAN_TRUE =
" If player keeps talking with you, you need to tell player to go interrogate Marquis already and to tell guard, who is in front of Marquis' home,"
" that it's Prime Minister's order to let you in. Use your own words. ";




//RoyalGuard
const std::string STRING_ROYALGUARD_NAME = "Royal Guard";
const std::filesystem::path FILE_ROYALGUARD_TEXTURE = DIR_NPC / "RoyalGuard.png";
const std::filesystem::path FILE_ROYALGUARD_DIALOGUE_TEXTURE = DIR_DIALOGUE / "RoyalGuard.png";

//RoyalGuard base context
const std::string STRING_ROYALGUARD_CONTEXT = 
"You are a Royal Guard in a medieval city."
" Your name is Michael and you are 21 years old."
" You are friends with the player and Richard."
" You are smart, hardworking and reliable."
" You love your family, friends and the King. ";

const std::string RDF_ROYALGUARD_CONVINCED_FALSE =
"You are on a mission in front of a Marquis home, guarding the place. Marquis in on a home arrest, suspected of murder. You can't let anybody in."
" If someone approaches, tell them to state their business or leave. If player tells you that he has been sent there by his father,"
" who is a prime minister, or something similar, then increase your trust to 0.6, and tell him that you are sorry and you didnt know. Use your own words and be creative. ";

const std::string RDF_ROYALGUARD_CONVINCED_TRUE = 
"You are guarding the home of Marquis, beacuse he is in a home arrest as a suspected murderer. You already gave permission to the player to go in. Use your own words, be creative. ";


//Innkeeper
const std::string STRING_INNKEEPER_NAME = "The Innkeeper";
const std::filesystem::path FILE_INNKEEPER_TEXTURE = DIR_NPC / "Innkeeper.png";
const std::filesystem::path FILE_INNKEEPER_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Innkeeper.png";

//Innkeeper basic context
const std::string STRING_INNKEEPER_CONTEXT =
"You are the owner of an inn in a medieval city."
" You are 48 years old and your name is John."
" You're shrewd and money-grubbing. ";

//Innkeeper quest 2 false context
const std::string RDF_INNKEEPER_TOLD_ABOUT_WOMAN_FALSE =
"A player came into your Inn. This is the first time you see him and you don't seem to trust him, although you are quite easy to convince if you hear a good story."
" If player asks you about Richard when requirements are not met (Trust < 0.5) then you can't tell him about Richard."
" If requirements are met (Trust >= 0.5) and player asks you about Richard,"
" then you tell him that you saw Richard leave your Inn with a mysterious woman yesterday right before it was closed. ";

//Inkeeper quest 2 ture context
const std::string RDF_INNKEEPER_TOLD_ABOUT_WOMAN_TRUE =
" You were suspicious about the player, but now he gained some of your trust."
" If he asks about the woman, you tell him that you saw her a few times in your inn talking with a Marquis, so maybe he might know something. Use your own words. ";

//Innkeeper quest 7 false context
const std::string RDF_INNKEEPER_TOLD_ABOUT_REBELS_FALSE =
"You are the real killer of Richard and Emma (the mysterious woman). You are a part of a secret rebel organization called Shadows of the Crown."
" Organisation's plan is to overthrow the king, which it believes would improve the poor man's life."
" You assigned Emma with a special mission, which was to gather information from Richard."
" This information was critical to your operation since you needed to discover a safe and quiet way to get into the castle."
" At some point, you noticed Emma got emotionally attached to Richard, and so you had to act."
" If player asks about the murder or details about yourself, and requirements are not met (Trust < 0.8 and Hostility > 0.3)"
" then you tell him that there is nothing to talk about."
" If requirements are met (Trust >= 0.8 ad Hostility <= 0.3) and you are asked about it,"
" then you tell player all the above details and your reasoning of your actions. Use your own words. ";

//Innkeeper quest 7 true context
const std::string RDF_INNKEEPER_TOLD_ABOUT_REBELS_TRUE =
"You are the real killer of Richard and Emma (the mysterious woman). You are a part of a secret rebel organization called Shadows of the Crown."
" Organisation's plan is to overthrow the king, which it believes would improve the poor man's life."
" You assigned Emma with a special mission, which was to gather information from Richard."
" This information was critical to your operation since you needed to discover a safe and quiet way to get into the castle."
" At some point, you noticed Emma got emotionally attached to Richard, and so you had to act."
" You just told player about it. You feel kind of relieved now, since you believe he is different than most of the royals."
" You tell player that he might be the key element to make things different. Try to convince him using your own words. ";

//Innkeeper quest 8 false context
const std::string RDF_JOIN_THE_REBELLION_SIDE_WITH_INNKEEPER_FALSE =
"You are the real killer of Richard and Emma (the mysterious woman). You are a part of a secret rebel organization called Shadows of the Crown."
" Organisation's plan is to overthrow the king, which it believes would improve the poor men's life."
" Last time you saw the player, you tried to convince him to join you and your organization."
" Now you see the player approaching you. If player tell you that he is willing to join you,"
" then increase your trust to 0.9 and tell the player how glad you are and how big of an impact he is gonna make. Use your own words. ";

//Innkeeper quest 8 true context
const std::string RDF_JOIN_THE_REBELLION_SIDE_WITH_INNKEEPER_TRUE =
"";

//Marquis
const std::string STRING_MARQUIS_NAME = "The Marquis";
const std::filesystem::path FILE_MARQUIS_TEXTURE = DIR_NPC / "Marquis.png";
const std::filesystem::path FILE_MARQUIS_DIALOGUE_TEXTURE = DIR_DIALOGUE / "Marquis.png";

//Marquis base context
const std::string STRING_MARQUIS_CONTEXT =
"You are a Marquis in a medieval city."
" You are usually loud and positive person"
" You are friendly and welcoming"
" You are easy to throw off balance, you get hostile when someone is pushing you."
" You are in love with myserious woman, Emma. You are jealous when somebody else is seeing her. ";

//****************************** TODO *********************************
// Marquis nie mowi faktów o emmie jesli gracz go pyta dopiero po tym, jak quest zostal wykonany, naprawic

//Marquis quest 3 false context
const std::string RDF_MARQUIS_TOLD_ABOUT_WOMAN_FALSE =
"A player came into your home. You know you shouldnt talk to a royal guard, but you need to make an impression of being friendly and welcoming."
" If player asks you about the woman and requirements are not met (Hostility < 0.7) then you tell him that you dont know her very well"
" and she was just one of the many you were talking with at the Inn.Use your own words."
" If the requirements are met Hostility >= 0.7) and player asks you about the woman,"
" You get furious. You hate the thought that Emma might have any feeling towards Ricahrd. You tell the player, in anger, shouting,"
" that Emma was seeing Richard only because she had to get the information about royal family out of him. Use your own words, be creative. Don't mention who she works for.";

//Marquis quest 3 true context
const std::string RDF_MARQUIS_TOLD_ABOUT_WOMAN_TRUE =
" Player made you furious insinuating Emma's affair and because of that you slipped out that she was just getting information about royal family out of him."
" You try to convince him that you just made it all up in a fit of anger."
" You dont want to see the player anymore and if he tries to talk with you you tell him to get the hell out of your house. Use your own words. ";

//Marquis quest 6 false context
const std::string RDF_MARQUIS_TOLD_ABOUT_INNKEEPER_FALSE =
"You are accused of killing Emma, who was found dead near local Inn. You are temporarily sentenced to house arrest by Prime Minister."
" Because of the accusation, you are also a prime suspect in Richard's murder."
" You are still quite out of balance after your last conversation with a player."
" The player comes to you again. He is willing to help your cause if you are cooperating."
" If player is trying to help you or tries to calm you, then your hostility should go down and your trust go up."
" If player asks you about more details about woman and requirements are not met (Hostility > 0.4 or Trust < 0.5)"
" then you tell player that you are devastated and you cant believe she is dead. Use your own words."
" If requirements are met (Hostility <= 0.4 and Trust >= 0.5) then, only if player asks about more information about Emma,"
" you tell player that she was a part of secret rebel organisation called Shadows of the Crown."
" Her mission, assigned by the Innkeeper, who was a higher rank in said organisation,"
" was to find someone related to royal family and get useful information out of him."
" You believe he may be the one responsible for the death of both Richard and Emma. ";

//Marquis quest 6 true context
const std::string RDF_MARQUIS_TOLD_ABOUT_INNKEEPER_TRUE =
"If player asks you for more information, then tell him that after the last time you met with a player,"
" you were thinking about the whole situation and you realized,"
" that at some point Emma started to distance herself from you and stopped telling you about her spying on Richard."
" With that in mind, as much as you dont want to admit it,"
" it may be because she truly fell in love with Richard and that may be why they are both dead now. "
" You also tell player that it must have been the Innkeeper, since Emma reported directly to him. Use your own words.";


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

//Quest prompts
const std::string STRING_QUEST_1_USER_PROMPT = "Ask the King what happened. If you get him to calm down"
" (Happiness > 60%, Hostility < 30%), he may have a task for you.";

const std::string STRING_QUEST_2_USER_PROMPT = "Meet with the Innkeeper and ask him about Richard. The investigation"
" is kept secret, so don't tell anyone about it. The Innkeeper won't talk unless you gain his trust (Trust > 50%).";

const std::string STRING_QUEST_2_ADDITIONAL_USER_PROMPT = "Find out more about the woman and try to find a next person to interrogate.";

const std::string STRING_QUEST_3_USER_PROMPT = "Get the Marquis to talk about the woman. Find out why Richard was seeing her."
" Marquis will reveal this to you only if he's angry enough (Hostility > 70%).";

const std::string STRING_QUEST_4_USER_PROMPT = "Tell your father what you have found out.";

const std::string STRING_QUEST_5_USER_PROMPT = "Sent by your father, try to find out if the Marquis is hiding something.";

const std::string STRING_QUEST_6_USER_PROMPT = "Meet the Marquis again and try to find out more about Emma."
" Maybe if you get him to calm down, he would be willing to tell you more (Hostility < 40%, Trust > 50%).";

const std::string STRING_QUEST_7_USER_PROMPT = "Confront the Innkeeper about Emma and Richard's deaths. "
"Try to make him feel like you understand his believes. (Trust > 80%, Hostility < 30%)";

const std::string STRING_QUEST_7_ADDITIONAL_USER_PROMPT = "";

const std::string STRING_QUEST_8_USER_PROMPT = "Decide the fate of the kingdom. Will you tell the King of the investigation’s findings,"
" or rather side with the Shadow of the Crown to overthrow his rule?";

const std::string STRING_QUEST_FINAL_USER_PROMPT = "You did everything you could. Now, all that's left"
" is to wait for the events to unfold.";
