
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "ClassDemoApp.h"
#include "Entity.h"
#include "Bullet.h"
using namespace std;

int main(int argc, char *argv[])
{
	ClassDemoApp app;
	while (!app.ProcessEvents()){}
	

	//while (ProcessEvents()) {

	//	float ticks = (float)SDL_GetTicks() / 1000.0f;
	//	float elapsed = ticks - lastFrameTicks;
	//	lastFrameTicks = ticks;
	//	
	//	update(elapsed);
	//	render(screenText);

	//	// start screen text
	//	/*glLoadIdentity();
	//	glTranslatef(-0.25, 0.25, 0.0);
	//	DrawText(screenText, "Space", 0.15, 0.001, 1.0, 1.0, 0.0, 1.0);
	//	glLoadIdentity();
	//	glTranslatef(-0.5, 0.0, 0.0);
	//	DrawText(screenText, "Invaders", 0.15, 0.001, 1.0, 1.0, 0.0, 1.0);*/

	//	// game over text
	//	/*glLoadIdentity();
	//	glTranslatef(-0.25, 0.25, 0.0);
	//	DrawText(screenText, "Game", 0.2, 0.001, 1.0, 0.0, 0.0, 1.0);
	//	glLoadIdentity();
	//	glTranslatef(-0.25, 0.0, 0.0);
	//	DrawText(screenText, "Over", 0.2, 0.001, 1.0, 0.0, 0.0, 1.0);
	//	glLoadIdentity();
	//	glTranslatef(-0.45, -0.25, 0.0);
	//	DrawText(screenText, "Final Score:", 0.07, 0.001, 1.0, 0.0, 0.0, 1.0);
	//	glLoadIdentity();
	//	glTranslatef(0.5, -0.25, 0.0);
	//	DrawText(screenText, scoreString, 0.07, 0.001, 1.0, 0.0, 0.0, 1.0);*/


	//	SDL_GL_SwapWindow(displayWindow);
	//}

	SDL_Quit();
	return 0;
}