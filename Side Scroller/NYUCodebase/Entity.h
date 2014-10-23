#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

//60 FPS (1.0/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define TILE_SIZE 0.1f
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8

class Entity {
public:
	Entity();

	void Update(float elapsed);
	void Render();

	void draw();

	bool collidesWith(Entity* entity);

	unsigned int textureID;

	float x;
	float y;

	float width;
	float height;

	float direction_x;
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

	bool enemy;
	bool visible;
	bool player;
};