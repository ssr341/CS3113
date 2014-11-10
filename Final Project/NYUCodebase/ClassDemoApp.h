#pragma once

#include "Bullet.h"
#include "Entity.h"
#include "MainMenu.h"
#include "Instructions.h"
#include "GameOver.h"
#include "Gameplay.h"
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

enum GameState { STATE_MAIN_MENU, STATE_INSTRUCTIONS, STATE_STAGE_ONE, STATE_STAGE_TWO, STATE_STAGE_THREE, STATE_GAME_OVER };

class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();

	void Init();
	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);

	bool ProcessEvents();
	void Render();
	void fixedUpdate();
	void UpdateAndRender();

private:

	int state;
	bool done;

	float lastFrameTicks;
	float timeLeftOver;

	MainMenu menu;
	Instructions instructions;
	//Gameplay gameplay;
	GameOver gameOver;

	Mix_Music* music;   // music that plays whole game

	SDL_Window* displayWindow;
};