#include "Entity.h"

Entity::Entity(){ visible = true; }

void Entity::draw(){
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
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

	glPopMatrix();
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

void Entity::buildMatrix(){
	Matrix scaleMatrix;
	scaleMatrix.identity();
	scaleMatrix.m[0][0] = width;
	scaleMatrix.m[1][1] = height;

	Matrix rotateMatrix;
	rotateMatrix.identity();
	rotateMatrix.m[0][0] = cos(rotation);
	rotateMatrix.m[0][1] = -1 * sin(rotation);
	rotateMatrix.m[1][0] = sin(rotation);
	rotateMatrix.m[1][1] = cos(rotation);

	Matrix translateMatrix;
	translateMatrix.identity();
	translateMatrix.m[0][3] = x;
	translateMatrix.m[1][3] = y;

	matrix = scaleMatrix * rotateMatrix * translateMatrix;
}

//bool Entity::collidesWith(Entity *entity){
//	float entityRight = entity->x + entity->width / 2;
//	float entityLeft = entity->x + entity->width / -2;
//	float entityTop = entity->y + entity->height / 2;
//	float entityBottom = entity->y + entity->height / -2;
//
//	float thisRight = this->x + this->width / 2;
//	float thisLeft = this->x + this->width / -2;
//	float thisTop = this->y + this->height / 2;
//	float thisBottom = this->y + this->height / -2;
//
//	if ((entityLeft <= thisRight) && (entityTop >= thisBottom) && (entityBottom <= thisTop) && (entityRight >= thisLeft)){
//		return true;
//	}
//
//	return false;
//}