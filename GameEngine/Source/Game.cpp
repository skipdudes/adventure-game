#include "Game.h"
#include "Logger.h"

Game::Game(std::string title, int width, int height)
	:mWidth(width), mHeight(height), mTitle(title), mWindow(nullptr), mRenderer(nullptr), mRunning(true), mPaused(false), mFullscreen(false)
{
	//Initialize the game
	if (!init())
	{
		LOG_ERROR("Failed to initialize the game");
		mRunning = false;
	}

	//Load the data
	if (!loadData())
	{
		LOG_ERROR("Failed to load the data");
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
	if (SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF) < 0)
		LOG_WARNING("Could not set renderer draw color, SDL Error: " + std::string(SDL_GetError()));
	if (SDL_RenderSetLogicalSize(mRenderer, mWidth, mHeight) < 0)
		LOG_WARNING("Could not set renderer logical size, SDL Error: " + std::string(SDL_GetError()));

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

bool Game::loadData()
{
	LOG_INFO("Starting loading data");

	//Load global data

	LOG_INFO("Finished loading data");
	return true;
}

void Game::handleEvents(SDL_Event& e)
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
			mRunning = false;
		else if (e.type == SDL_KEYDOWN)
		{
			LOG_INFO("Pressed " + std::string(SDL_GetKeyName(e.key.keysym.sym)));

			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				pauseGame();
				break;
			case SDLK_F4:
				toggleFullscreen();
				break;
			}
		}
	}
}

void Game::toggleFullscreen()
{
	//Store mouse position
	int mouseX, mouseY;
	SDL_GetGlobalMouseState(&mouseX, &mouseY);

	//Change display mode
	mFullscreen = !mFullscreen;
	if (!mFullscreen)
	{
		LOG_INFO("Setting display mode to windowed");
		if (SDL_SetWindowFullscreen(mWindow, 0) < 0)
			LOG_WARNING("Could not set display to windowed mode, SDL Error: " + std::string(SDL_GetError()));
	}
	else
	{
		LOG_INFO("Setting display mode to fullscreen");
		if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
			LOG_WARNING("Could not set display to fullscreen mode, SDL Error: " + std::string(SDL_GetError()));
	}

	//Recover mouse position
	if (SDL_WarpMouseGlobal(mouseX, mouseY) < 0)
		LOG_WARNING("Mouse position could not be restored, SDL Error: " + std::string(SDL_GetError()));
}

void Game::pauseGame()
{
	mPaused = !mPaused;
	if (!mPaused)
	{
		LOG_INFO("Unpausing the game");
	}
	else
	{
		LOG_INFO("Pausing the game");
	}
}

void Game::update(const int& timeStep)
{
	//Update game logic based on timeStep
}

void Game::render(const double& alpha)
{
	SDL_RenderClear(mRenderer);

	//Render textures based on alpha

	SDL_RenderPresent(mRenderer);
}

void Game::run()
{
	LOG_INFO("Starting the game loop");

	SDL_Event e;
	Uint32 lastFrameStartTime = SDL_GetTicks();
	Uint32 timeAccumulator = 0;

	while (mRunning)
	{
		//Calculate the time between the frames
		Uint32 currentFrameStartTime = SDL_GetTicks();
		Uint32 elapsedFrameTime = currentFrameStartTime - lastFrameStartTime;
		lastFrameStartTime = currentFrameStartTime;
		timeAccumulator += elapsedFrameTime;

		//Handle user input
		handleEvents(e);

		while (timeAccumulator >= MAX_UPDATETIME)
		{
			//Update with fixed time step
			update(MAX_UPDATETIME);
			timeAccumulator -= MAX_UPDATETIME;
		}

		//Render with extrapolation
		render(timeAccumulator / (double)MAX_UPDATETIME);

		//Cap the framerate
		Uint32 frameTime = SDL_GetTicks() - currentFrameStartTime;
		if (frameTime < MIN_FRAMETIME)
			SDL_Delay(MIN_FRAMETIME - frameTime);
	}

	LOG_INFO("Finished the game loop");
}

void Game::freeData()
{
	LOG_INFO("Starting freeing data");

	//Free the data

	LOG_INFO("Finished freeing data");
}

void Game::free()
{
	LOG_INFO("Starting game teardown");

	//Free the memory
	freeData();

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
	free();
}