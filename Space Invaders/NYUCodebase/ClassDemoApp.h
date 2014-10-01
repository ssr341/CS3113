#pragma once

#include "Bullet.h"
#include "Entity.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "Gameplay.h"
#include <vector>
#include <string>

enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };

class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();
	
	void Init();
	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);

	bool ProcessEvents();
	void Render();
	void Update(float elapsed);
	bool UpdateAndRender();

private:

	int state = 0;
	int score;
	bool done;

	float lastFrameTicks;

	MainMenu menu;
	Gameplay gameplay;
	GameOver gameOver;

	SDL_Window* displayWindow;
};