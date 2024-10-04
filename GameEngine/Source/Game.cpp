#include "Game.h"
#include "Logger.h"

Game::Game(std::string title, int width, int height)
	:mWidth(width), mHeight(height), mTitle(title), mWindow(nullptr), mRenderer(nullptr), mRunning(true), mFullscreen(true)
{
	//Initialize the game
	if (!init())
	{
		LOG_ERROR("Failed to initialize the game");
		mRunning = false;
	}

	//Load the media
	if (!loadMedia())
	{
		LOG_ERROR("Failed to load the media");
		mRunning = false;
	}

	//Run the game loop
	run();
}

bool Game::init()
{
	LOG_INFO("Starting game initialization");

	//SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		LOG_ERROR("SDL could not initialize, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}

	//Window
	mWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_SHOWN);
	if (mWindow == nullptr)
	{
		LOG_ERROR("Window could not be created, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}

	//Renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == nullptr)
	{
		LOG_ERROR("Renderer could not be created, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderSetLogicalSize(mRenderer, mWidth, mHeight);

	//SDL_image
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		LOG_ERROR("SDL_image could not initialize, SDL_image Error: " + std::string(IMG_GetError()));
		return false;
	}

	//SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG_ERROR("SDL_mixer could not initialize, SDL_mixer Error: " + std::string(Mix_GetError()));
		return false;
	}

	LOG_INFO("Finished game initialization");
	return true;
}

bool Game::loadMedia()
{
	LOG_INFO("Starting loading media");

	// load media

	LOG_INFO("Finished loading media");
	return true;
}

void Game::run()
{
	LOG_INFO("Starting the game loop");

	SDL_Event e;
	while (mRunning)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				mRunning = false;
		}
		SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(mRenderer);
		SDL_RenderPresent(mRenderer);
	}

	LOG_INFO("Finished the game loop");
}

void Game::teardown()
{
	LOG_INFO("Starting game teardown");

	//Window	
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	//SDL
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	LOG_INFO("Finished game teardown");
}

Game::~Game()
{
	//Free the memory and close subsystems
	teardown();
}