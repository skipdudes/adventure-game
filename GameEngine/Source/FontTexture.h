#pragma once
#include "Texture.h"

class FontTexture : public Texture
{
public:
	FontTexture(SDL_Renderer* renderer);
	~FontTexture();

	bool load(std::string filepath);
	bool create();
	void free();
	Uint32 getPitch();
	Uint32 getPixel(int x, int y);

private:
	SDL_Surface* mPixels;
};