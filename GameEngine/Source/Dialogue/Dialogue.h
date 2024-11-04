#pragma once
#include <memory>
#include "../Texture.h"

class Dialogue
{
public:
	enum Turn
	{
		PLAYER_TURN,
		NPC_TURN
	};

	Dialogue(const std::string& NPCName, const std::string& NPCDialogueTexturePath);
	~Dialogue();

	static const int MAXIMUM_INPUT = 300;

	bool load();
	void free();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

	void handleEventsPlayerTurn(SDL_Event& e);
	void handleEventsNPCTurn(SDL_Event& e);
	
	void updatePlayerTurn();
	void updateNPCTurn();

	void renderPlayerTurn(const int& padding, const int& content, const int& promptHeight);
	void renderNPCTurn(const int& padding, const int& content, const int& promptHeight);

private:
	std::string mNPCName;
	std::string mNPCDialogueTexturePath;
	std::unique_ptr<Texture> mPlayerDialogueTexture;
	std::unique_ptr<Texture> mNPCDialogueTexture;

	std::string mCurrentLine;
	Turn mCurrentTurn;
	bool mChangeTurn;
};