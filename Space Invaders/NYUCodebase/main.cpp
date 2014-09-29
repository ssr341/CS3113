
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "ClassDemoApp.h"
using namespace std;

int main(int argc, char *argv[])
{
	ClassDemoApp app;
	while (!app.ProcessEvents()){
		app.UpdateAndRender();
	}
	
	SDL_Quit();
	return 0;
}