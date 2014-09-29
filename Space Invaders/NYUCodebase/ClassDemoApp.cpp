#include "ClassDemoApp.h"
#include "Bullet.h"
#include "Entity.h"
#include "mainMenu.h"
#include "gameOver.h"
#include <vector>
#include <string>

ClassDemoApp::ClassDemoApp(){
	Init();
	done = false;
}

void ClassDemoApp::Init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

}

ClassDemoApp::~ClassDemoApp(){
	SDL_Quit();
}

bool ClassDemoApp::ProcessEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	switch (state) {
	case STATE_MAIN_MENU:
		menu.ProcessEvents();
		break;
	case STATE_GAME_LEVEL:
		gameplay.ProcessEvents();
		break;
	case STATE_GAME_OVER:
		gameOver.ProcessEvents();
		break;
	}
	return done;
}

void ClassDemoApp::Update(float elapsed){
	switch (state) {
	case STATE_MAIN_MENU:
		state = menu.Update();
	break;
	case STATE_GAME_LEVEL:
		state = gameplay.Update(elapsed);
	break;
	case STATE_GAME_OVER:
		state = gameOver.Update();
	}
}


void ClassDemoApp::Render(){
	//render stuff
	switch (state) {
		case STATE_MAIN_MENU:
			menu.Render();
			break;
		case STATE_GAME_LEVEL:
			score = gameplay.Render();	
			break;
		case STATE_GAME_OVER:
			gameOver.Render(score);
			break;
	}
}

bool ClassDemoApp::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	while (!ProcessEvents()){
		Update(elapsed);
		Render();
	}

	return done;
}