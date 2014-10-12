#pragma once

#include "Entity.h"

#include <vector>
#include <string>
#include <math.h>

#define MAX_BLOCKS 100

enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };

class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();

	void Init();

	GLuint LoadTexture(const char *image_path);

	void buildLevel();

	bool ProcessEvents();
	void Render();
	void fixedUpdate();
	void UpdateAndRender();

private:
	/*unsigned int blockTexture;
	unsigned int playerTexture;*/

	Entity player;
	Entity blocks[MAX_BLOCKS];
	Entity key;
	Entity float1;
	Entity float2;
	Entity float3;
	Entity float4;
	Entity float5;
	Entity keyhole;
	std::vector<Entity*> entities;

	float gravity_y;

	bool done;
	float lastFrameTicks;
	float timeLeftOver;

	SDL_Window* displayWindow;
};