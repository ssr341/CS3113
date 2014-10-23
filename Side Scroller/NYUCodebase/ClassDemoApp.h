#pragma once

#include "Entity.h"

#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

#define WORLD_OFFSET_X 0
#define WORLD_OFFSET_Y 0

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
	void entityDraw(Entity* entity);

	void placeEntity(string type, float placeX, float placeY);

	void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);
	void doLevelCollisionY(Entity* entity);
	float checkPointForGridCollisionY(float x, float y);
	void doLevelCollisionX(Entity* entity);
	float checkPointForGridCollisionX(float x, float y);

	bool isSolid(unsigned char index);

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

	float gravity_y;

	bool done;
	float lastFrameTicks;
	float timeLeftOver;

	// respawn location
	float spawnLocX;
	float spawnLocY;

	void readLevel();

	SDL_Window* displayWindow;
};