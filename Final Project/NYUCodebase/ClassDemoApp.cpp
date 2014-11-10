#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp(){
	Init();
	menu.Init();
	instructions.Init();
	//stageOne.Init();
	//stageTwo.Init();
	//stageThree.Init();
	gameOver.Init();
	done = false;
	state = 0;
	timeLeftOver = 0.0f;
}

void ClassDemoApp::Init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Galaxy Defender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("gameMusic.mp3"); //http://www.youtube.com/watch?v=ea-XjlC6YKA
	Mix_PlayMusic(music, -1);

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
		/*switch (state){
		case STATE_GAME_LEVEL:
			gameplay.ProcessShoot(&event);
			break;
		}*/
	}
	switch (state) {
	case STATE_MAIN_MENU:
		state = menu.ProcessEvents();
		break;
	case STATE_INSTRUCTIONS:
		state = instructions.ProcessEvents();
		break;
	}

	return done;
}

void ClassDemoApp::fixedUpdate(){
	//switch (state) {
	//case STATE_STAGE_ONE:
	//	state = stageOne.fixedUpdate();
	//	break;
	//}
	//case STATE_STAGE_TWO:
	//	state = stageTwo.fixedUpdate();
	//	break;
	//}
	//case STATE_STAGE_THREE:
	//	state = stageOne.fixedUpdate();
	//	break;
	//}
}


void ClassDemoApp::Render(){
	//render stuff
	switch (state) {
	case STATE_MAIN_MENU:
		menu.Render();
		break;
	case STATE_INSTRUCTIONS:
		instructions.Render();
		break;
	/*case STATE_GAME_LEVEL:
		score = gameplay.Render();
		break;
	case STATE_GAME_OVER:
		gameOver.Render(score);
		break;*/
	}
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	// fixed timestep loop
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}

	while (fixedElapsed >= FIXED_TIMESTEP){
		fixedElapsed -= FIXED_TIMESTEP;
		fixedUpdate();
	}

	timeLeftOver = fixedElapsed;
	Render();

}