#include "Bullet.h"
#include <math.h>

Bullet::Bullet(){}

void Bullet::Update(float elapsed){
	y += elapsed * speed * direction;
	// if bullet is off screen make it invisible to be removed
	if (y > 1.0)
		visible = false;
}

void Bullet::Draw(){
	// only draw bullets on screen that haven't collided
	if (!visible)
		return;

	glLoadIdentity();
	glTranslatef(x, y, 0.0);

	GLfloat quad[] = { -size, size, -size, -size, size, -size, size, size };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
}