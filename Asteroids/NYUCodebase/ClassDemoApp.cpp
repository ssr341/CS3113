#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp(){
	Init();
	done = false;
	timeLeftOver = 0.0f;

	// player creation
	unsigned int playerTexture = LoadTexture("playerShip1_red.png");
	player.textureID = playerTexture;
	player.player = true;
	player.width = 0.2f;
	player.height = 0.2f;
	player.friction_x = 30.0f;
	player.friction_y = 30.0f;
	player.isStatic = false;
	player.y = 0.0f;
	player.x = 0.0f;
	player.enableCollisions = true;
	player.buildMatrix();
	entities.push_back(&player);

	// asteroid creation
	unsigned int asteroidTexture = LoadTexture("meteorBrown_big4.png");
	asteroid1.textureID = asteroidTexture;
	asteroid1.width = 0.25f;
	asteroid1.height = 0.25f;
	asteroid1.velocity_x = 1.0f;
	asteroid1.velocity_y = 1.5f;
	asteroid1.isStatic = false;
	asteroid1.y = -0.8f;
	asteroid1.x = -1.1f;
	asteroid1.enableCollisions = true;
	asteroid1.buildMatrix();
	entities.push_back(&asteroid1);

	/*asteroid2.textureID = asteroidTexture;
	asteroid2.width = 0.5f;
	asteroid2.height = 0.5f;
	asteroid2.velocity_x = -0.5f;
	asteroid2.velocity_y = -0.7f;
	asteroid2.isStatic = false;
	asteroid2.y = 0.8f;
	asteroid2.x = 1.1f;
	asteroid2.enableCollisions = true;
	asteroid2.buildMatrix();
	entities.push_back(&asteroid2);

	asteroid3.textureID = asteroidTexture;
	asteroid3.width = 1.0f;
	asteroid3.height = 1.0f;
	asteroid3.velocity_x = 0.25f;
	asteroid3.velocity_y = 0.1f;
	asteroid3.isStatic = false;
	asteroid3.y = 0.0f;
	asteroid3.x = -1.1f;
	asteroid3.enableCollisions = true;
	asteroid3.buildMatrix();
	entities.push_back(&asteroid3);*/
}

ClassDemoApp::~ClassDemoApp(){
	SDL_Quit();
}

void ClassDemoApp::Init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	//glOrtho(-2.6666, 2.6666, -2.0, 2.0, 1.0, 1.0);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
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

bool ClassDemoApp::ProcessEvents(){
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	player.acceleration_x = 0.0f;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}

	// arrow keys used by player
	if (keys[SDL_SCANCODE_LEFT]){
		// if left pressed, set x acceleration negative
		player.acceleration_x = -1.5f;
	}
	if (keys[SDL_SCANCODE_RIGHT]){
		// if right pressed, set x acceleration positive
		player.acceleration_x = 1.5f;
	}
	if (keys[SDL_SCANCODE_UP]){
		// if up pressed, set y acceleration positive
		player.acceleration_y = 1.5f;
	}
	if (keys[SDL_SCANCODE_DOWN]){
		// if up pressed, set y acceleration negative
		player.acceleration_y = -1.5f;
	}

	return done;
}

void ClassDemoApp::fixedUpdate(){
	for (size_t i = 0; i < entities.size(); i++){
		entities[i]->collidedBottom = false;
	}

	for (size_t i = 0; i < entities.size(); i++){
		entities[i]->fixedUpdate();
	}

	for (size_t i = 0; i < entities.size(); i++){
		// y-axis
		entities[i]->y += entities[i]->velocity_y *FIXED_TIMESTEP;
		// x-axis
		entities[i]->x += entities[i]->velocity_x *FIXED_TIMESTEP;
		if (!entities[i]->isStatic&& entities[i]->enableCollisions){
			// collision detection
			for (size_t j = 0; j < entities.size(); j++){
				if (i != j && entities[j]->isStatic){
					if (checkCollision(entities[i], entities[j])){
						Vector entityI(entities[i]->x, entities[i]->y, 0.0f);
						Vector entityJ(entities[j]->x, entities[j]->y, 0.0f);

						//transform to world
						entityI = entities[i]->matrix * entityI;
						entityJ = entities[j]->matrix * entityJ;

						entityI = entities[i]->matrix.inverse() * entityI;
						entityJ = entities[j]->matrix.inverse() * entityJ;

						//divide x and y of their position difference by distance between them
						float diffX = fabs(entityI.x - entityJ.x);
						float diffY = fabs(entityI.y - entityJ.y);

						//normalize vectors
						entityI.normalize();
						entityJ.normalize();

						entityI.x /= diffX;
						entityI.y /= diffY;
						entityJ.x /= diffX;
						entityJ.y /= diffY;

						entities[i]->x = entityI.x;
						entities[i]->y = entityI.y;
						entities[j]->x = entityJ.x;
						entities[j]->y = entityJ.y;
						
					}
				}
			}
			// check if exiting sides
			if (entities[i]->x > 1.33f)
				entities[i]->x = 1.33f;
			if (entities[i]->x < -1.33f)
				entities[i]->x = -1.33f;
		}
	}
}

bool ClassDemoApp::checkCollision(Entity* entity1, Entity* entity2){
	if (entity1 == entity2)
		return false;
	entity1->buildMatrix();
	entity2->buildMatrix();

	Matrix entity1Inverse = entity1->matrix.inverse();
	Matrix entity2Inverse = entity2->matrix.inverse();

	// for entity 2
	// get points
	Vector ent2TL = Vector(-entity2->width*0.5, entity2->height*0.5, 0.0);  // top left
	Vector ent2TR = Vector(entity2->width*0.5, entity2->height*0.5, 0.0);   // top right
	Vector ent2BL = Vector(-entity2->width*0.5, -entity2->height*0.5, 0.0); // bottom left
	Vector ent2BR = Vector(-entity2->width*0.5, -entity2->height*0.5, 0.0); // bottom right

	//transform to world
	ent2TL = entity2->matrix * ent2TL;
	ent2TR = entity2->matrix * ent2TR;
	ent2BL = entity2->matrix * ent2BL;
	ent2BR = entity2->matrix * ent2BR;

	ent2TL = entity1Inverse * ent2TL;
	ent2TR = entity1Inverse * ent2TR;
	ent2BL = entity1Inverse * ent2BL;
	ent2BR = entity1Inverse * ent2BR;

	// check X axis
	float minX = std::min(std::min(std::min(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	float maxX = std::max(std::max(std::max(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);

	// check Y axis
	float minY = std::min(std::min(std::min(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	float maxY = std::max(std::max(std::max(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);

	// if no collision, return false
	if (!(minX <= entity1->width*0.5 && maxX >= entity1->width*-0.5))
		return false;
	if (!(minY <= entity1->height*0.5 && maxY >= entity1->height*-0.5))
		return false;

	// for entity1
	// get points
	Vector ent1TL = Vector(-entity1->width*0.5, entity1->height*0.5, 0.0);  // top left
	Vector ent1TR = Vector(entity1->width*0.5, entity1->height*0.5, 0.0);   // top right
	Vector ent1BL = Vector(-entity1->width*0.5, -entity1->height*0.5, 0.0); // bottom left
	Vector ent1BR = Vector(-entity1->width*0.5, -entity1->height*0.5, 0.0); // bottom right

	//transform to world
	ent1TL = entity1->matrix * ent1TL;
	ent1TR = entity1->matrix * ent1TR;
	ent1BL = entity1->matrix * ent1BL;
	ent1BR = entity1->matrix * ent1BR;

	ent1TL = entity2Inverse * ent1TL;
	ent1TR = entity2Inverse * ent1TR;
	ent1BL = entity2Inverse * ent1BL;
	ent1BR = entity2Inverse * ent1BR;

	// check X axis
	minX = std::min(std::min(std::min(ent1TL.x, ent1BL.x), ent1TR.x), ent1BR.x);
	maxX = std::max(std::max(std::max(ent1TL.x, ent1BL.x), ent1TR.x), ent1BR.x);

	// check Y axis
	minY = std::min(std::min(std::min(ent1TL.y, ent1BL.y), ent1TR.y), ent1BR.y);
	maxY = std::max(std::max(std::max(ent1TL.y, ent1BL.y), ent1TR.y), ent1BR.y);

	// if no collision, return false
	if (!(minX <= entity2->width*0.5 && maxX >= entity2->width*-0.5))
		return false;
	if (!(minY <= entity2->height*0.5 && maxY >= entity2->height*-0.5))
		return false;

	return true;
}

void ClassDemoApp::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	for (size_t i = 0; i < entities.size(); i++){
		if (entities[i]->visible)
			entities[i]->draw();
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