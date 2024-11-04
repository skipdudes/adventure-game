#pragma once
#include <memory>
#include "../Texture.h"

class Dialogue
{
public:
	Dialogue(const std::string& NPCName, const std::string& NPCDialogueTexturePath);
	~Dialogue();

	bool load();
	void free();

	void handleEvents(SDL_Event& e);
	void update();
	void render(SDL_Rect camera);

private:
	std::string mNPCName;
	std::string mNPCDialogueTexturePath;
	std::unique_ptr<Texture> mPlayerDialogueTexture;
	std::unique_ptr<Texture> mNPCDialogueTexture;
};