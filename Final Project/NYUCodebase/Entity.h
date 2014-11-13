#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Bullet.h"

class Entity{
public:
	Entity();

	void draw();

	void fixedUpdate();
	bool collidesWith(Bullet bullet);

	float x;
	float y;

	unsigned int textureID;
	float width;
	float height;

	bool visible;
	float velocity_y;
	float acceleration_y;
	float friction_y;
};