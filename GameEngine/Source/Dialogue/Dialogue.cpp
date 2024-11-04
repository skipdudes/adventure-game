#include "Dialogue.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"

Dialogue::Dialogue(const std::string& NPCName, const std::string& NPCDialogueTexturePath)
	:mCurrentLine(""), mCurrentTurn(Turn::PLAYER_TURN), mChangeTurn(false)
{
	LOG_INFO("Called Dialogue constructor");

	mNPCName = NPCName;
	mNPCDialogueTexturePath = NPCDialogueTexturePath;

	mPlayerDialogueTexture = std::make_unique<Texture>();
	mNPCDialogueTexture = std::make_unique<Texture>();

	//Initial Player turn
	SDL_StartTextInput();
}

bool Dialogue::load()
{
	if (!(mPlayerDialogueTexture->load(FILE_PLAYER_DIALOGUE_TEXTURE.string())))
	{
		LOG_ERROR("Could not load Player dialogue texture");
		return false;
	}

	if (!(mNPCDialogueTexture->load(mNPCDialogueTexturePath)))
	{
		LOG_ERROR("Could not load NPC dialogue texture");
		return false;
	}

	LOG_INFO("Successfully loaded dialogue textures");
	return true;
}

void Dialogue::handleEventsPlayerTurn(SDL_Event& e)
{
	//Pressed non-text button
	if (e.type == SDL_KEYDOWN)
	{
		//Send line
		if (e.key.keysym.sym == BUTTON_CONFIRM)
		{
			if (mCurrentLine.length() > 0 && mCurrentLine.length() <= MAXIMUM_INPUT)
			{
				//Send dialogue to language model
				LOG_INFO("(" + std::to_string(mCurrentLine.length()) + ") " + mCurrentLine);

				//Change turn
				mChangeTurn = true;
			}
		}
		//Backspace
		else if (e.key.keysym.sym == SDLK_BACKSPACE)
		{
			if (mCurrentLine.length() > 0)
			{
				mCurrentLine.pop_back();
			}
		}
	}
	//Text buttons
	else if (e.type == SDL_TEXTINPUT)
	{
		if (mCurrentLine.length() <= MAXIMUM_INPUT)
			mCurrentLine += e.text.text;
	}
}

void Dialogue::handleEventsNPCTurn(SDL_Event& e)
{
	//Pressed confirm
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == BUTTON_CONFIRM)
	{
		mChangeTurn = true;
	}
}

void Dialogue::handleEvents(SDL_Event& e)
{
	if (mCurrentTurn == Turn::PLAYER_TURN)
		handleEventsPlayerTurn(e);
	else if (mCurrentTurn == Turn::NPC_TURN)
		handleEventsNPCTurn(e);
}

void Dialogue::updatePlayerTurn()
{

}

void Dialogue::updateNPCTurn()
{

}

void Dialogue::update()
{
	//Change turn
	if (mChangeTurn)
	{
		if (mCurrentTurn == Turn::PLAYER_TURN)
		{
			//Player -> NPC
			SDL_StopTextInput();
			mCurrentTurn = Turn::NPC_TURN;
		}
		else if (mCurrentTurn == Turn::NPC_TURN)
		{
			//NPC -> Player
			SDL_StartTextInput();
			mCurrentTurn = Turn::PLAYER_TURN;
		}

		mChangeTurn = false;
		mCurrentLine = "";
	}
}

void Dialogue::renderPlayerTurn(const int& padding, const int& content, const int& promptHeight)
{
	//Line
	gFontSmall->setColor(0xFF, 0xFF, 0xFF);
	gFontSmall->renderDialogueText(4 * padding, SCREEN_HEIGHT - (promptHeight)-(content - 2 * padding), mCurrentLine, SCREEN_WIDTH - (4 * padding));

	//Avatar
	mPlayerDialogueTexture->render(0, SCREEN_HEIGHT - (promptHeight)-(2 * padding + content) - mPlayerDialogueTexture->getHeight());

	//Prompts
	if (mCurrentLine.length() > MAXIMUM_INPUT)
		renderPrompt(207, DIALOGUE_TOO_LONG, 1, 0xFF, 0x00, 0x00);
	else if (mCurrentLine.length() <= 0)
		renderPrompt(166, DIALOGUE_START_PROMPT, 1, 0xFF, 0xFF, 0xFF);
	else
		renderPrompt(93, DIALOGUE_HELP_PROMPT, 1, 0xFF, 0xFF, 0xFF);
}

void Dialogue::renderNPCTurn(const int& padding, const int& content, const int& promptHeight)
{
	//Line
	gFontSmall->setColor(0xFF, 0xFF, 0xFF);
	gFontSmall->renderDialogueText(4 * padding, SCREEN_HEIGHT - (promptHeight)-(content - 2 * padding), mCurrentLine, SCREEN_WIDTH - (4 * padding));

	//Avatar
	mNPCDialogueTexture->render(SCREEN_WIDTH - mNPCDialogueTexture->getWidth(), SCREEN_HEIGHT - (promptHeight)-(2 * padding + content) - mNPCDialogueTexture->getHeight());

	//Prompts
	renderPrompt(106, DIALOGUE_HELP_NPC_TURN, 1, 0xFF, 0xFF, 0xFF);
}

void Dialogue::render()
{
	int linesNumber = 4;
	int padding = 2;
	int content = 28 * linesNumber;
	int promptHeight = 34;

	//Outer box
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect outerBox = { 0, SCREEN_HEIGHT - (promptHeight)-(2 * padding + content), SCREEN_WIDTH, (2 * padding + content) };
	SDL_RenderFillRect(gRenderer, &outerBox);

	//Inner box
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect innerBox = { padding, SCREEN_HEIGHT - (promptHeight)-(padding + content), SCREEN_WIDTH - (2 * padding), content };
	SDL_RenderFillRect(gRenderer, &innerBox);

	if (mCurrentTurn == Turn::PLAYER_TURN)
		renderPlayerTurn(padding, content, promptHeight);
	else if (mCurrentTurn == Turn::NPC_TURN)
		renderNPCTurn(padding, content, promptHeight);
}

void Dialogue::free()
{
	mPlayerDialogueTexture->free();
	mNPCDialogueTexture->free();
}

Dialogue::~Dialogue()
{
	LOG_INFO("Called Dialogue deconstructor");

	//Free the memory
	free();

	//If ended on Player turn
	if (mCurrentTurn == Turn::PLAYER_TURN)
		SDL_StopTextInput();
}