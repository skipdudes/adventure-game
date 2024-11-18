#include "Dialogue.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"
#include "../SendPrompt.h"
#include "../UpdateBools.h"
#include <sstream>

Dialogue::Dialogue(const std::shared_ptr<NPC>& NPC)
	:mCurrentLine(""), mCurrentTurn(Turn::PLAYER_TURN), mChangeTurn(false), mLastRenderUpateTime(0)
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
	mCurrentLine = mNPC->getName() + " is thinking";
	mLastRenderUpateTime = SDL_GetTicks();



	// ******************* TODO ********************
	//std::string gameEvents = rdfBoolsToString()


	//std::string context = mNPC->getContext() + STRING_RDF_BASE_CONTEXT + rdfBoolsToString() + STRING_NPC_BASE_CONTEXT;
	std::string context = STRING_RDF_BASE_CONTEXT + mNPC->getContext() + mNPC->mRDFDynamicContext + STRING_NPC_BASE_CONTEXT;

	//Begin generating response
	generatedResponse = std::async(
		std::launch::async,
		generateMessage,
		std::ref(mNPC->mMessages),
		context,
		std::ref(mNPC->mHappiness),
		std::ref(mNPC->mTrust),
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
		//Add dot
		Uint32 currentTicks = SDL_GetTicks();
		if (currentTicks - mLastRenderUpateTime >= DOT_DURATION)
		{
			mCurrentLine += ".";
			mLastRenderUpateTime = currentTicks;
		}

		if (generatedResponse.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
		{
			//Retrieve response
			mCurrentLine = generatedResponse.get();

			//Log stats
			LOG_INFO(mNPC->getName() + " stats after the line:");
			LOG_INFO("Happiness: " + std::to_string(mNPC->mHappiness));
			LOG_INFO("Trust: " + std::to_string(mNPC->mTrust));
			LOG_INFO("Hostility: " + std::to_string(mNPC->mHostility));

			//TODO *************************************************************************
			updateBools(mNPC);

			//Not thinking anymore
			mNPC->mThinking = false;
		}
	}
}

void Dialogue::renderStat(int x, int y, const float& stat, bool good)
{
	int percentage = static_cast<int>(stat * 100);

	if (good)
	{
		if(percentage >= 0 && percentage < 34)
			//Weak
			gFontSmall->setColor(0xFF, 0x00, 0x00);
		else if (percentage >= 34 && percentage < 67)
			//Medium
			gFontSmall->setColor(0xFF, 0xA5, 0x00);
		else
			//Strong
			gFontSmall->setColor(0x00, 0xFF, 0x00);
	}
	else
	{
		if (percentage >= 0 && percentage < 34)
			//Strong
			gFontSmall->setColor(0x00, 0xFF, 0x00);
		else if (percentage >= 34 && percentage < 67)
			//Medium
			gFontSmall->setColor(0xFF, 0xA5, 0x00);
		else
			//Weak
			gFontSmall->setColor(0xFF, 0x00, 0x00);
	}

	std::ostringstream oss;
	oss << percentage << "%";

	gFontSmall->renderStat(x, y, oss.str());
}

void Dialogue::renderPlayerTurn(const int& padding, const int& content, const int& promptHeight)
{
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
	//Prompts
	if (mNPC->mThinking)
		renderPrompt(204, DIALOGUE_HELP_NPC_THINKING, 1, 0xFF, 0xFF, 0xFF);
	else
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

	//Line
	gFontSmall->setColor(0xFF, 0xFF, 0xFF);
	gFontSmall->renderDialogueText(4 * padding, SCREEN_HEIGHT - (promptHeight)-(content - 2 * padding), mCurrentLine, SCREEN_WIDTH - (4 * padding));

	//Player Avatar
	mPlayerDialogueTexture->render(0, SCREEN_HEIGHT - (promptHeight)-(2 * padding + content) - mPlayerDialogueTexture->getHeight());

	//Stats boxes
	int statsInnerBoxSizeX = 136;
	int statsInnerBoxSizeY = 84;

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect statsOuterBox = { SCREEN_WIDTH - (2 * padding) - statsInnerBoxSizeX,
		SCREEN_HEIGHT - (promptHeight)-(2 * padding + content) - padding - statsInnerBoxSizeY,
		statsInnerBoxSizeX + (2 * padding),
		statsInnerBoxSizeY + padding };
	SDL_RenderFillRect(gRenderer, &statsOuterBox);

	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect statsInnerBox = { SCREEN_WIDTH - padding - statsInnerBoxSizeX,
		SCREEN_HEIGHT - (promptHeight)-(2 * padding + content) - statsInnerBoxSizeY,
		statsInnerBoxSizeX,
		statsInnerBoxSizeY };
	SDL_RenderFillRect(gRenderer, &statsInnerBox);

	//Stats labels
	int statsLabelsX = SCREEN_WIDTH - padding - statsInnerBoxSizeX + (1 * padding);
	int statsLabelsY = SCREEN_HEIGHT - (promptHeight)-(2 * padding + content) - statsInnerBoxSizeY;
	std::string statsLabelsText = mNPC->getName() + "\n" + STRING_HAPPINESS + "\n" + STRING_TRUST + "\n" + STRING_HOSTILITY;
	gFontSmall->setColor(0xFF, 0xFF, 0xFF);
	gFontSmall->renderText(statsLabelsX, statsLabelsY, statsLabelsText);

	//Stats
	renderStat(636, 270, mNPC->mHappiness, true);
	renderStat(636, 292, mNPC->mTrust, true);
	renderStat(636, 314, mNPC->mHostility, false);

	//NPC Avatar
	mNPCDialogueTexture->render(SCREEN_WIDTH - (statsInnerBoxSizeX + (2 * padding)) - mNPCDialogueTexture->getWidth(),
		SCREEN_HEIGHT - (promptHeight)-(2 * padding + content) - mNPCDialogueTexture->getHeight());

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