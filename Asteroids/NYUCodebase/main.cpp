// how does vector/matrix multiplication work since its 4x4 matrix and 3x1 vector, when is vector length used
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ClassDemoApp.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	ClassDemoApp app;

	while (!app.ProcessEvents()){
		app.UpdateAndRender();
	}

	return 0;
}