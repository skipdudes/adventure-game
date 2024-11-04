#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include "../Texture.h"
#include "../Dialogue/Dialogue.h"

class NPC
{
public:
	//Create & delete
	NPC();
	~NPC();

	//Second collider
	static const int DIALOGUE_COLLIDER_PADDING = 24;

	//Dialogue variables
	bool mAbleToTalk;
	bool mCurrentlyTalkingTo;
	bool mRecentlyTalkedTo;
	void loadDialogueVariables(const std::string& name, const std::string& dialogueTexturePath);
	std::unique_ptr<Dialogue> mDialogue;

	bool load(const std::string& filepath);
	void free();
	void setPosition(int x, int y);
	void render(SDL_Rect camera);

	int getWidth() const;
	int getHeight() const;

	std::string getName() const;
	std::string getDialogueTexturePath() const;

	SDL_Rect getCollider() const;
	SDL_Rect getDialogueCollider() const;

private:
	int mWidth;
	int mHeight;
	SDL_Rect mCollider{};
	SDL_Rect mDialogueCollider{};
	std::unique_ptr<Texture> mTexture;

	//Dialogue
	std::string mName;
	std::string mDialogueTexturePath;
};