#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool load(std::string filepath);
	void free();

	void render(int x, int y, SDL_Rect* srcRect = nullptr);
	bool setColor(Uint8 r, Uint8 g, Uint8 b);
	bool setAlpha(Uint8 a);

	int getWidth() const;
	int getHeight() const;

protected:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};