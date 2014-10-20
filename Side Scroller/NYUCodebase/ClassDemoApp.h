#pragma once

#include "Entity.h"
#include "Bullet.h"

#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

#define MAX_BLOCKS 100
#define TILE_SIZE 0.25f
#define MAX_BULLETS 32
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8

enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };

class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();

	void Init();

	GLuint LoadTexture(const char *image_path);

	void buildLevel();
	bool readHeader(ifstream &stream);
	bool readLayerData(ifstream &stream);
	bool readEntityData(ifstream &stream);

	void drawBlocks();

	void placeEntity(string type, float placeX, float placeY);

	void shootBullet(float x, float y, float direction);

	bool ProcessEvents();
	void Render();
	void fixedUpdate();
	void UpdateAndRender();

private:

	unsigned int levelTexture;

	int mapHeight;
	int mapWidth;
	unsigned char** levelData;

	Entity player;
	vector<Entity*> entities;

	Bullet bullets[MAX_BULLETS];
	int bulletIndex;

	float gravity_y;

	bool done;
	float lastFrameTicks;
	float timeLeftOver;

	void readLevel();

	SDL_Window* displayWindow;
};