#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

//60 FPS (1.0/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Bullet{
public:
	Bullet();

	unsigned int textureID;

	//void Update(float elapsed);
	void fixedUpdate();
	void Draw();

	float x;
	float y;

	float size;
	float speed;
	float direction;

	float shooter; // 0 is player 1, 1 is player 2, 2 is enemies on left side, 3 is enemies on right side, 4 is enemy in midde (stage 3)
	bool visible;
};