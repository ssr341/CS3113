#pragma once

#include "Bullet.h"
#include "Entity.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "Gameplay.h"
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

enum GameState { STATE_MAIN_MENU, STATE_INSTRUCTIONS, STATE_STAGE_SELECT, STATE_GAME_LEVEL, STATE_GAME_OVER };

class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();

	void Init();
	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);

	bool ProcessEvents();
	void Render();
	//void Update(float elapsed);
	bool UpdateAndRender();

private:

	int state;
	bool done;

	float lastFrameTicks;

	MainMenu menu;
	//Instructions instruction;
	//Gameplay gameplay;
	GameOver gameOver;

	SDL_Window* displayWindow;
};