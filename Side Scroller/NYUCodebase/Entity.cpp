#include "Entity.h"

Entity::Entity(){ visible = true; }

void Entity::draw(){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	glLoadIdentity();

	int index = 0;
	if (enemy)
		index = 78;
	else if (player)
		index = 81;

	float u = (float)(((int)index) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
	float v = (float)(((int)index) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
	float spriteWidth = 1.0 / (float)SPRITE_COUNT_X;
	float spriteHeight = 1.0 / (float)SPRITE_COUNT_Y;

	GLfloat quad[] = { TILE_SIZE * x, -TILE_SIZE * y, TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
		(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE, (TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat uvs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPopMatrix();
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

// linear interpolation
float lerp(float v0, float v1, float t){
	return (1.0 - t) * v0 + t * v1;
}

void Entity::fixedUpdate(){
	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);

	velocity_x += acceleration_x * FIXED_TIMESTEP;
	velocity_y += acceleration_y * FIXED_TIMESTEP;

	x += velocity_x * FIXED_TIMESTEP;
	y += velocity_y * FIXED_TIMESTEP;
}

bool Entity::collidesWith(Entity *entity){
	float entityRight = entity->x + entity->width / 2;
	float entityLeft = entity->x + entity->width / -2;
	float entityTop = entity->y + entity->height / 2;
	float entityBottom = entity->y + entity->height / -2;

	float thisRight = this->x + this->width / 2;
	float thisLeft = this->x + this->width / -2;
	float thisTop = this->y + this->height / 2;
	float thisBottom = this->y + this->height / -2;

	if ((entityLeft <= thisRight) && (entityTop >= thisBottom) && (entityBottom <= thisTop) && (entityRight >= thisLeft)){
		return true;
	}

	return false;
}