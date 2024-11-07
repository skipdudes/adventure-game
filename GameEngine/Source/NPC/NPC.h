#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include "../Texture.h"
#include "../Dialogue/Dialogue.h"
#include <nlohmann/json.hpp>

class Dialogue;

class NPC
{
public:
	//Create & delete
	NPC(const std::string& name, const std::string& texturePath, const std::string& dialogueTexturePath, const std::string& context);
	~NPC();

	//Second collider
	static const int DIALOGUE_COLLIDER_PADDING = 24;

	//Dialogue variables
	bool mAbleToTalk;
	bool mCurrentlyTalkingTo;
	bool mRecentlyTalkedTo;
	bool mThinking;
	std::unique_ptr<Dialogue> mDialogue;

	//Stats
	float mHappiness;
	float mAnxiety;
	float mHostility;

	//Messages array
	nlohmann::json mMessages = nlohmann::json::array();

	bool load();
	void free();
	void setPosition(int x, int y);
	void render(SDL_Rect camera);

	int getWidth() const;
	int getHeight() const;

	std::string getName() const;
	std::string getDialogueTexturePath() const;
	std::string getContext() const;

	SDL_Rect getCollider() const;
	SDL_Rect getDialogueCollider() const;

	void startDialogue();
	void endDialogue();

private:
	int mWidth;
	int mHeight;
	SDL_Rect mCollider{};
	SDL_Rect mDialogueCollider{};
	std::string mTexturePath;
	std::unique_ptr<Texture> mTexture;

	//Dialogue
	std::string mName;
	std::string mDialogueTexturePath;
	std::string mContext;
};