#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Bullet.h"
#include "Entity.h"
#include <string>
#include <vector>

class MainMenu{
public:
	MainMenu();

	void Init();
	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);
	
	bool ProcessEvents();
	void Render();
	int Update();
private:
	bool done = false;
	unsigned int screenText;
	//SDL_Window* displayWindow;
};