#include "NPC.h"
#include "../Logger.h"

NPC::NPC()
	:mWidth(0), mHeight(0), mAbleToTalk(false), mCurrentlyTalkingTo(false), mRecentlyTalkedTo(false)
{
	mTexture = std::make_unique<Texture>();
	mName = "";
	mDialogueTexturePath = "";
}

NPC::~NPC()
{
	free();
}

int NPC::getWidth() const
{
	return mWidth;
}
int NPC::getHeight() const
{
	return mHeight;
}

void NPC::loadDialogueVariables(const std::string& name, const std::string& dialogueTexturePath)
{
	mName = name;
	mDialogueTexturePath = dialogueTexturePath;
}

bool NPC::load(const std::string& filepath)
{
	free();

	if (!(mTexture->load(filepath)))
	{
		LOG_ERROR("Could not load NPC texture");
		return false;
	}
	LOG_INFO("Successfully loaded NPC texture");

	mWidth = mTexture->getWidth();
	mHeight = mTexture->getHeight();

	mCollider.w = mWidth;
	mCollider.h = mHeight;

	mDialogueCollider.w = mWidth + 2 * DIALOGUE_COLLIDER_PADDING;
	mDialogueCollider.h = mHeight + 2 * DIALOGUE_COLLIDER_PADDING;

	return true;
}

void NPC::free()
{
	mTexture->free();
	mWidth = 0;
	mHeight = 0;
}

void NPC::setPosition(int x, int y)
{
	mCollider.x = x;
	mCollider.y = y;

	mDialogueCollider.x = x - DIALOGUE_COLLIDER_PADDING;
	mDialogueCollider.y = y - DIALOGUE_COLLIDER_PADDING;
}

void NPC::render(SDL_Rect camera)
{
	mTexture->render(mCollider.x - camera.x, mCollider.y - camera.y);
}

SDL_Rect NPC::getCollider() const
{
	return mCollider;
}

SDL_Rect NPC::getDialogueCollider() const
{
	return mDialogueCollider;
}

std::string NPC::getName() const
{
	return mName;
}

std::string NPC::getDialogueTexturePath() const
{
	return mDialogueTexturePath;
}