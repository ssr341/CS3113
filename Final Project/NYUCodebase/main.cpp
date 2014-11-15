// render issue from main/instruction to stage 1, gameover to main menu shows up in alt tab when you run through it but not normally, rapid fire 
// enemy bullets on start up
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