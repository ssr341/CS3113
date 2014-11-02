
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