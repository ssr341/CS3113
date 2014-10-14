// value of level height and width, tileSize value, does texture have to be loaded, how do the blocks get pushed back and collision work with them
// combining legs and body into one Entity, how bullet texture is loaded, tileMap stuff in relation to level designer
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