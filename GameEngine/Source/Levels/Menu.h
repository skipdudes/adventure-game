#pragma once
#include "Level.h"
#include "../Texture.h"
#include <memory>
#include <array>

class Menu : public Level
{
public:
	static Menu* get();

	bool enter();
	bool exit();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

private:
	enum Option { START, AUTHORS, QUIT, TOTAL_OPTIONS };

	struct MenuOption
	{
		bool chosen = false;
		std::string text;
	};
	std::array<MenuOption, TOTAL_OPTIONS> mOptions;

	int mChosenOption;
	static Menu gLevelMenu;
	Menu();

	std::unique_ptr<Texture> mLevelTexture;

	void renderOption(int index, int x, int y);
};