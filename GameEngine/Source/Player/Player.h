#pragma once
#include <SDL.h>
#include <memory>
#include <vector>
#include "../Texture.h"

enum Direction { UP, DOWN, LEFT, RIGHT, TOTAL };

class Player
{
public:
	Player();
	~Player();

	static const int PLAYER_WIDTH = 38;
	static const int PLAYER_HEIGHT = 60;
	static const int PLAYER_VELOCITY = 2; //Pixels per update

	static const int SPRITES_IDLE = Direction::TOTAL;
	static const int SPRITES_WALK_PER_DIRECTION = 4;

	static const int SPRITE_DURATION = 120;

	bool load();
	void free();
	void setPosition(int x, int y);

	void handleEvents(SDL_Event& e);
	void fixDirection();

	void move(int levelWidth, int levelHeight, const std::vector<SDL_Rect>& walls);
	bool checkWallCollision(const std::vector<SDL_Rect>& walls);
	void stop();
	void updateAnimation();

	void render(SDL_Rect camera);

	SDL_Rect getCollider() const;

private:
	int mVelocityX;
	int mVelocityY;
	SDL_Rect mCollider;

	SDL_Rect mSpritesIdle[SPRITES_IDLE]{};
	SDL_Rect mSpritesWalk[Direction::TOTAL][SPRITES_WALK_PER_DIRECTION]{};
	SDL_Rect* mCurrentSprite;
	Direction mDirection;

	int mCurrentSpriteFrame;
	Uint32 mLastAnimationUpdateTime;
	bool mIsMoving;

	std::unique_ptr<Texture> mTexture;
};