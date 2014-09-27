#pragma once

#define MAX_BULLETS 32
#define PI 3.14159265359

#include "Bullet.h"
#include "Entity.h"
#include "mainMenu.h"
#include "gameOver.h"
#include <vector>

class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();
	
	void Init();
	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);

	bool ProcessEvents();

	void shootBullet(float x, float y);

	void Render();
	void Update(float elapsed);
	bool UpdateAndRender();

private:

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int state = 0;

	mainMenu menu;
	gameOver gameOver;

	Bullet bullets[MAX_BULLETS];
	int bulletIndex;

	unsigned int spriteSheetTexture;
	unsigned int screenText;

	Entity playerSprite;
	std::vector<std::vector<Entity>> enemies;

	int score = 0;         // score of the player

	// rows and columns for enemies
	int num_rows = 4;
	int num_columns = 10;

	bool done;
	float lastFrameTicks;

	SDL_Window* displayWindow;
};