#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <string>
#include <vector>

class GameOver{
public:
	GameOver();

	void Init();
	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);

	int ProcessEvents(SDL_Event* event);
	void Render(int winningPlayer);
private:

	unsigned int screenText;
};