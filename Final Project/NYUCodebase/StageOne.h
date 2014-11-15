#pragma once

#include <SDL.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Bullet.h"
#include "Entity.h"
#include <string>
#include <vector>

#define MAX_BULLETS 32

class StageOne{
public:
	StageOne();
	~StageOne();

	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);

	void Init();
	void ProcessShoot(SDL_Event* event);
	void ProcessEvents();
	void Render();
	int fixedUpdate(float fixedElapsed);

	void shootBullet(float x, float y, float direction, float shooter, float size, float speed);

	Mix_Chunk* explosionSound; // sound when bullets collide
	Mix_Chunk* shootingSound; // sound when bullets shot
private:
	Bullet bullets[MAX_BULLETS];
	int bulletIndex;

	int enemyNum; // number of enemies
	float enemyShot; // time between enemy shots
	float respawnTimer; // time for enemy respawns

	int winner; // winner of stage. 0 means gameplay still happening, 1 means player 1, 2 means player 2

	Entity player1;
	Entity player2;
	std::vector<Entity*> enemies1;
	std::vector<Entity*> enemies2;

	// powerups
	float player1Shot; // current time between shots
	float player2Shot; // current time between shots
	float player1ShotTime; // time before another shot can be fired
	float player2ShotTime; // time before another shot can be fired
	int player1KillCount;
	int player2KillCount;
	float player1BulletSize;
	float player2BulletSize;
	float player1BulletSpeed;
	float player2BulletSpeed;

	float enemyBulletSize;
	float enemyBulletSpeed;

	float lastFrameTicks;

};