#include "Texture.h"
#include "Logger.h"

Texture::Texture(SDL_Renderer* renderer)
	:mWidth(0), mHeight(0), mTexture(nullptr), mRenderer(renderer)
{

}

bool Texture::load(std::string filepath)
{
	//Deallocate previous memory assignment
	free();

	//Load new texture
	mTexture = IMG_LoadTexture(mRenderer, filepath.c_str());
	if (mTexture == nullptr)
	{
		LOG_ERROR("Texture file \"" + filepath + "\" could not be loaded, SDL_image Error: " + std::string(IMG_GetError()));
		return false;
	}

	//Get width and height
	if (SDL_QueryTexture(mTexture, nullptr, nullptr, &mWidth, &mHeight) < 0)
	{
		LOG_ERROR("Could not query texture \"" + filepath + "\", SDL Error: " + std::string(SDL_GetError()));
		return false;
	}

	LOG_INFO("Successfully loaded texture \"" + filepath + "\"");
	return true;
}

int Texture::getWidth() const
{
	return mWidth;
}

int Texture::getHeight() const
{
	return mHeight;
}

void Texture::render(int x, int y, SDL_Rect* srcRect)
{
	//Set initial destination
	SDL_Rect dstRect = { x, y, mWidth, mHeight };

	//If the texture will be clipped
	if (srcRect != nullptr)
	{
		dstRect.w = srcRect->w;
		dstRect.h = srcRect->h;
	}

	//Render the texture
	SDL_RenderCopy(mRenderer, mTexture, srcRect, &dstRect);
}

bool Texture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	if (SDL_SetTextureColorMod(mTexture, r, g, b) < 0)
	{
		LOG_WARNING("Could not set texture color mod, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}

	return true;
}

bool Texture::setAlpha(Uint8 a)
{
	if (SDL_SetTextureAlphaMod(mTexture, a) < 0)
	{
		LOG_WARNING("Could not set texture alpha mod, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}

	return true;
}

void Texture::free()
{
	//If the texture exists
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

Texture::~Texture()
{
	//Free the memory
	free();
}