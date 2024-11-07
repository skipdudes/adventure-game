#include "Dialogue.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "../SendPrompt.h"

Dialogue::Dialogue(const std::shared_ptr<NPC>& NPC)
	:mCurrentLine(""), mCurrentTurn(Turn::PLAYER_TURN), mChangeTurn(false)
{
	LOG_INFO("Called Dialogue constructor");

	mNPC = NPC;

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

	if (!(mNPCDialogueTexture->load(mNPC->getDialogueTexturePath())))
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
		//Pressed confirm
		if (e.key.keysym.sym == BUTTON_CONFIRM)
		{
			if (mCurrentLine.length() > 0 && mCurrentLine.length() <= MAXIMUM_INPUT)
				mChangeTurn = true;
		}
		//Backspace
		else if (e.key.keysym.sym == SDLK_BACKSPACE)
		{
			if (mCurrentLine.length() > 0)
				mCurrentLine.pop_back();
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
		if (!(mNPC->mThinking))
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

void Dialogue::changeToNPCTurn()
{
	//User message backup
	std::string prompt = mCurrentLine;

	//Player turn ended
	SDL_StopTextInput();
	LOG_INFO("Sending line (" + std::to_string(prompt.length()) + ") " + prompt);

	//NPC is thinking
	mNPC->mThinking = true;
	mCurrentLine = mNPC->getName() + " is thinking...";

	//Begin generating response
	generatedResponse = std::async(
		std::launch::async,
		generateMessage,
		std::ref(mNPC->mMessages),
		mNPC->getContext(),
		std::ref(mNPC->mHappiness),
		std::ref(mNPC->mAnxiety),
		std::ref(mNPC->mHostility),
		prompt
	);

	//NPC turn started
	mCurrentTurn = Turn::NPC_TURN;
}

void Dialogue::changeToPlayerTurn()
{
	//Reset line, player will provide a new one
	mCurrentLine = "";

	//Player turn started
	SDL_StartTextInput();
	mCurrentTurn = Turn::PLAYER_TURN;
}

void Dialogue::update()
{
	//Change turn
	if (mChangeTurn)
	{
		if (mCurrentTurn == Turn::PLAYER_TURN)
			//Player -> NPC
			changeToNPCTurn();

		else if (mCurrentTurn == Turn::NPC_TURN)
			//NPC -> Player
			changeToPlayerTurn();

		mChangeTurn = false;
	}

	//If NPC is thinking and response is ready (w/o blocking the thread)
	if (mNPC->mThinking && generatedResponse.valid())
	{
		if (generatedResponse.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
		{
			//Retrieve response
			mCurrentLine = generatedResponse.get();

			//Log stats
			LOG_INFO(mNPC->getName() + " stats after the line:");
			LOG_INFO("Happiness: " + std::to_string(mNPC->mHappiness));
			LOG_INFO("Anxiety: " + std::to_string(mNPC->mAnxiety));
			LOG_INFO("Hostility: " + std::to_string(mNPC->mHostility));

			//Not thinking anymore
			mNPC->mThinking = false;
		}
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