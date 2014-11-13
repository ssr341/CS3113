//NEEDS FIXED UPDATE
#include "Bullet.h"

Bullet::Bullet(){}

//void Bullet::Update(float elapsed){
//	y += elapsed * speed * direction;
//	// if bullet is off screen make it invisible to be removed
//	if (x < -1.33 || x > 1.33)
//		visible = false;
//}

// linear interpolation
//float lerp(float v0, float v1, float t){
//	return (1.0 - t) * v0 + t * v1;
//}

void Bullet::fixedUpdate(){
	/*velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);

	velocity_y += acceleration_y * FIXED_TIMESTEP;

	y += velocity_y * FIXED_TIMESTEP;*/
	x += direction * speed * FIXED_TIMESTEP;
}

void Bullet::Draw(){
	// only draw bullets on screen that haven't collided
	if (!visible)
		return;

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glScalef(size, size, 0.0f);
	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	/*GLfloat quad[] = { -size, size, -size, -size, size, -size, size, size };*/
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
}