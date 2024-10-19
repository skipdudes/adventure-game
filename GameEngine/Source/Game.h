#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <memory>
#include "Font.h"

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
	bool mPaused;
	bool mFullscreen;
	const unsigned int FPS = 60;
	const unsigned int MIN_FRAMETIME = 1000 / FPS;
	const unsigned int MAX_UPDATETIME = 10;

	//Data shared across gamestates
	std::unique_ptr<Font> mFontSmall;
	std::unique_ptr<Font> mFontMedium;
	std::unique_ptr<Font> mFontLarge;

	bool init();
	bool loadData();
	void handleEvents(SDL_Event& e);
	void toggleFullscreen();
	void pauseGame();
	void update(const int& timeStep);
	void render(const double& alpha);
	void run();
	void freeData();
	void free();
};