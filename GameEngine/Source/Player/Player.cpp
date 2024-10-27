#include "Player.h"
#include "../Constants.h"
#include "../Logger.h"

Player::Player()
	: mVelocityX(0), mVelocityY(0), mCurrentSprite(nullptr), mDirection(Direction::DOWN), mCurrentSpriteFrame(0), mLastAnimationUpdateTime(0), mIsMoving(false)
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

	//Adjust sprite animations

	//Idle
	for (int i = 0; i < Direction::TOTAL; i++)
	{
		mSpritesIdle[i].x = PLAYER_WIDTH;
		mSpritesIdle[i].w = PLAYER_WIDTH;
		mSpritesIdle[i].h = PLAYER_HEIGHT;
	}
	mSpritesIdle[Direction::UP].y = 3 * PLAYER_HEIGHT;
	mSpritesIdle[Direction::DOWN].y = 0 * PLAYER_HEIGHT;
	mSpritesIdle[Direction::LEFT].y = 1 * PLAYER_HEIGHT;
	mSpritesIdle[Direction::RIGHT].y = 2 * PLAYER_HEIGHT;

	//Walk
	for (int i = 0; i < Direction::TOTAL; i++)
	{
		for (int j = 0; j < SPRITES_WALK_PER_DIRECTION; j++)
		{
			mSpritesWalk[i][j].w = PLAYER_WIDTH;
			mSpritesWalk[i][j].h = PLAYER_HEIGHT;
		}

		mSpritesWalk[i][0].x = 0 * PLAYER_WIDTH;
		mSpritesWalk[i][1].x = 1 * PLAYER_WIDTH;
		mSpritesWalk[i][2].x = 2 * PLAYER_WIDTH;
		mSpritesWalk[i][3].x = 1 * PLAYER_WIDTH;
	}

	//Walk up
	for (int i = 0; i < SPRITES_WALK_PER_DIRECTION; i++)
	{
		mSpritesWalk[Direction::UP][i].y = 3 * PLAYER_HEIGHT;
	}

	//Walk down
	for (int i = 0; i < SPRITES_WALK_PER_DIRECTION; i++)
	{
		mSpritesWalk[Direction::DOWN][i].y = 0 * PLAYER_HEIGHT;
	}

	//Walk left
	for (int i = 0; i < SPRITES_WALK_PER_DIRECTION; i++)
	{
		mSpritesWalk[Direction::LEFT][i].y = 1 * PLAYER_HEIGHT;
	}

	//Walk right
	for (int i = 0; i < SPRITES_WALK_PER_DIRECTION; i++)
	{
		mSpritesWalk[Direction::RIGHT][i].y = 2 * PLAYER_HEIGHT;
	}

	LOG_INFO("Successfully adjusted player sprites");

	return true;
}

void Player::handleEvents(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case BUTTON_MOVE_UP:
			mVelocityY -= PLAYER_VELOCITY;
			mDirection = Direction::UP;
			break;
		case BUTTON_MOVE_DOWN:
			mVelocityY += PLAYER_VELOCITY;
			mDirection = Direction::DOWN;
			break;
		case BUTTON_MOVE_LEFT:
			mVelocityX -= PLAYER_VELOCITY;
			mDirection = Direction::LEFT;
			break;
		case BUTTON_MOVE_RIGHT:
			mVelocityX += PLAYER_VELOCITY;
			mDirection = Direction::RIGHT;
			break;
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

		fixDirection();
	}
}

void Player::fixDirection()
{
	if (mVelocityY < 0)
		mDirection = Direction::UP;
	else if (mVelocityY > 0)
		mDirection = Direction::DOWN;
	else if (mVelocityX < 0)
		mDirection = Direction::LEFT;
	else if (mVelocityX > 0)
		mDirection = Direction::RIGHT;
}

void Player::move(int levelWidth, int levelHeight, const std::vector<SDL_Rect>& walls)
{
	//Store previous position
	int lastX = mCollider.x;
	int lastY = mCollider.y;

	//Move in X axis
	mCollider.x += mVelocityX;
	if (mCollider.x < 0 || mCollider.x + PLAYER_WIDTH > levelWidth || checkWallCollision(walls))
		mCollider.x = lastX;

	//Move in Y axis
	mCollider.y += mVelocityY;
	// Sprawdzenie kolizji w pionie (Y) ze œcianami lub krawêdziami poziomu
	if (mCollider.y < 0 || mCollider.y + PLAYER_HEIGHT > levelHeight || checkWallCollision(walls))
		mCollider.y = lastY;

	//Determine if actually moving
	mIsMoving = (mCollider.x != lastX || mCollider.y != lastY);
}

bool Player::checkWallCollision(const std::vector<SDL_Rect>& walls)
{
	for (const SDL_Rect& wall : walls)
	{
		if (SDL_HasIntersection(&mCollider, &wall))
		{
			return true;
		}
	}
	return false;
}

void Player::stop()
{
	mVelocityX = 0;
	mVelocityY = 0;
	mIsMoving = false;
}

void Player::updateAnimation()
{
	if (mIsMoving)
	{
		Uint32 currentTicks = SDL_GetTicks();
		if (currentTicks - mLastAnimationUpdateTime >= SPRITE_DURATION)
		{
			mCurrentSpriteFrame = (mCurrentSpriteFrame + 1) % SPRITES_WALK_PER_DIRECTION;
			mLastAnimationUpdateTime = currentTicks;
		}
		mCurrentSprite = &mSpritesWalk[mDirection][mCurrentSpriteFrame];
	}
	else
	{
		mCurrentSprite = &mSpritesIdle[mDirection];
		mCurrentSpriteFrame = 0;
	}
}

void Player::render(SDL_Rect camera)
{
	updateAnimation();

	//Render relative to the camera
	mTexture->render(mCollider.x - camera.x, mCollider.y - camera.y, mCurrentSprite);
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