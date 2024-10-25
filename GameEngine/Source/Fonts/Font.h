#pragma once
#include <string>
#include <memory>
#include "FontTexture.h"
#include <SDL.h>

class Font
{
public:
	Font();
	~Font();

	bool build(std::string filepath);
	void renderText(int x, int y, std::string text);
	bool setColor(Uint8 r, Uint8 g, Uint8 b);
	bool setAlpha(Uint8 a);
	void free();

private:
	static const int MAX_CHARS = 128;
	static const int CHARS_ROW = 16;
	static const int CHARS_COLUMN = 8;
	static const int PADDING = 2;

	bool mBuilt;
	std::unique_ptr<FontTexture> mFontTexture;
	SDL_Rect mChars[MAX_CHARS]{};
	int mSpace;
	int mNewLine;
};