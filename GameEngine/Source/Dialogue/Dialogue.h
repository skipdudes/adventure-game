#pragma once
#include <memory>
#include "../Texture.h"
#include "../NPC/NPC.h"
#include <thread>
#include <future>

class NPC;

class Dialogue
{
public:
	enum Turn
	{
		PLAYER_TURN,
		NPC_TURN
	};

	Dialogue(const std::shared_ptr<NPC>& NPC);
	~Dialogue();

	static const int MAXIMUM_INPUT = 300;

	bool load();
	void free();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

	void handleEventsPlayerTurn(SDL_Event& e);
	void handleEventsNPCTurn(SDL_Event& e);

	void changeToNPCTurn();
	void changeToPlayerTurn();

	void renderPlayerTurn(const int& padding, const int& content, const int& promptHeight);
	void renderNPCTurn(const int& padding, const int& content, const int& promptHeight);

private:
	std::shared_ptr<NPC> mNPC;
	std::unique_ptr<Texture> mPlayerDialogueTexture;
	std::unique_ptr<Texture> mNPCDialogueTexture;

	std::string mCurrentLine;
	Turn mCurrentTurn;
	bool mChangeTurn;

	std::future<std::string> generatedResponse;
};