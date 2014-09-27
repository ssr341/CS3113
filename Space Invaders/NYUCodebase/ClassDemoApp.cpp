#include "ClassDemoApp.h"
#include "Bullet.h"
#include "Entity.h"
#include "mainMenu.h"
#include <vector>
#include <string>

ClassDemoApp::ClassDemoApp(){
	Init();
	done = false;
	lastFrameTicks = 0.0f;
}

GLuint ClassDemoApp::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textureID;
}

void ClassDemoApp::DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float texture_size = 1.0 / 16.0f;

	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	std::vector<float> colorData;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
			* size });

		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
}

void ClassDemoApp::Init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	unsigned int spriteSheetTexture = LoadTexture("sheet.png");
	unsigned int screenText = LoadTexture("font1.png");

	// name = "playerShip1_blue.png" x = "211" " y = "941" width = "99" height = "75"
	playerSprite = Entity(spriteSheetTexture, 211.0f / 1024.f, 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
	playerSprite.y = -0.8f;
	playerSprite.x = 0.0f;
	playerSprite.direction_x = 0.0f;

	// name="ufoBlue.png" x="444" y="91" width="91" height="91" 
	float y = 0.8;  // y position of top aliens
	float x = -1.0; // x position of first column
	for (int i = 0; i < num_columns; i++){
		std::vector<Entity> column;
		for (int j = 0; j < num_rows; j++){
			column.push_back(Entity(spriteSheetTexture, 444.0f / 1024.f, 91.0f / 1024.0f, 91.0f / 1024.0f, 91.0f / 1024.0f));
			column[j].x = x;
			column[j].y = y;
			y -= 0.2f; // position next alien
		}
		y = 0.8f;   // reset y position
		x += 0.2f;  // next row
		enemies.push_back(column);
		column.clear();
	}
}

ClassDemoApp::~ClassDemoApp(){
	SDL_Quit;
}

bool ClassDemoApp::ProcessEvents(){
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (keys[SDL_SCANCODE_SPACE]){
			// shoot bullet from player location
			shootBullet(playerSprite.x, playerSprite.y);
		}
	}

	// left, right and space keys used by player
	if (keys[SDL_SCANCODE_LEFT] == 1){
		// if left pressed, set direction negative
		playerSprite.direction_x = -1.0;
	}
	if (keys[SDL_SCANCODE_RIGHT]){
		// if down pressed, set direction positive
		playerSprite.direction_x = 1.0;
	}

	// set direction and speed of enemies at start
	// check for neutral, if first enemy is neutral rest are as well
	if (enemies[0][0].direction_x == 0 && enemies[0][0].direction_y == 0){
		for (size_t i = 0; i < enemies.size(); i++){
			for (size_t j = 0; j < enemies[i].size(); j++){
				enemies[i][j].direction_x = 1.0f;
				enemies[i][j].direction_y = -1.0f;
				enemies[i][j].speed = 0.01f;
			}
		}
	}

	return done;
}

void ClassDemoApp::Update(float elapsed){
	switch (state) {
	case STATE_MAIN_MENU:
		menu.Update();
	break;
	case STATE_GAME_LEVEL:
		// player
		playerSprite.x += playerSprite.direction_x * elapsed;
		// screen boundaries for player
		if (playerSprite.x > 1.2)
			playerSprite.x = 1.2f;
		if (playerSprite.x < -1.2)
			playerSprite.x = -1.2f;
		playerSprite.direction_x = 0;  // stops further movement

		// enemies
		/*for (size_t i = 0; i < enemies.size(); i++){
		for (size_t j = 0; j < enemies[i].size(); j++){
		enemies[i][j].x += enemies[i][j].direction_x * elapsed * enemies[i][j].speed;
		enemies[i][j].y += enemies[i][j].direction_y * elapsed * enemies[i][j].speed;
		}
		}*/

		// check for collision with right side
		if (enemies[num_columns - 1][num_rows - 1].x = 1.28f){
			for (size_t i = 0; i < enemies.size(); i++){
				for (size_t j = 0; j < enemies[i].size(); j++){
					enemies[i][j].direction_x = -1.0;
				}
			}
		}

		// check for collision with left side
		if (enemies[0][0].x = -1.28f){
			for (size_t i = 0; i < enemies.size(); i++){
				for (size_t j = 0; j < enemies[i].size(); j++){
					enemies[i][j].direction_x = 1.0;
				}
			}
		}

		// check for enemy collision with bullets
		for (size_t i = 0; i < enemies.size(); i++){
			for (size_t j = 0; j < enemies[i].size(); j++){
				for (int k = 0; k < MAX_BULLETS; k++){
					// if enemy already dead ignore
					if (enemies[i][j].visible){
						// enemy sides
						float enemyRight = enemies[i][j].x + enemies[i][j].width / 2;
						float enemyLeft = enemies[i][j].x + enemies[i][j].width / -2;
						float enemyTop = enemies[i][j].y + enemies[i][j].height / 2;
						float enemyBottom = enemies[i][j].y + enemies[i][j].height / -2;

						if (bullets[k].x > enemyLeft && bullets[k].x < enemyRight){
							// make enemy and bullet invisible and update score
							enemies[i][j].visible = false;
							bullets[k].visible = false;
							score += 100;
						}
						if (bullets[k].y > enemyBottom && bullets[k].y < enemyTop){
							// make enemy and bullet invisible and update score
							enemies[i][j].visible = false;
							bullets[k].visible = false;
							score += 100;
						}
					}
				}
			}
		}

		// update position of each bullet
		for (int i = 0; i < MAX_BULLETS; i++){
			bullets[i].Update(elapsed);
		}
	break;
	case STATE_GAME_OVER:
		gameOver.Update();
	}
}


void ClassDemoApp::Render(){
	//render stuff
	switch (state) {
		case STATE_MAIN_MENU:
			menu.Render();
		break;
		case STATE_GAME_LEVEL:
			glClear(GL_COLOR_BUFFER_BIT);

			playerSprite.Draw(1.0f);

			for (size_t i = 0; i < enemies.size(); i++){
				for (size_t j = 0; j < enemies[i].size(); j++){
					if (enemies[i][j].visible)
						enemies[i][j].Draw(1.0f);
				}
			}

			for (int i = 0; i < MAX_BULLETS; i++){
				// only draw bullets that haven't collided or are on screen
				if (bullets[i].visible)
					bullets[i].Draw();
			}

			// score text
			glLoadIdentity();
			glTranslatef(-0.9f, 0.9f, 0.0);
			DrawText(screenText, "Score:", 0.06f, 0.001f, 1.0, 1.0, 0.0, 1.0);

			std::string scoreString = std::to_string(score);
			glLoadIdentity();
			glTranslatef(-0.4f, 0.9f, 0.0);
			DrawText(screenText, scoreString, 0.06f, 0.001f, 1.0, 1.0, 0.0, 1.0);

			SDL_GL_SwapWindow(displayWindow);
			
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

// shoots bullet at specified location
void ClassDemoApp::shootBullet(float x, float y){
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = x;
	bullets[bulletIndex].y = y;
	bullets[bulletIndex].size = 0.01f;
	bullets[bulletIndex].speed = 2.0f;

	bulletIndex++;
	// reset index if max bullet is reached and overwrite earlier bullets
	if (bulletIndex > MAX_BULLETS - 1)
		bulletIndex = 0;
}