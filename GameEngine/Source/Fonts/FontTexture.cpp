#include "FontTexture.h"
#include "../Globals.h"
#include "../Logger.h"

FontTexture::FontTexture()
	:Texture(), mPixels(nullptr)
{

}

bool FontTexture::load(std::string filepath)
{
	//Deallocate previous memory assignment
	free();

	//Load pixels
	mPixels = IMG_Load(filepath.c_str());
	if (mPixels == nullptr)
	{
		LOG_ERROR("Font texture file \"" + filepath + "\" could not be loaded, SDL_image Error: " + std::string(IMG_GetError()));
		return false;
	}

	//Get width and height
	mWidth = mPixels->w;
	mHeight = mPixels->h;

	LOG_INFO("Successfully loaded font texture \"" + filepath + "\"");
	return true;
}

Uint32 FontTexture::getPitch()
{
	Uint32 pitch = 0;

	if (mPixels != nullptr)
	{
		//4 bytes per pixel
		pitch = (mPixels->pitch) / 4;
	}

	return pitch;
}

Uint32 FontTexture::getPixel(int x, int y)
{
	//Convert pixels to int
	Uint32* pixels = static_cast<Uint32*>(mPixels->pixels);

	//Y offset * pitch + X offset
	return pixels[(y * getPitch()) + x];
}

bool FontTexture::create()
{
	//Check if pixels were loaded
	if (mPixels == nullptr)
	{
		LOG_ERROR("Cannot create texture from no loaded pixels");
		return false;
	}

	//Create texture from loaded pixels
	mTexture = SDL_CreateTextureFromSurface(gRenderer, mPixels);
	if (mTexture == nullptr)
	{
		LOG_ERROR("Could not creature texture from loaded pixels, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}

	//Free loaded pixels
	SDL_FreeSurface(mPixels);
	mPixels = nullptr;

	LOG_INFO("Successfully created font texture from pixels");
	return true;
}

void FontTexture::free()
{
	//Call the parent class dealloc
	Texture::free();

	//If the pixels were loaded
	if (mPixels != nullptr)
	{
		SDL_FreeSurface(mPixels);
		mPixels = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

FontTexture::~FontTexture()
{
	//Free the memory
	free();
}