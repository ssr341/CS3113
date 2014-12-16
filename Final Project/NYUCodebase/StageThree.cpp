#include "StageThree.h"

StageThree::StageThree(){
	winner = 0;
	freeze = false;

	hitsToKill = 3;
	player1Hits = 0;
	player2Hits = 0;

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

// release sound chunks
StageThree::~StageThree(){
	Mix_FreeChunk(shootingSound);
	Mix_FreeChunk(explosionSound);
}

// load textures for entities
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

void StageThree::Init(){
	// entity creation
	player1.textureID = LoadTexture("playerShip1.png");
	player1.height = 0.25f;
	player1.width = 0.25f;
	player1.x = -1.13f;
	player1.y = 0.0f;
	player1.rotate = false;
	player1.rotation = 0.0f;
	player1.friction_y = 2.5f;
	player1.acceleration_y = 0.0f;
	player1.velocity_y = 0.0f;
	player1.visible = true;

	player1fire.textureID = LoadTexture("fire.png");
	player1fire.height = 0.25f;
	player1fire.width = 0.25f;
	player1fire.x = -1.13f;
	player1fire.y = 0.0f;
	player1fire.rotate = false;
	player1fire.rotation = 0.0f;
	player1fire.friction_y = 0.0f;
	player1fire.acceleration_y = 0.0f;
	player1fire.velocity_y = 0.0f;
	player1fire.visible = true;

	player1shield.textureID = LoadTexture("ring.png");
	player1shield.height = 0.25f;
	player1shield.width = 0.25f;
	player1shield.x = -1.13f;
	player1shield.y = 0.0f;
	player1shield.rotate = false;
	player1shield.rotation = 0.0f;
	player1shield.friction_y = 0.0f;
	player1shield.acceleration_y = 0.0f;
	player1shield.velocity_y = 0.0f;
	player1shield.visible = true;

	player2.textureID = LoadTexture("playerShip2.png");
	player2.height = 0.25f;
	player2.width = 0.25f;
	player2.x = 1.13f;
	player2.y = 0.0f;
	player2.rotate = false;
	player2.rotation = 0.0f;
	player2.friction_y = 2.5f;
	player2.acceleration_y = 0.0f;
	player2.velocity_y = 0.0f;
	player2.visible = true;

	player2fire.textureID = LoadTexture("fire.png");
	player2fire.height = 0.25f;
	player2fire.width = 0.25f;
	player2fire.x = 1.13f;
	player2fire.y = 0.0f;
	player2fire.rotate = false;
	player2fire.rotation = 0.0f;
	player2fire.friction_y = 0.0f;
	player2fire.acceleration_y = 0.0f;
	player2fire.velocity_y = 0.0f;
	player2fire.visible = true;

	player2shield.textureID = LoadTexture("ring.png");
	player2shield.height = 0.25f;
	player2shield.width = 0.25f;
	player2shield.x = 1.13f;
	player2shield.y = 0.0f;
	player2shield.rotate = false;
	player2shield.rotation = 0.0f;
	player2shield.friction_y = 0.0f;
	player2shield.acceleration_y = 0.0f;
	player2shield.velocity_y = 0.0f;
	player2shield.visible = true;

	sparkle.textureID = LoadTexture("sparkle.png");
	sparkle.x = -100.0f;
	sparkle.y = -100.0f;
	sparkle.rotate = false;
	sparkle.rotation = 0.0f;
	sparkle.height = 0.0f;
	sparkle.width = 0.0f;
	sparkle.friction_y = 0.0f;
	sparkle.acceleration_y = 0.0f;
	sparkle.velocity_y = 0.0f;
	sparkle.visible = false;

	// bullet texture
	for (int i = 0; i < MAX_BULLETS; i++)
		bullets[i].textureID = LoadTexture("laserBlue03.png");

	// enemy creation
	enemy.textureID = LoadTexture("enemyUFO.png");
	enemy.width = 0.75f;
	enemy.height = 0.75f;
	enemy.x = 0.0f;
	enemy.y = 0.0f;
	enemy.rotate = false;
	enemy.rotation = 0.0f;
	enemy.velocity_y = 0.0f;
	enemy.acceleration_y = 0.5f;
	enemy.friction_y = 1.0f;
	enemy.visible = true;
}

// reset the stage. called when user wants to play again
void StageThree::reset(){
	winner = 0;
	freeze = false;
	explosionTime = 0.0f;

	bulletIndex = 0;

	player1Hits = 0;
	player2Hits = 0;
	enemyShot = 0.0f;

	player1Shot = 10.0f;
	player2Shot = 10.0f;
	player1ShotTime = 0.0125f;
	player2ShotTime = 0.0125f;
	player1BulletSize = 0.04f;
	player1BulletSpeed = 4.0f;
	player2BulletSize = 0.04f;
	player2BulletSpeed = 4.0f;

	player1.x = -1.13f;
	player1.y = 0.0f;
	player1.acceleration_y = 0.0f;
	player1.velocity_y = 0.0f;
	player1.visible = true;

	player1shield.x = -1.13f;
	player1shield.y = 0.0f;
	player1shield.height = 0.25f;
	player1shield.width = 0.25f;

	player1fire.x = -1.13f;
	player1fire.y = 0.0f;

	player2.x = 1.13f;
	player2.y = 0.0f;
	player2.acceleration_y = 0.0f;
	player2.velocity_y = 0.0f;
	player2.visible = true;

	player2shield.x = 1.13f;
	player2shield.y = 0.0f;
	player2shield.height = 0.25f;
	player2shield.width = 0.25f;

	player2fire.x = 1.13f;
	player2fire.y = 0.0f;

	sparkle.x = -100.0f;
	sparkle.y = -100.0f;
	sparkle.height = 0.0f;
	sparkle.width = 0.0f;
	sparkle.visible = false;

	for (int i = 0; i < MAX_BULLETS; i++){
		bullets[i].x = -10.0f;
		bullets[i].y = -10.0f;
		bullets[i].direction = 0.0f;
		bullets[i].shooter = -1.0f;
		bullets[i].speed = 0.0f;
		bullets[i].size = 0.0f;
	}

	enemy.x = 0.0f;
	enemy.y = 0.0f;
	enemy.velocity_y = 0.0f;
	enemy.acceleration_y = 0.5f;
	enemy.friction_y = 1.0f;
	enemy.visible = true;
}

// process shooting input or quitting
void StageThree::ProcessShoot(SDL_Event* event, bool& done){
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (event->type == SDL_KEYDOWN) {
		// player 1 shoot
		if (event->key.keysym.scancode == SDL_SCANCODE_F) {
			if (player1Shot > player1ShotTime){
				player1Shot = 0.0;
				shootBullet(player1.x, player1.y, 1.0, 0, player1BulletSize, player1BulletSpeed);
				Mix_PlayChannel(-1, shootingSound, 0);
			}
		}
		// player 2 shoot
		if (event->key.keysym.scancode == SDL_SCANCODE_KP_0) {
			if (player2Shot > player2ShotTime){
				player2Shot = 0.0;
				shootBullet(player2.x, player2.y, -1.0, 1, player2BulletSize, player2BulletSpeed);
				Mix_PlayChannel(-1, shootingSound, 0);
			}
		}
		// if esc is pressed quit
		if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
			done = true;
		}
	}
}

// process movement
void StageThree::ProcessEvents(){
	player1.acceleration_y = 0.0f;
	player2.acceleration_y = 0.0f;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	// w, s and f keys used by player 1
	if (keys[SDL_SCANCODE_W]){
		// if e pressed, set acceleration positive
		player1.acceleration_y = 4.5f;
	}
	if (keys[SDL_SCANCODE_S]){
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

}

// update the gameplay each frame
int StageThree::fixedUpdate(float fixedElapsed){
	// if a player is not dead
	if (!freeze){
		// run fixed update for everything
		player1.fixedUpdate();
		player2.fixedUpdate();
		enemy.fixedUpdate();

		// screen boundaries
		if (player1.y > 0.85f)
			player1.y = 0.85f;
		if (player2.y > 0.85f)
			player2.y = 0.85f;
		if (player1.y < -0.85f)
			player1.y = -0.85f;
		if (player2.y < -0.85f)
			player2.y = -0.85f;

		// update shield and fire to player position
		player1shield.x = player1.x;
		player1shield.y = player1.y;
		player2shield.x = player2.x;
		player2shield.y = player2.y;
		player1fire.x = player1.x;
		player1fire.y = player1.y;
		player2fire.x = player2.x;
		player2fire.y = player2.y;

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
				// player not dead
				if (player1Hits < hitsToKill - 1){
					player1Hits++;
					bullets[i].visible = false;
				}
				// player dead
				else{
					Mix_PlayChannel(-1, explosionSound, 0);
					freeze = true;
					winner = 2;
					return winner;
				}
			}
		}
		for (int i = 0; i < MAX_BULLETS; i++){
			// if bullet is visible and colliding
			if (bullets[i].visible && bullets[i].shooter != 1 && player2.collidesWith(bullets[i])){
				// player not dead
				if (player2Hits < hitsToKill - 1){
					player2Hits++;
					bullets[i].visible = false;
				}
				// player dead
				else{
					Mix_PlayChannel(-1, explosionSound, 0);
					freeze = true;
					winner = 1;
					return winner;
				}
			}
		}

		// have enemy shoot bullet
		enemyShot += fixedElapsed;
		if (enemyShot >= 0.08f){    // shoot every 8 frames
			Mix_PlayChannel(-1, shootingSound, 0);
			shootBullet(enemy.x - enemy.width / 2, enemy.y, -1.0f, 2, enemyBulletSize, enemyBulletSpeed);
			shootBullet(enemy.x + enemy.width / 2, enemy.y, 1.0f, 2, enemyBulletSize, enemyBulletSpeed);
			enemyShot = 0.0f;
		}

		// shrink shield if shot
		if (player1Hits > 0 && player1shield.width > 0.0f){
			player1shield.width -= fixedElapsed * 3;
			player1shield.height -= fixedElapsed * 3;
		}
		if (player2Hits > 0 && player2shield.width > 0.0f){
			player2shield.width -= fixedElapsed * 3;
			player2shield.height -= fixedElapsed * 3;
		}

		// update position of each bullet
		for (int i = 0; i < MAX_BULLETS; i++){
			bullets[i].fixedUpdate();
		}

		// update player shot timers
		player1Shot += fixedElapsed;
		player2Shot += fixedElapsed;
	}
	return winner;
}

// draw entities
void StageThree::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (player1shield.width > 0.0f)
		player1shield.draw();
	player1.draw();
	if (player1Hits == 2)
		player1fire.fireDraw();
	if (player2shield.width > 0.0f)
		player2shield.draw();
	player2.draw();
	if (player2Hits == 2)
		player2fire.fireDraw();
	enemy.draw();

	// if someone isn't dead, draw bullets
	if (!freeze){
		for (int i = 0; i < MAX_BULLETS; i++){
			// only draw bullets that haven't collided or are on screen
			if (bullets[i].visible)
				bullets[i].Draw();
		}
	}

	// if someone is dead, draw explosion
	if (freeze)
		sparkle.draw();

}

// have losing player's ship explode
bool StageThree::explosion(float fixedElapsed){
	sparkle.visible = true;
	explosionTime += fixedElapsed;
	// set sparkle over the loser
	if (winner == 1 && sparkle.x == -100.0f){
		sparkle.x = player2.x;
		sparkle.y = player2.y;
	}
	if (winner == 2 && sparkle.x == -100.0f){
		sparkle.x = player1.x;
		sparkle.y = player1.y;
	}
	// have sparkle grow over time
	sparkle.height += explosionTime;
	sparkle.width += explosionTime;
	// when sparkle is big enough move to game over
	if (sparkle.width > 15.0f)
		return true;
	return false;
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