#include "NPC.h"
#include "../Logger.h"
#include "../Constants.h"
#include "../Globals.h"

NPC::NPC(const std::string& name, const std::string& texturePath, const std::string& dialogueTexturePath, const std::string& context)
	:mWidth(0), mHeight(0), mAbleToTalk(false), mCurrentlyTalkingTo(false), mRecentlyTalkedTo(false), mThinking(false), 
	mName(name), mTexturePath(texturePath), mDialogueTexturePath(dialogueTexturePath), mContext(context), 
	mHappiness(0.5f), mTrust(0.5f), mHostility(0.5f)
{
	mTexture = std::make_unique<Texture>();
	mMessages.clear();
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

void NPC::renderDialoguePrompt(int xOffset)
{
	if(mAbleToTalk && !mCurrentlyTalkingTo && !mRecentlyTalkedTo)
		renderPrompt(xOffset, BEGIN_DIALOGUE_PROMPT + mName, 1, 0xFF, 0xFF, 0xFF);
}

void NPC::renderDialogue()
{
	if (mCurrentlyTalkingTo && !(mDialogue == nullptr))
		mDialogue->render();
}

void NPC::handleDialogueEvents(SDL_Event& e)
{
	if (mCurrentlyTalkingTo && !(mDialogue == nullptr))
		mDialogue->handleEvents(e);
}

bool NPC::startedDialogue()
{
	if (mAbleToTalk && !mCurrentlyTalkingTo)
	{
		//Set internal variables
		mCurrentlyTalkingTo = true;
		mRecentlyTalkedTo = true;
		
		return true;
	}

	return false;
}

bool NPC::endedDialogue()
{
	if (mAbleToTalk && mCurrentlyTalkingTo && !mThinking)
	{
		//Set internal variables
		mCurrentlyTalkingTo = false;

		return true;
	}

	return false;
}

void NPC::checkIfAbleToTalk()
{
	if (checkCollision(gPlayer->getCollider(), mDialogueCollider))
	{
		mAbleToTalk = true;
	}
	else
	{
		mAbleToTalk = false;
		mRecentlyTalkedTo = false;
	}
}

void NPC::updateDialogue()
{
	if (mCurrentlyTalkingTo && !(mDialogue == nullptr))
		mDialogue->update();
}