#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Bullet.h"

class Entity{
public:
	Entity();

	void draw();
	void fireDraw();

	void fixedUpdate();
	bool collidesWith(Bullet bullet);

	float x;
	float y;

	unsigned int textureID;
	float width;
	float height;

	bool rotate;
	float rotation;
	bool visible;
	float deadTime;

	float velocity_y;
	float acceleration_y;
	float friction_y;
};