#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
//#include "ClassDemoApp.h"
#include "Entity.h"

#define PI 3.14159265359

class Bullet{
public:
	Bullet();
	void Update(float elapsed);
	void Draw();

	float x;
	float y;

	float size;
	float speed;
	float direction;

	bool visible;
};