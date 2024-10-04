#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

class Game
{	
public:
	Game(std::string title, int width, int height);
	~Game();

private:
	int mWidth;
	int mHeight;
	std::string mTitle;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mRunning;
	bool mFullscreen;
	//const int SCREEN_FPS = 60;
	//const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

	bool init();
	bool loadMedia();
	void run();
	void teardown();
};