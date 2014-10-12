#include "Gameplay.h"

Gameplay::Gameplay(){
	enemyShot = 0.0f;
	state = 1;
	score = 0;
	num_rows = 4;
	num_columns = 10;
}

GLuint Gameplay::LoadTexture(const char *image_path) {
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

void Gameplay::DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a) {
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

void Gameplay::Init(){
	spriteSheetTexture = LoadTexture("sheet.png");
	screenText = LoadTexture("font1.png");

	// name = "playerShip1_blue.png" x = "211" " y = "941" width = "99" height = "75"
	playerSprite = Entity(spriteSheetTexture, 211.0f / 1024.f, 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
	playerSprite.y = -0.8f;
	playerSprite.x = 0.0f;
	playerSprite.direction_x = 0.0f;

	// name="ufoBlue.png" x="444" y="91" width="91" height="91" 
	float y = 0.8f;  // y position of top aliens
	float x = -1.0f; // x position of first column
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

//bool Gameplay::ProcessEvents(SDL_Event* event){
//	const Uint8* keys = SDL_GetKeyboardState(NULL);
//
//	if (event->type == SDL_KEYDOWN) {
//		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
//			shootBullet(playerSprite.x, playerSprite.y, 1.0, true);
//		}
//	}
//
//	// left, right and space keys used by player
//	if (keys[SDL_SCANCODE_LEFT] == 1){
//		// if left pressed, set direction negative
//		playerSprite.direction_x = -1.0;
//	}
//	if (keys[SDL_SCANCODE_RIGHT]){
//		// if down pressed, set direction positive
//		playerSprite.direction_x = 1.0;
//	}
//
//	// set direction and speed of enemies at start
//	// check for neutral, if first enemy is neutral rest are as well
//	if (enemies[0][0].direction_x == 0 && enemies[0][0].direction_y == 0){
//		for (size_t i = 0; i < enemies.size(); i++){
//			for (size_t j = 0; j < enemies[i].size(); j++){
//				enemies[i][j].direction_x = 1.0f;
//				enemies[i][j].direction_y = -1.0f;
//				enemies[i][j].speed = 0.25f;
//			}
//		}
//	}
//
//	return true;
//}

bool Gameplay::ProcessShoot(SDL_Event* event){
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	if (event->type == SDL_KEYDOWN) {
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
			shootBullet(playerSprite.x, playerSprite.y, 1.0, true);
		}
	}
	return true;
}

bool Gameplay::ProcessEvents(){
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	// left, right and space keys used by player
	if (keys[SDL_SCANCODE_LEFT] == 1){
		// if left pressed, set direction negative
		playerSprite.direction_x = -1.0;
	}
	if (keys[SDL_SCANCODE_RIGHT]){			// if down pressed, set direction positive
		playerSprite.direction_x = 1.0;
	}
	
	// set direction and speed of enemies at start
	// check for neutral, if first enemy is neutral rest are as well
	if (enemies[0][0].direction_x == 0 && enemies[0][0].direction_y == 0){
		for (size_t i = 0; i < enemies.size(); i++){
			for (size_t j = 0; j < enemies[i].size(); j++){
				enemies[i][j].direction_x = 1.0f;
				enemies[i][j].direction_y = -1.0f;
				enemies[i][j].speed = 0.25f;
			}
		}
	}
	
	return true;
}

int Gameplay::Update(float elapsed){
	// player
	playerSprite.x += playerSprite.direction_x * elapsed;
	// screen boundaries for player
	if (playerSprite.x > 1.2)
		playerSprite.x = 1.2f;
	if (playerSprite.x < -1.2)
		playerSprite.x = -1.2f;
	playerSprite.direction_x = 0; // stops further movement

	// enemies

	// y boundary for enemy
	if (enemies[num_columns - 1][num_rows - 1].y == -0.4 || enemies[0][num_rows - 1].y == -0.4){
		for (size_t i = 0; i < enemies.size(); i++){
			for (size_t j = 0; j < enemies[i].size(); j++){
				enemies[i][j].direction_y = 0.0;
			}
		}
	}
	// update enemy movement
	for (size_t i = 0; i < enemies.size(); i++){
		for (size_t j = 0; j < enemies[i].size(); j++){
			enemies[i][j].x += enemies[i][j].direction_x * elapsed * enemies[i][j].speed;
			enemies[i][j].y += enemies[i][j].direction_y / 20 * elapsed * enemies[i][j].speed;
		}
	}

	// check for visible enemies
	/*bool visCount = false;
	while (!visCount){
		for (size_t i = 0; i < enemies.size(); i++){
			for (size_t j = 0; j < enemies[i].size(); j++){
				if (enemies[i][j].visible){
					visCount = true;
					break;
				}
			}
			if(visCount)
				break;
		}
	}
	if (!visCount){
		state = 2;
		return state;
	}*/

	// check for alive enemies
	if (score >= 4000){
		state = 2;
		return state;
	}

	// check for collision with right side
	if (enemies[num_columns - 1][num_rows - 1].x >= 1.28f){
		for (size_t i = 0; i < enemies.size(); i++){
			for (size_t j = 0; j < enemies[i].size(); j++){
				enemies[i][j].direction_x = -1.0;
			}
		}
	}

	// check for collision with left side
	if (enemies[0][0].x <= -1.28f){
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

					// if bullet is visible and colliding
					if (bullets[k].visible && bullets[k].x > enemyLeft && bullets[k].x < enemyRight &&
						bullets[k].y > enemyBottom && bullets[k].y < enemyTop && bullets[k].shooter){
						// make enemy and bullet invisible and update score
						enemies[i][j].visible = false;
						bullets[k].visible = false;
						bullets[k].speed = 0.0;
						score += 100;
					}
				}
			}
		}
	}

	// check for bullet collision with player
	for (int k = 0; k < MAX_BULLETS; k++){
		// enemy sides
		float playerRight = playerSprite.x + playerSprite.width / 2;
		float playerLeft = playerSprite.x + playerSprite.width / -2;
		float playerTop = playerSprite.y + playerSprite.height / 2;
		float playerBottom = playerSprite.y + playerSprite.height / -2;

		// if bullet is visible and colliding
		if (bullets[k].visible && bullets[k].x > playerLeft && bullets[k].x < playerRight &&
			bullets[k].y > playerBottom && bullets[k].y < playerTop && !bullets[k].shooter){
			state = 2;
			return state;
		}
	}

	// have enemy shoot bullet
	enemyShot += elapsed;
	if (enemyShot >= 0.5f){    // shoot every 50 frames
		int enemyBulletX = rand() % (9);
		bool shot = false;  // was the bullet shot?
		for (size_t i = 3; i > 0; i--){
			if (!shot){
				if (enemies[enemyBulletX][i].visible){
					shootBullet(enemies[enemyBulletX][i].x, enemies[enemyBulletX][i].y - 0.1f, -1.0f, false);
					shot = true;
				}
			}
			else
				break;
		}
		enemyShot = 0;
	}

	// update position of each bullet
	for (int i = 0; i < MAX_BULLETS; i++){
		bullets[i].Update(elapsed);
	}

	return state;
}

int Gameplay::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
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
	//SDL_GL_SwapWindow(displayWindow);

	return score;
}

// shoots bullet at specified location
void Gameplay::shootBullet(float x, float y, float direction, bool shooter){
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].shooter = shooter;
	bullets[bulletIndex].x = x;
	bullets[bulletIndex].y = y;
	bullets[bulletIndex].direction = direction;
	bullets[bulletIndex].size = 0.01f;
	bullets[bulletIndex].speed = 2.0f;
	bulletIndex++;

	// reset index if max bullet is reached and overwrite earlier bullets
	if (bulletIndex > MAX_BULLETS - 1)
		bulletIndex = 0;
}