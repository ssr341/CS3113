#pragma once

#include "ClassDemoApp.h"
#include "Bullet.h"

class Entity{
public:
	Entity();
	Entity(unsigned int textureID, float u, float v, float width, float height);

	void Draw(float scale);

	float scale;

	float x;
	float y;

	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;

	float visible = true;
	float speed;
	float direction_x;
	float direction_y;
};