#include "Font.h"
#include "../Logger.h"

Font::Font()
	:mBuilt(false), mSpace(0), mNewLine(0)
{
	mFontTexture = std::make_unique<FontTexture>();
}

bool Font::build(std::string filepath)
{
	//Deallocate previous memory assignment
	free();

	//Load pixels from the font texture
	if (!mFontTexture->load(filepath))
	{
		LOG_ERROR("Could not load font texture");
		return false;
	}

	//Set font values
	Uint32 backgroundColor = mFontTexture->getPixel(0, 0);
	int charWidth = mFontTexture->getWidth() / CHARS_ROW;
	int charHeight = mFontTexture->getHeight() / CHARS_COLUMN;
	int charTop = charHeight;

	int currentChar = 0;
	for (int row = 0; row < CHARS_COLUMN; row++)
	{
		for (int col = 0; col < CHARS_ROW; col++)
		{
			//Initial char SDL_Rect values
			mChars[currentChar].x = charWidth * col;
			mChars[currentChar].y = charHeight * row;
			mChars[currentChar].w = charWidth;
			mChars[currentChar].h = charHeight;

			//Find and set char left side
			for (int charX = 0; charX < charWidth; charX++)
			{
				for (int charY = 0; charY < charHeight; charY++)
				{
					int charGlobalX = (charWidth * col) + charX;
					int charGlobalY = (charHeight * row) + charY;

					if (mFontTexture->getPixel(charGlobalX, charGlobalY) != backgroundColor)
					{
						mChars[currentChar].x = charGlobalX;
						goto found_left_side;
					}
				}
			}
			found_left_side:

			//Find and set char right side
			for (int charX = charWidth - 1; charX >= 0; charX--)
			{
				for (int charY = 0; charY < charHeight; charY++)
				{
					int charGlobalX = (charWidth * col) + charX;
					int charGlobalY = (charHeight * row) + charY;

					if (mFontTexture->getPixel(charGlobalX, charGlobalY) != backgroundColor)
					{
						mChars[currentChar].w = (charGlobalX - mChars[currentChar].x) + 1;
						goto found_right_side;
					}
				}
			}
			found_right_side:

			//Find tallest char
			for (int charY = 0; charY < charHeight; charY++)
			{
				for (int charX = 0; charX < charWidth; charX++)
				{
					int charGlobalX = (charWidth * col) + charX;
					int charGlobalY = (charHeight * row) + charY;

					if (mFontTexture->getPixel(charGlobalX, charGlobalY) != backgroundColor)
					{
						//If taller than current tallest char
						if (charY < charTop)
							charTop = charY;

						goto found_top;
					}
				}
			}
			found_top:
			currentChar++;
		}
	}

	mSpace = (charWidth - PADDING) / 3;
	mNewLine = charHeight - (3 * PADDING);

	//Remove extra space on top of chars based on charTop
	for (int i = 0; i < MAX_CHARS; i++)
	{
		mChars[i].y += charTop;
		mChars[i].h -= charTop;
	}

	//Create font texture
	if (!mFontTexture->create())
	{
		LOG_ERROR("Could not create font texture");
		return false;
	}
	
	LOG_INFO("Successfully built font \"" + filepath + "\"");
	mBuilt = true;
	return true;
}

void Font::renderText(int x, int y, std::string text)
{
	//If the font was built
	if (mBuilt)
	{
		int charX = x;
		int charY = y;

		for (int i = 0; i < text.length(); i++)
		{
			//Space
			if (text[i] == ' ')
			{
				charX += mSpace;
			}
			//Newline
			else if (text[i] == '\n')
			{
				charY += mNewLine;
				
				//Return to the beginning
				charX = x;
			}
			//Other chars
			else
			{
				mFontTexture->render(charX, charY, &mChars[int(text[i])]);

				//Apply padding
				charX += mChars[int(text[i])].w + PADDING;
			}
		}
	}
}

bool Font::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	if (!mFontTexture->setColor(r, g, b))
	{
		LOG_WARNING("Could not set font color");
		return false;
	}

	return true;
}

bool Font::setAlpha(Uint8 a)
{
	if (!mFontTexture->setAlpha(a))
	{
		LOG_WARNING("Could not set font alpha");
		return false;
	}

	return true;
}

void Font::free()
{
	//Dealloc texture assigned memory
	mFontTexture->free();

	//If the font was build
	if (mBuilt)
	{
		int mSpace = 0;
		int mNewLine = 0;
		mBuilt = false;
	}
}

Font::~Font()
{
	//Free the memory
	free();
}