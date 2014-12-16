#include "StageTwo.h"

StageTwo::StageTwo(){
	winner = 0;
	freeze = false;

	hitsToKill = 3;
	player1Hits = 0;
	player2Hits = 0;

	bulletIndex = 0;
	enemyShot = 0.0f;
	enemyNum = 3;
	enemyBulletSize = 0.025f;
	enemyBulletSpeed = 1.0f;
	respawnTimer = 0.15f;

	player1Shot = 10.0f;
	player2Shot = 10.0f;
	player1ShotTime = 0.025f;
	player2ShotTime = 0.025f;
	player1KillCount = 0;
	player2KillCount = 0;
	player1BulletSize = 0.025f;
	player1BulletSpeed = 2.0f;
	player2BulletSize = 0.025f;
	player2BulletSpeed = 2.0f;

	for (int i = 0; i < MAX_BULLETS; i++){
		bullets[i].x = -10.0f;
		bullets[i].y = -10.0f;
		bullets[i].direction = 0.0f;
		bullets[i].shooter = -1.0f;
		bullets[i].speed = 0.0f;
		bullets[i].size = 0.0f;
	}
}

StageTwo::~StageTwo(){
	for (size_t i = 0; i < enemies1.size(); i++)
		delete enemies1[i];
	for (size_t i = 0; i < enemies2.size(); i++)
		delete enemies2[i];
	Mix_FreeChunk(shootingSound);
	Mix_FreeChunk(explosionSound);
	Mix_FreeChunk(powerupSound);
}

GLuint StageTwo::LoadTexture(const char *image_path) {
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

void StageTwo::Init(){
	// player creation
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
	player2shield.rotate = false;
	player2shield.rotation = 0.0f;
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

	// asteroid creation
	asteroid.textureID = LoadTexture("meteorSmall.png");
	asteroid.height = 0.4f;
	asteroid.width = 0.4f;
	asteroid.x = 0.0f;
	asteroid.y = 0.0f;
	asteroid.rotate = true;
	asteroid.rotation = 0.0f;
	asteroid.friction_y = 1.0f;
	asteroid.acceleration_y = 0.5f;
	asteroid.velocity_y = 0.0f;
	asteroid.visible = true;

	// enemy creation
	float enemy1X = -0.3f;
	float enemyY = 0.3f;
	float enemy2X = 0.3f;
	for (int i = 0; i < enemyNum; i++){
		Entity* enemy1 = new Entity;
		enemy1->textureID = LoadTexture("enemyUFO.png");
		enemy1->width = 0.2f;
		enemy1->height = 0.2f;
		enemy1->x = enemy1X;
		enemy1->y = enemyY;
		enemy1->rotate = false;
		enemy1->rotation = 0.0f;
		enemy1->velocity_y = 0.0f;
		enemy1->acceleration_y = -1.0f;
		enemy1->friction_y = 0.5f;
		enemy1->visible = true;
		enemy1->deadTime = 0.0f;
		enemies1.push_back(enemy1);

		Entity* enemy2 = new Entity;
		enemy2->textureID = LoadTexture("enemyUFO.png");
		enemy2->width = 0.2f;
		enemy2->height = 0.2f;
		enemy2->x = enemy2X;
		enemy2->y = enemyY;
		enemy2->rotate = false;
		enemy2->rotation = 0.0f;
		enemy2->velocity_y = 0.0f;
		enemy2->acceleration_y = -1.0f;
		enemy2->friction_y = 0.5f;
		enemy2->visible = true;
		enemy2->deadTime = 0.0f;
		enemies2.push_back(enemy2);

		enemyY -= 0.2f;
	}
}

// reset the stage. called when user wants to play again
void StageTwo::reset(){
	winner = 0;
	freeze = false;
	explosionTime = 0.0f;

	bulletIndex = 0;

	player1Hits = 0;
	player2Hits = 0;
	enemyShot = 0.0f;

	player1Shot = 10.0f;
	player2Shot = 10.0f;
	player1ShotTime = 0.025f;
	player2ShotTime = 0.025f;
	player1KillCount = 0;
	player2KillCount = 0;
	player1BulletSize = 0.025f;
	player1BulletSpeed = 2.0f;
	player2BulletSize = 0.025f;
	player2BulletSpeed = 2.0f;

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

	asteroid.x = 0.0f;
	asteroid.y = 0.0f;
	asteroid.rotation = 0.0f;
	asteroid.acceleration_y = 0.5f;
	asteroid.velocity_y = 0.0f;

	for (int i = 0; i < MAX_BULLETS; i++){
		bullets[i].x = -10.0f;
		bullets[i].y = -10.0f;
		bullets[i].direction = 0.0f;
		bullets[i].shooter = -1.0f;
		bullets[i].speed = 0.0f;
		bullets[i].size = 0.0f;
	}

	float enemyY = 0.3f;
	for (int i = 0; i < enemyNum; i++){
		enemies1[i]->x = -0.3f;;
		enemies1[i]->y = enemyY;
		enemies1[i]->velocity_y = 0.0f;
		enemies1[i]->acceleration_y = -1.0f;
		enemies1[i]->friction_y = 0.5f;
		enemies1[i]->visible = true;
		enemies1[i]->deadTime = 0.0f;

		enemies2[i]->x = 0.3f;
		enemies2[i]->y = enemyY;
		enemies2[i]->velocity_y = 0.0f;
		enemies2[i]->acceleration_y = -1.0f;
		enemies2[i]->friction_y = 0.5f;
		enemies2[i]->visible = true;
		enemies2[i]->deadTime = 0.0f;

		enemyY -= 0.2f;
	}
}

// process player shooting or quit
void StageTwo::ProcessShoot(SDL_Event* event, bool& done){
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

// process player movement
void StageTwo::ProcessEvents(){
	player1.acceleration_y = 0.0f;
	player2.acceleration_y = 0.0f;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	// w, s and f keys used by player 1
	if (keys[SDL_SCANCODE_W]){
		// if w pressed, set acceleration positive
		if (player1KillCount >= 10)
			player1.acceleration_y = 4.5f;
		else
			player1.acceleration_y = 3.5f;
	}
	if (keys[SDL_SCANCODE_S]){
		// if s pressed, set acceleration negative
		if (player1KillCount >= 10)
			player1.acceleration_y = -4.5f;
		else
			player1.acceleration_y = -3.5f;
	}

	// up, down and 0 keys used by player 2
	if (keys[SDL_SCANCODE_UP]){
		// if up pressed, set acceleration positive
		if (player2KillCount >= 10)
			player2.acceleration_y = 4.5f;
		else
			player2.acceleration_y = 3.5f;
	}
	if (keys[SDL_SCANCODE_DOWN]){
		// if down pressed, set acceleration negative
		if (player2KillCount >= 10)
			player2.acceleration_y = -4.5f;
		else
			player2.acceleration_y = -3.5f;
	}
}

int StageTwo::fixedUpdate(float fixedElapsed){
	// if a player isn't dead yet
	if (!freeze){
		// run fixed update for everything
		player1.fixedUpdate();
		player2.fixedUpdate();
		asteroid.fixedUpdate();
		for (size_t i = 0; i < enemies1.size(); i++){
			enemies1[i]->fixedUpdate();
		}
		for (size_t i = 0; i < enemies2.size(); i++){
			enemies2[i]->fixedUpdate();
		}

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

		// have asteroid reverse when meets edge of screen
		if (asteroid.y > 0.65)
			asteroid.acceleration_y = -1.0f;
		if (asteroid.y < -0.65)
			asteroid.acceleration_y = 1.0f;

		// have enemies reverse when they meet edge of screen
		if (enemies1[0]->y >= 0.45){
			for (size_t i = 0; i < enemies1.size(); i++){
				enemies1[i]->acceleration_y = -1.0f;
			}
		}
		if (enemies1[enemyNum - 1]->y <= -0.45){
			for (size_t i = 0; i < enemies1.size(); i++){
				enemies1[i]->acceleration_y = 1.0f;
			}
		}
		if (enemies2[0]->y >= 0.45){
			for (size_t i = 0; i < enemies2.size(); i++){
				enemies2[i]->acceleration_y = -1.0f;
			}
		}
		if (enemies2[enemyNum - 1]->y <= -0.45){
			for (size_t i = 0; i < enemies2.size(); i++){
				enemies2[i]->acceleration_y = 1.0f;
			}
		}

		// check for enemy collision with bullets
		for (size_t i = 0; i < enemies1.size(); i++){
			for (int j = 0; j < MAX_BULLETS; j++){
				// if enemy already dead ignore
				if (enemies1[i]->visible && bullets[j].visible && bullets[j].shooter != 2){
					// if bullet is visible and colliding
					if (enemies1[i]->collidesWith(bullets[j])){
						// make enemy and bullet invisible
						Mix_PlayChannel(-1, explosionSound, 0);
						enemies1[i]->visible = false;
						bullets[j].visible = false;
						if (bullets[j].shooter == 0)
							player1KillCount++;
						if (bullets[j].shooter == 1)
							player2KillCount++;
					}
				}
			}
		}

		// check for enemy collision with bullets
		for (size_t i = 0; i < enemies2.size(); i++){
			for (int j = 0; j < MAX_BULLETS; j++){
				// if enemy already dead ignore
				if (enemies2[i]->visible && bullets[j].visible && bullets[j].shooter != 3){
					// if bullet is visible and colliding
					if (enemies2[i]->collidesWith(bullets[j])){
						// make enemy and bullet invisible
						Mix_PlayChannel(-1, explosionSound, 0);
						enemies2[i]->visible = false;
						bullets[j].visible = false;
						if (bullets[j].shooter == 0)
							player1KillCount++;
						if (bullets[j].shooter == 1)
							player2KillCount++;
					}
				}
			}
		}

		// update respawn timers
		for (size_t i = 0; i < enemies1.size(); i++){
			if (!enemies1[i]->visible)
				enemies1[i]->deadTime += fixedElapsed;
		}
		for (size_t i = 0; i < enemies2.size(); i++){
			if (!enemies2[i]->visible)
				enemies2[i]->deadTime += fixedElapsed;
		}

		// check for respawns
		for (size_t i = 0; i < enemies1.size(); i++){
			if (!enemies1[i]->visible && enemies1[i]->deadTime > respawnTimer){
				enemies1[i]->visible = true;
				enemies1[i]->deadTime = 0.0;
			}
		}
		for (size_t i = 0; i < enemies2.size(); i++){
			if (!enemies2[i]->visible && enemies2[i]->deadTime > respawnTimer){
				enemies2[i]->visible = true;
				enemies2[i]->deadTime = 0.0;
			}
		}

		// have bullets colliding with asteroid vanish
		for (int i = 0; i < MAX_BULLETS; i++){
			if (bullets[i].visible && asteroid.collidesWith(bullets[i])){
				Mix_PlayChannel(-1, explosionSound, 0);
				bullets[i].visible = false;
			}
		}

		// check for bullet collision with player
		for (int i = 0; i < MAX_BULLETS; i++){
			// if bullet is visible and colliding
			if (bullets[i].visible && bullets[i].shooter != 0 && player1.collidesWith(bullets[i])){
				if (player1Hits < hitsToKill - 1){
					player1Hits++;
					bullets[i].visible = false;
				}
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
				if (player2Hits < hitsToKill - 1){
					player2Hits++;
					bullets[i].visible = false;
				}
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
		if (enemyShot >= 0.10f){    // shoot every 10 frames
			bool shot1 = false;  // was the bullet shot?
			bool shot2 = false;  // was the bullet shot?
			while (!shot1 && !shot2){
				int enemyBulletX = rand() % (enemyNum);
				// used to check for alive enemies
				int enemy1visCounter = 0;
				int enemy2visCounter = 0;
				for (size_t i = 0; i < enemies1.size(); i++){
					if (enemies1[i]->visible)
						++enemy1visCounter;
				}
				for (size_t i = 0; i < enemies2.size(); i++){
					if (enemies2[2]->visible)
						++enemy2visCounter;
				}

				if (enemies1[enemyBulletX]->visible && !shot1){
					Mix_PlayChannel(-1, shootingSound, 0);
					shootBullet(enemies1[enemyBulletX]->x, enemies1[enemyBulletX]->y, -1.0f, 2, enemyBulletSize, enemyBulletSpeed);
					shot1 = true;
				}
				else if (enemy1visCounter == 0)  // if no enemies are visible, mark shot as fired
					shot1 = true;
				if (enemies2[enemyBulletX]->visible && !shot2){
					Mix_PlayChannel(-1, shootingSound, 0);
					shootBullet(enemies2[enemyBulletX]->x, enemies2[enemyBulletX]->y, 1.0f, 3, enemyBulletSize, enemyBulletSpeed);
					shot2 = true;
				}
				else if (enemy2visCounter == 0)  // if no enemies are visible, mark shot as fired
					shot2 = true;
			}
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

		// update for powerups
		if (player1KillCount >= 3 && player1BulletSpeed == 2.0f){
			player1BulletSpeed *= 2.0f;
			Mix_PlayChannel(-1, powerupSound, 0);
		}
		if (player1KillCount >= 6 && player1BulletSize == 0.025f){
			player1BulletSize = 0.04f;
			Mix_PlayChannel(-1, powerupSound, 0);
		}
		if (player2KillCount >= 3 && player2BulletSpeed == 2.0f){
			player2BulletSpeed *= 2.0f;
			Mix_PlayChannel(-1, powerupSound, 0);
		}
		if (player2KillCount >= 6 && player2BulletSize == 0.025f){
			player2BulletSize = 0.04f;
			Mix_PlayChannel(-1, powerupSound, 0);
		}
		if (player1KillCount >= 10 && player1ShotTime == 0.025f){
			player1ShotTime /= 2.0f;
			Mix_PlayChannel(-1, powerupSound, 0);
		}
		if (player2KillCount >= 10 && player2ShotTime == 0.025f){
			player2ShotTime /= 2.0f;
			Mix_PlayChannel(-1, powerupSound, 0);
		}
	}
	return winner;
}

// draw stuff on screen
void StageTwo::Render(){
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
	asteroid.draw();

	for (size_t i = 0; i < enemies1.size(); i++){
		if (enemies1[i]->visible)
			enemies1[i]->draw();
	}
	for (size_t i = 0; i < enemies2.size(); i++){
		if (enemies2[i]->visible)
			enemies2[i]->draw();
	}

	// if a player is alive, draw bullets
	if (!freeze){
		for (int i = 0; i < MAX_BULLETS; i++){
			// only draw bullets that haven't collided or are on screen
			if (bullets[i].visible)
				bullets[i].Draw();
		}
	}

	// if player is dead, draw explosion
	if (freeze)
		sparkle.draw();
}

// have losing player's ship explode
bool StageTwo::explosion(float fixedElapsed){
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
	// when explosion big enough, move to next stage
	if (sparkle.width > 15.0f)
		return true;
	return false;
}

// shoots bullet at specified location
void StageTwo::shootBullet(float x, float y, float direction, float shooter, float size, float speed){
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