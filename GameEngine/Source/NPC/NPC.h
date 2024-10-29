#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include "../Texture.h"

class NPC
{
public:
	NPC();
	~NPC();

	static const int DIALOGUE_COLLIDER_PADDING = 24;
	bool mAbleToTalk;

	bool load(const std::string& filepath);
	void free();
	void setPosition(int x, int y);
	void render(SDL_Rect camera);

	int getWidth() const;
	int getHeight() const;

	SDL_Rect getCollider() const;
	SDL_Rect getDialogueCollider() const;

private:
	int mWidth;
	int mHeight;
	SDL_Rect mCollider{};
	SDL_Rect mDialogueCollider{};
	std::unique_ptr<Texture> mTexture;
};