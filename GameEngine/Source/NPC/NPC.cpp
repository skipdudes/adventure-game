#include "NPC.h"
#include "../Logger.h"
#include "../Constants.h"

NPC::NPC(const std::string& name, const std::string& texturePath, const std::string& dialogueTexturePath, const std::string& context)
	:mWidth(0), mHeight(0), mAbleToTalk(false), mCurrentlyTalkingTo(false), mRecentlyTalkedTo(false), mThinking(false), 
	mName(name), mTexturePath(texturePath), mDialogueTexturePath(dialogueTexturePath), 
	mHappiness(0.5f), mAnxiety(0.5f), mHostility(0.5f)
{
	mTexture = std::make_unique<Texture>();
	mMessages.clear();
	mContext = context + STRING_NPC_BASE_CONTEXT;
}

NPC::~NPC()
{
	//Dealloc texture
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

bool NPC::load()
{
	//Free tex
	free();

	//Load tex
	if (!(mTexture->load(mTexturePath)))
	{
		LOG_ERROR("Could not load NPC texture");
		return false;
	}
	LOG_INFO("Successfully loaded NPC texture");

	mWidth = mTexture->getWidth();
	mHeight = mTexture->getHeight();

	//Init colliders
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

std::string NPC::getContext() const
{
	return mContext;
}

void NPC::startDialogue()
{
	
}

void NPC::endDialogue()
{
	
}