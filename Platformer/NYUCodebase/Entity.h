#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

//60 FPS (1.0/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

//#include "SheetSprite.h"

class Entity {
public:
	Entity();

	void Update(float elapsed);
	void Render();

	void draw();

	bool collidesWith(Entity* entity);

	unsigned int textureID;

	//SheetSprite sprite;
	float x;
	float y;

	float width;
	float height;
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float friction_x;
	float friction_y;
	float mass;
	bool isStatic;
	bool enableCollisions;

	void fixedUpdate();

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

	bool key;
	bool keyhole;
	bool player;
	bool visible;
};