#include "StageThree.h"

StageThree::StageThree(){
	winner = 0;

	bulletIndex = 0;
	enemyShot = 0.0f;
	enemyBulletSize = 0.03f;
	enemyBulletSpeed = 2.0f;

	player1Shot = 10.0f;
	player2Shot = 10.0f;
	player1ShotTime = 0.0125f;
	player2ShotTime = 0.0125f;
	player1BulletSize = 0.04f;
	player1BulletSpeed = 4.0f;
	player2BulletSize = 0.04f;
	player2BulletSpeed = 4.0f;

	for (int i = 0; i < MAX_BULLETS; i++){
		bullets[i].x = -10.0f;
		bullets[i].y = -10.0f;
		bullets[i].direction = 0.0f;
		bullets[i].shooter = -1.0f;
		bullets[i].speed = 0.0f;
		bullets[i].size = 0.0f;
	}
}

StageThree::~StageThree(){
	Mix_FreeChunk(shootingSound);
	Mix_FreeChunk(explosionSound);
}


GLuint StageThree::LoadTexture(const char *image_path) {
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

void StageThree::DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a) {
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

void StageThree::Init(){
	// player creation
	player1.textureID = LoadTexture("playerShip1.png");
	player1.height = 0.25f;
	player1.width = 0.25f;
	player1.x = -1.13f;
	player1.y = 0.0f;
	player1.friction_y = 1.5f;
	player1.acceleration_y = 0.0f;
	player1.velocity_y = 0.0f;
	player1.visible = true;

	player2.textureID = LoadTexture("playerShip2.png");
	player2.height = 0.25f;
	player2.width = 0.25f;
	player2.x = 1.13f;
	player2.y = 0.0f;
	player2.friction_y = 1.5f;
	player2.acceleration_y = 0.0f;
	player2.velocity_y = 0.0f;
	player2.visible = true;

	// bullet texture
	for (int i = 0; i < MAX_BULLETS; i++)
		bullets[i].textureID = LoadTexture("laserBlue03.png");

	// enemy creation
	enemy.textureID = LoadTexture("enemyUFO.png");
	enemy.width = 0.75f;
	enemy.height = 0.75f;
	enemy.x = 0.0f;
	enemy.y = 0.0f;
	enemy.velocity_y = 0.0f;
	enemy.acceleration_y = 0.5f;
	enemy.friction_y = 1.0f;
	enemy.visible = true;
}

void StageThree::ProcessShoot(SDL_Event* event){
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (event->type == SDL_KEYDOWN) {
		if (event->key.keysym.scancode == SDL_SCANCODE_F) {
			if (player1Shot > player1ShotTime){
				player1Shot = 0.0;
				shootBullet(player1.x, player1.y, 1.0, 0, player1BulletSize, player1BulletSpeed);
				Mix_PlayChannel(-1, shootingSound, 0);
			}
		}
		if (event->key.keysym.scancode == SDL_SCANCODE_KP_0) {
			if (player2Shot > player2ShotTime){
				player2Shot = 0.0;
				shootBullet(player2.x, player2.y, -1.0, 1, player2BulletSize, player2BulletSpeed);
				Mix_PlayChannel(-1, shootingSound, 0);
			}
		}
	}
}

void StageThree::ProcessEvents(){
	player1.acceleration_y = 0.0f;
	player2.acceleration_y = 0.0f;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	// e, d and f keys used by player 1
	if (keys[SDL_SCANCODE_E]){
		// if e pressed, set acceleration positive
		player1.acceleration_y = 4.5f;
	}
	if (keys[SDL_SCANCODE_D]){
		// if d pressed, set acceleration negative
		player1.acceleration_y = -4.5f;
	}

	// up, down and 0 keys used by player 2
	if (keys[SDL_SCANCODE_UP]){
		// if up pressed, set acceleration positive
		player2.acceleration_y = 4.5f;
	}
	if (keys[SDL_SCANCODE_DOWN]){
		// if down pressed, set acceleration negative
		player2.acceleration_y = -4.5f;
	}
	// if q is pressed quit
	if (keys[SDL_SCANCODE_Q] == 1){
		SDL_Quit();
	}

}

int StageThree::fixedUpdate(float fixedElapsed){
	// run fixed update for everything
	player1.fixedUpdate();
	player2.fixedUpdate();
	enemy.fixedUpdate();

	// screen boundaries
	if (player1.y > 0.85)
		player1.y = 0.85;
	if (player2.y > 0.85)
		player2.y = 0.85;
	if (player1.y < -0.85)
		player1.y = -0.85;
	if (player2.y < -0.85)
		player2.y = -0.85;

	// have enemies reverse when they meet edge of screen
	if (enemy.y >= 0.45)
		enemy.acceleration_y = -1.0f;
	if (enemy.y <= -0.45)
		enemy.acceleration_y = 1.0f;

	// have bullets colliding with ufo vanish
	for (int i = 0; i < MAX_BULLETS; i++){
		if (bullets[i].visible && bullets[i].shooter != 2 && enemy.collidesWith(bullets[i])){
			bullets[i].visible = false;
			Mix_PlayChannel(-1, explosionSound, 0);
		}
	}

	// check for bullet collision with player
	for (int i = 0; i < MAX_BULLETS; i++){
		// if bullet is visible and colliding
		if (bullets[i].visible && bullets[i].shooter != 0 && player1.collidesWith(bullets[i])){
			Mix_PlayChannel(-1, explosionSound, 0);
			winner = 2;
			return winner;
		}
	}
	for (int i = 0; i < MAX_BULLETS; i++){
		// if bullet is visible and colliding
		if (bullets[i].visible && bullets[i].shooter != 1 && player2.collidesWith(bullets[i])){
			Mix_PlayChannel(-1, explosionSound, 0);
			winner = 1;
			return winner;
		}
	}

	// have enemy shoot bullet
	enemyShot += fixedElapsed;
	if (enemyShot >= 0.08f){    // shoot every 8 frames
		Mix_PlayChannel(-1, shootingSound, 0);
		shootBullet(enemy.x - enemy.width/2, enemy.y, -1.0f, 2, enemyBulletSize, enemyBulletSpeed);
		shootBullet(enemy.x + enemy.width/2, enemy.y, 1.0f, 2, enemyBulletSize, enemyBulletSpeed);
		enemyShot = 0;
	}

	// update position of each bullet
	for (int i = 0; i < MAX_BULLETS; i++){
		bullets[i].fixedUpdate();
	}

	// update player shot timers
	player1Shot += fixedElapsed;
	player2Shot += fixedElapsed;

	return winner;
}

void StageThree::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);

	player1.draw();
	player2.draw();
	enemy.draw();

	for (int i = 0; i < MAX_BULLETS; i++){
		// only draw bullets that haven't collided or are on screen
		if (bullets[i].visible)
			bullets[i].Draw();
	}

}

// shoots bullet at specified location
void StageThree::shootBullet(float x, float y, float direction, float shooter, float size, float speed){
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = x;
	bullets[bulletIndex].y = y;
	bullets[bulletIndex].direction = direction;
	bullets[bulletIndex].size = size;
	bullets[bulletIndex].speed = speed;
	bullets[bulletIndex].shooter = shooter;
	bulletIndex++;

	// reset index if max bullet is reached and overwrite earlier bullets
	if (bulletIndex > MAX_BULLETS - 1)
		bulletIndex = 0;
}