#pragma once

#include "Entity.h"

#include <vector>
#include <string>
#include <math.h>
#include <algorithm>


class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();

	void Init();

	GLuint LoadTexture(const char *image_path);

	bool checkCollision(Entity* entity1, Entity* entity2);

	bool ProcessEvents();
	void Render();
	void fixedUpdate();
	void UpdateAndRender();

private:
	unsigned int asteroidTexture;
	unsigned int playerTexture;

	Entity player;
	Entity asteroid1;
	Entity asteroid2;
	Entity asteroid3;
	std::vector<Entity*> entities;

	bool done;
	float lastFrameTicks;
	float timeLeftOver;

	SDL_Window* displayWindow;
};