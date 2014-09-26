#pragma once

#define MAX_BULLETS 32
#define PI 3.14159265359

#include "Bullet.h"
#include <vector>

class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();
	
	void Init();

	bool UpdateAndRender();

	void shootBullet();

	void Render();
	void Update(float elapsed);

private:

	//std::vector<Bullet> bullets;
	Bullet bullets[MAX_BULLETS];
	int bulletIndex;

	unsigned int spriteSheetTexture;

	bool done;
	float lastFrameTicks;

	SDL_Window* displayWindow;
};