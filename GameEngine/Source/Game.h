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
	bool mPaused;
	bool mFullscreen;
	const int FPS = 60;
	const int MIN_FRAMETIME = 1000 / FPS;
	const int MAX_UPDATETIME = 10;

	bool init();
	bool loadData();
	void handleEvents(SDL_Event& e);
	void pauseGame();
	void toggleFullscreen();
	void update(const int& timeStep);
	void render(const double& alpha);
	void run();
	void freeData();
	void free();
};