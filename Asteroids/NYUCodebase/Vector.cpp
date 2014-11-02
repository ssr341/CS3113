#include "Vector.h"

Vector::Vector(){ x = 0; y = 0; z = 0; }

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector::length(){
	return sqrt(x*x + y*y);
}

void Vector::normalize(){
	x /= length();
	y /= length();
}