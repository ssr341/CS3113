#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp(){
	Init();
	menu.Init();
	instructions.Init();
	stage1.Init();
	stage2.Init();
	stage3.Init();
	gameOver.Init();
	done = false;
	state = 0;
	stage1Winner = 0;
	stage2Winner = 0;
	gameWinner = 0;
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

	// sound stuff
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	stage1.shootingSound = Mix_LoadWAV("shootingSound.wav");
	stage1.explosionSound = Mix_LoadWAV("explosionSound.wav");
	stage1.powerupSound = Mix_LoadWAV("powerupSound.wav");
	stage2.shootingSound = Mix_LoadWAV("shootingSound.wav");
	stage2.explosionSound = Mix_LoadWAV("explosionSound.wav");
	stage2.powerupSound = Mix_LoadWAV("powerupSound.wav");
	stage3.shootingSound = Mix_LoadWAV("shootingSound.wav");
	stage3.explosionSound = Mix_LoadWAV("explosionSound.wav");
	music = Mix_LoadMUS("gameMusic.mp3"); //http://www.youtube.com/watch?v=ea-XjlC6YKA
	Mix_PlayMusic(music, -1);

}

// clear background music
ClassDemoApp::~ClassDemoApp(){
	Mix_FreeMusic(music);
	SDL_Quit();
}

// process user input
bool ClassDemoApp::ProcessEvents(){
	SDL_Event event;

	while (SDL_PollEvent(&event) && !done) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		switch (state){
		case STATE_MAIN_MENU:
			state = menu.ProcessEvents(&event, done);
			if (state != 0)
				return done;
			break;
		case STATE_INSTRUCTIONS:
			state = instructions.ProcessEvents(&event, done);
			if (state != 1)
				return done;
			break;
		case STATE_STAGE_ONE:
			stage1.ProcessShoot(&event, done);
			break;
		case STATE_STAGE_TWO:
			stage2.ProcessShoot(&event, done);
			break;
		case STATE_STAGE_THREE:
			stage3.ProcessShoot(&event, done);
			break;
		case STATE_GAME_OVER:
			state = gameOver.ProcessEvents(&event, done);
			// if user wants to play again, reset everything
			if (state == 0){
				winner = 0;
				stage1Winner = 0;
				stage2Winner = 0;
				gameWinner = 0;
				stage1.reset();
				stage2.reset();
				stage3.reset();
			}
			break;
		}
	}
	switch (state) {
	case STATE_STAGE_ONE:
		stage1.ProcessEvents();
		break;
	case STATE_STAGE_TWO:
		stage2.ProcessEvents();
		break;
	case STATE_STAGE_THREE:
		stage3.ProcessEvents();
		break;
	}

	return done;
}

// update the levels based on time
void ClassDemoApp::fixedUpdate(float fixedElapsed){
	switch (state) {
	case STATE_STAGE_ONE:
		if (winner == 0) // no winner, resume gameplay
			winner = stage1.fixedUpdate(fixedElapsed);
		if (winner == 1){ // player 1 wins
			stage1Winner = 1;
			if (stage1.explosion(fixedElapsed)) // perform explosion
				state++;
		}
		if (winner == 2){ // player 2 wins
			stage1Winner = 2;
			if (stage1.explosion(fixedElapsed)) // perform explosion
				state++;
		}
		break;
	case STATE_STAGE_TWO:
		winner = stage2.fixedUpdate(fixedElapsed);
		if (winner == 1){ // player 1 wins
			stage2Winner = 1;
			if (stage2.explosion(fixedElapsed)){ // perform explosion
				if (stage1Winner == 1 && stage2Winner == 1){ // if player 1 won stage 1 and 2, go to game over
					gameWinner = 1;
					state = 5;
				}
				else // if tie, go to stage 3
					state++;
			}
		}
		if (winner == 2){ // player 2 wins
			stage2Winner = 2;
			if (stage2.explosion(fixedElapsed)){ // perform explosion
				if (stage1Winner == 2 && stage2Winner == 2){ // if player 2 won stage 1 and 2, go to game over
					gameWinner = 2;
					state = 5;
				}
				else // if tie, go to stage 3
					state++;
			}
		}
		break;
	case STATE_STAGE_THREE:
		winner = stage3.fixedUpdate(fixedElapsed);
		if (winner == 1){ // player 1 wins
			if (stage3.explosion(fixedElapsed)){ // perform explosion
				gameWinner = 1;
				state++;
			}
		}
		if (winner == 2){ // player 2 wins
			if (stage3.explosion(fixedElapsed)){ // perform explosion
				gameWinner = 2;
				state++;
			}
		}
		break;
	}
}

// draw stages
void ClassDemoApp::Render(float fixedElapsed){
	switch (state) {
	case STATE_MAIN_MENU:
		menu.Render(fixedElapsed);
		break;
	case STATE_INSTRUCTIONS:
		instructions.Render();
		break;
	case STATE_STAGE_ONE:
		stage1.Render();
		break;
	case STATE_STAGE_TWO:
		stage2.Render();
		break;
	case STATE_STAGE_THREE:
		stage3.Render();
		break;
	case STATE_GAME_OVER:
		gameOver.Render(gameWinner, fixedElapsed);
		break;
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
		fixedUpdate(fixedElapsed);
	}

	Render(fixedElapsed);
	timeLeftOver = fixedElapsed;
}