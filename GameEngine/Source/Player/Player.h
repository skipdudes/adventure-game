#pragma once
#include <SDL.h>
#include <memory>
#include "../Texture.h"

class Player
{
public:
	Player();
	~Player();

	static const int PLAYER_WIDTH = 40;
	static const int PLAYER_HEIGHT = 60;
	static const int PLAYER_VELOCITY = 3; //Pixels per update

	bool load();
	void free();
	void setPosition(int x, int y);

	void handleEvents(SDL_Event& e);
	void move(int levelWidth, int levelHeight);
	void render(SDL_Rect camera);

	SDL_Rect getCollider() const;

private:
	int mVelocityX;
	int mVelocityY;
	SDL_Rect mCollider;
	std::unique_ptr<Texture> mTexture;
};