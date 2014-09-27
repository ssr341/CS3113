#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Bullet.h"
#include "Entity.h"
#include "ClassDemoApp.h"
#include <string>

class gameOver{
public:
	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);
	void Render(int score);
	int Update();
private:
	unsigned int screenText;
	SDL_Window* displayWindow;
};