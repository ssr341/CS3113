#pragma once

#include <SDL.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Bullet.h"
#include "Entity.h"
#include <string>
#include <vector>
#include <string>

#define MAX_BULLETS 32

class Gameplay{
public:
	Gameplay();

	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);

	void Init();
	bool ProcessEvents(SDL_Event* event);
	int Render();
	int Update(float elasped);

	void shootBullet(float x, float y, float direction, bool shooter);
private:
	Bullet bullets[MAX_BULLETS];
	int bulletIndex;

	float enemyShot; // time between enemy shots

	int state; // state of gameplay

	unsigned int spriteSheetTexture;
	unsigned int screenText;

	Entity playerSprite;
	std::vector<std::vector<Entity>> enemies;

	int score;         // score of the player

	// rows and columns for enemies
	int num_rows;
	int num_columns;

	bool done;
	float lastFrameTicks;

	//SDL_Window* displayWindow;
};