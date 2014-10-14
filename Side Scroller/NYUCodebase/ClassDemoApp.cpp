#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp(){
	Init();
	done = false;
	gravity_y = -11.0f;
	timeLeftOver = 0.0f;
	levelTexture = LoadTexture("arne_sprites.png");

	readLevel();
}

ClassDemoApp::~ClassDemoApp(){
	SDL_Quit();
}

void ClassDemoApp::Init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Side Scroller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.6666, 2.6666, -2.0, 2.0, 1.0, 1.0);
	//glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
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

void ClassDemoApp::readLevel(){
	ifstream infile("myMap.txt");
	string line; 
	while (getline(infile, line)){
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			readEntityData(infile);
		}
	}
}

bool ClassDemoApp::readHeader(ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool ClassDemoApp::readLayerData(ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;

				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

bool ClassDemoApp::readEntityData(ifstream &stream) {
	string line;
	string type;

	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');

			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;

			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void ClassDemoApp::placeEntity(string type, float placeX, float placeY){
	if (type == "Enemy"){
		Entity enemy;
		enemy.x = placeX;
		enemy.y = placeY;
		enemy.isStatic = false;
		enemy.acceleration_x = 1.0;
		entities.push_back(&enemy);
	}
	if (type == "Start"){
		player.player = true;
		player.friction_x = 50.0f;
		player.isStatic = false;
		player.x = placeX;
		player.y = placeY;
		entities.push_back(&player);
	}
}

// shoots bullet at specified location
void ClassDemoApp::shootBullet(float x, float y, float direction){
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = x;
	bullets[bulletIndex].y = y;
	bullets[bulletIndex].direction = direction;
	//bullets[bulletIndex].size = 0.01f;
	bullets[bulletIndex].speed = 2.0f;
	bulletIndex++;

	// reset index if max bullet is reached and overwrite earlier bullets
	if (bulletIndex > MAX_BULLETS - 1)
		bulletIndex = 0;
}

bool ClassDemoApp::ProcessEvents(){
	SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	player.acceleration_x = 0.0f;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (event.type == SDL_KEYDOWN) {
			if (player.collidedBottom && event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				player.velocity_y = 2.0f;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_X){
				shootBullet(player.x + 0.01f, player.y, player.direction_x);
			}
		}
	}

	// left, right and space keys used by player
	if (keys[SDL_SCANCODE_LEFT] == 1){
		// if left pressed, set acceleration negative
		player.direction_x = -1.0f;
		player.acceleration_x = -20.0f;
	}
	if (keys[SDL_SCANCODE_RIGHT]){
		// if down pressed, set acceleration positive
		player.direction_x = 1.0f;
		player.acceleration_x = 20.0f;
	}

	return done;
}

void ClassDemoApp::fixedUpdate(){
	for (size_t i = 0; i < entities.size(); i++){
		entities[i]->collidedBottom = false;
	}

	for (size_t i = 0; i < entities.size(); i++){
		entities[i]->fixedUpdate();
		if (!entities[i]->isStatic){
			/*entities[i]->velocity_x += gravity_x*FIXEDTIMESTEP;*/
			entities[i]->velocity_y += gravity_y*FIXED_TIMESTEP;
		}
	}

	for (size_t i = 0; i < entities.size(); i++){
		// y-axis
		entities[i]->y += entities[i]->velocity_y *FIXED_TIMESTEP;
		if (!entities[i]->isStatic&& entities[i]->enableCollisions){
			// collision detection
			for (size_t j = 0; j < entities.size(); j++){
				if (i != j && entities[j]->isStatic){
					if (entities[i]->collidesWith(entities[j])){

						float yPenetration = fabs(fabs(entities[j]->y - entities[i]->y) - entities[i]->height / 2.0f
							- entities[j]->height / 2.0f);

						// check side of penetration
						if (entities[j]->y > entities[i]->y){
							entities[i]->y -= yPenetration + 0.001f;
							entities[i]->collidedTop = true;
						}
						else{
							entities[i]->y += yPenetration + 0.001f;
							entities[i]->collidedBottom = true;
						}
						entities[i]->velocity_y = 0.0f;

						// check if player collides with key and have player pick it up
						if (entities[i]->player && entities[j]->key){
							entities[j]->visible = false;
							entities[i]->key = true;
						}

						// check if player has key on the keyhole
						if (entities[i]->player && entities[j]->keyhole){
							entities[i]->key = false;
							// make key visible
							for (size_t i = 0; i < entities.size(); i++){
								if (!entities[i]->visible && entities[i]->key)
									entities[i]->visible = true;
							}
						}
					}
				}
			}
		}

		//// x-axis
		entities[i]->x += entities[i]->velocity_x *FIXED_TIMESTEP;
		// check if exiting sides
		if (entities[i]->x > 1.33f)
			entities[i]->x = 1.33f;
		if (entities[i]->x < -1.33f)
			entities[i]->x = -1.33f;
		//// collision detection
		//for (int j = 0; j < entities.size(); j++){
		//	if (i != j){
		//		if (entities[i]->collidesWith(entities[j])){
		//			float xPenetration = fabs(entities[j]->x - entities[i]->x) - entities[i]->width / 2.0f - entities[j]->width / 2.0f;
		//			// check side of penetration
		//			if (entities[j]->x > entities[i]->x){
		//				entities[i]->collidedRight = true;
		//				entities[j]->x -= xPenetration + 0.001f;
		//			}
		//			else{
		//				entities[i]->collidedLeft = true;
		//				entities[j]->x += xPenetration + 0.001f;
		//			}
		//			entities[j]->velocity_x = 0.0f;
		//		}
		//	}
		//}
	}

	// if offscreen, respawn
	if (player.y < -1.0f){
		player.x = -1.25f;
		player.y = -0.70f;
	}
}

void ClassDemoApp::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(player.x, player.y, 0.0f);
	for (size_t i = 0; i < entities.size(); i++){
		if (entities[i]->visible)
			entities[i]->draw();
	}
	glPopMatrix();
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