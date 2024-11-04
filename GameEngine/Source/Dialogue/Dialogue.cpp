#include "Dialogue.h"
#include "../Constants.h"
#include "../Globals.h"
#include "../Logger.h"

Dialogue::Dialogue(const std::string& NPCName, const std::string& NPCDialogueTexturePath)
{
	LOG_INFO("Called Dialogue constructor");

	mNPCName = NPCName;
	mNPCDialogueTexturePath = NPCDialogueTexturePath;

	mPlayerDialogueTexture = std::make_unique<Texture>();
	mNPCDialogueTexture = std::make_unique<Texture>();
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

void Dialogue::free()
{
	mPlayerDialogueTexture->free();
	mNPCDialogueTexture->free();
}

void Dialogue::handleEvents(SDL_Event& e)
{
	if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_w))
	{
		LOG_INFO("dialogueeeeeee w");
	}
}

void Dialogue::update()
{
	
}

void Dialogue::render(SDL_Rect camera)
{
	int linesNumber = 4;

	int padding = 2;
	int content = 32 * linesNumber;

	//Outer box
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect outerBox = { 0, SCREEN_HEIGHT - (2 * padding + content), SCREEN_WIDTH, (2 * padding + content) };
	SDL_RenderFillRect(gRenderer, &outerBox);

	//Inner box
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_Rect innerBox = { padding, SCREEN_HEIGHT - (padding + content), SCREEN_WIDTH - (2 * padding), content };
	SDL_RenderFillRect(gRenderer, &innerBox);

	//Prompt
	gFontSmall->setColor(0xFF, 0xFF, 0xFF);
	std::string text = "This is a message from the king\nThis is a message from the king\nThis is a message from the king\nThis is a message from the king";
	gFontSmall->renderText(6, SCREEN_HEIGHT - (content - 2 * padding), text);

	//Avatars
	mPlayerDialogueTexture->render(0 - camera.x, SCREEN_HEIGHT - (2 * padding + content) - mPlayerDialogueTexture->getHeight() - camera.y);
	mNPCDialogueTexture->render(SCREEN_WIDTH - mNPCDialogueTexture->getWidth() - camera.x, SCREEN_HEIGHT - (2 * padding + content) - mNPCDialogueTexture->getHeight() - camera.y);
}

Dialogue::~Dialogue()
{
	LOG_INFO("Called Dialogue deconstructor");
	//Free the memory
	free();
}