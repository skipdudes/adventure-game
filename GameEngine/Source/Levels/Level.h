#pragma once
#include <SDL.h>

class Level
{
public:
	virtual bool enter() = 0;
	virtual bool exit() = 0;

	virtual void handleEvents(SDL_Event& e) = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	virtual ~Level() {};
};