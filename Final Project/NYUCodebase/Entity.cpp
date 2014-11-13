#include "Entity.h"

Entity::Entity(){ visible = true; }

void Entity::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glScalef(width, height, 0.0);
	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

// linear interpolation
float lerp(float v0, float v1, float t){
	return (1.0 - t) * v0 + t * v1;
}

void Entity::fixedUpdate(){
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);

	velocity_y += acceleration_y * FIXED_TIMESTEP;

	y += velocity_y * FIXED_TIMESTEP;
}

bool Entity::collidesWith(Bullet bullet){
	float bulletRight = bullet.x + bullet.size / 2;
	float bulletLeft = bullet.x + bullet.size / -2;
	float bulletTop = bullet.y + bullet.size / 2;
	float bulletBottom = bullet.y + bullet.size / -2;

	float thisRight = this->x + this->width / 2;
	float thisLeft = this->x + this->width / -2;
	float thisTop = this->y + this->height / 2;
	float thisBottom = this->y + this->height / -2;

	if ((bulletLeft <= thisRight) && (bulletTop >= thisBottom) && (bulletBottom <= thisTop) && (bulletRight >= thisLeft)){
		return true;
	}

	return false;
}