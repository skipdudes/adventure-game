#include "Player.h"
#include "../Constants.h"
#include "../Logger.h"

Player::Player()
	: mVelocityX(0), mVelocityY(0)
{
	//Collider init
	mCollider.x = 0;
	mCollider.y = 0;
	mCollider.w = PLAYER_WIDTH;
	mCollider.h = PLAYER_HEIGHT;

	//Textures
	mTexture = std::make_unique<Texture>();
}

void Player::setPosition(int x, int y)
{
	mCollider.x = x;
	mCollider.y = y;
}

bool Player::load()
{
	//Deallocate previous memory assignment
	free();

	//Load player texture
	if (!mTexture->load(FILE_PLAYER_TEXTURE.string()))
	{
		LOG_ERROR("Could not load player texture");
		return false;
	}
	LOG_INFO("Successfully loaded player texture");

	return true;
}

void Player::handleEvents(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case BUTTON_MOVE_UP: mVelocityY -= PLAYER_VELOCITY; break;
		case BUTTON_MOVE_DOWN: mVelocityY += PLAYER_VELOCITY; break;
		case BUTTON_MOVE_LEFT: mVelocityX -= PLAYER_VELOCITY; break;
		case BUTTON_MOVE_RIGHT: mVelocityX += PLAYER_VELOCITY; break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case BUTTON_MOVE_UP: mVelocityY += PLAYER_VELOCITY; break;
		case BUTTON_MOVE_DOWN: mVelocityY -= PLAYER_VELOCITY; break;
		case BUTTON_MOVE_LEFT: mVelocityX += PLAYER_VELOCITY; break;
		case BUTTON_MOVE_RIGHT: mVelocityX -= PLAYER_VELOCITY; break;
		}
	}
}

void Player::move(int levelWidth, int levelHeight)
{
	//Move in X axis
	mCollider.x += mVelocityX;
	if ((mCollider.x < 0) || (mCollider.x + PLAYER_WIDTH > levelWidth))
		mCollider.x -= mVelocityX;

	//Move in Y axis
	mCollider.y += mVelocityY;
	if ((mCollider.y < 0) || (mCollider.y + PLAYER_HEIGHT > levelHeight))
		mCollider.y -= mVelocityY;
}

void Player::render(SDL_Rect camera)
{
	//Render relative to the camera
	mTexture->render(mCollider.x - camera.x, mCollider.y - camera.y);
}

SDL_Rect Player::getCollider() const
{
	return mCollider;
}

void Player::free()
{
	//Dealloc texture assigned memory
	mTexture->free();
}

Player::~Player()
{
	free();
}