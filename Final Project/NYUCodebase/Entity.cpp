#include "Entity.h"

Entity::Entity(){ visible = true; }

// draw the entity based on texture loaded and position
void Entity::draw(){
	if (!visible)
		return;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	if (rotate)
		glRotatef(rotation, 0.0, 0.0, 1.0);
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

// draw the blended fire when player hit twice
void Entity::fireDraw(){
	if (!visible)
		return;
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

// linear interpolation
float lerp(float v0, float v1, float t){
	return (1.0f - t) * v0 + t * v1;
}

// update position, velocity and rotation (if used)
void Entity::fixedUpdate(){
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
	velocity_y += acceleration_y * FIXED_TIMESTEP;
	y += velocity_y * FIXED_TIMESTEP;
	rotation += FIXED_TIMESTEP * 20.0f;
}

// check for collision
bool Entity::collidesWith(Bullet bullet){
	// hitbox of bullet
	float bulletRight = bullet.x + bullet.size / 2;
	float bulletLeft = bullet.x + bullet.size / -2;
	float bulletTop = bullet.y + bullet.size / 2;
	float bulletBottom = bullet.y + bullet.size / -2;

	// hit box of this entity
	float thisRight = this->x + this->width / 2;
	float thisLeft = this->x + this->width / -2;
	float thisTop = this->y + this->height / 2;
	float thisBottom = this->y + this->height / -2;

	if ((bulletLeft <= thisRight) && (bulletTop >= thisBottom) && (bulletBottom <= thisTop) && (bulletRight >= thisLeft)){
		return true;
	}

	return false;
}