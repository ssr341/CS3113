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
	glClearColor(0.2f, 0.6f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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
	// load texture for all entities
	for (size_t i = 0; i < entities.size(); i++){
		entities[i]->textureID = levelTexture;
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
		Entity* enemy = new Entity;
		enemy->x = placeX;
		enemy->y = placeY;
		enemy->isStatic = false;
		enemy->enableCollisions = true;
		enemy->acceleration_x = 1.0;
		enemy->enemy = true;
		entities.push_back(enemy);
	}
	else if (type == "Start"){
		spawnLocX = placeX;
		spawnLocY = placeY;
		player.player = true;
		player.friction_x = 50.0f;
		player.isStatic = false;
		player.enableCollisions = true;
		player.x = placeX;
		player.y = placeY;
		entities.push_back(&player);
	}
}

void ClassDemoApp::drawBlocks(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, levelTexture);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(-TILE_SIZE * mapWidth / 2, TILE_SIZE * mapHeight / 2, 0.0f);

	vector<GLfloat>vertexData;
	vector<GLfloat>texCoordData;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0){
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, vertexData.size()/2);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void ClassDemoApp::entityDraw(Entity* entity){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, levelTexture);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(entity->x, entity->y, 0.0f);

	int index = 0;
	if (entity->enemy)
		index = 78;
	else if (entity->player)
		index = 81;

	float u = (float)(((int)index) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
	float v = (float)(((int)index) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
	float spriteWidth = 1.0 / (float)SPRITE_COUNT_X;
	float spriteHeight = 1.0 / (float)SPRITE_COUNT_Y;

	//GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	GLfloat quad[] = { -0.5f*TILE_SIZE, 0.5f*TILE_SIZE, -0.5f*TILE_SIZE, -0.5f*TILE_SIZE, 0.5f*TILE_SIZE, -0.5f*TILE_SIZE, 
		0.5f*TILE_SIZE, 0.5f*TILE_SIZE };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat uvs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
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
		}
	}

	// left, right and space keys used by player
	if (keys[SDL_SCANCODE_LEFT] == 1){
		// if left pressed, set acceleration negative
		player.acceleration_x = -20.0f;
	}
	if (keys[SDL_SCANCODE_RIGHT]){
		// if down pressed, set acceleration positive
		player.acceleration_x = 20.0f;
	}

	return done;
}

void ClassDemoApp::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)((worldX + (WORLD_OFFSET_X)) / TILE_SIZE);
	*gridY = (int)((-worldY + (WORLD_OFFSET_Y)) / TILE_SIZE);
}

void ClassDemoApp::doLevelCollisionY(Entity* entity){
	//check bottom
	float adjust = checkPointForGridCollisionY(entity->x, entity->y - entity->height*0.5);
	if (adjust != 0.0f){
		entity->y += adjust;
		entity->velocity_y = 0.0f;
		entity->collidedBottom = true;
	}

	//check top
	adjust = checkPointForGridCollisionY(entity->x, entity->y + entity->height*0.5);
	if (adjust != 0.0f){
		entity->y += adjust - TILE_SIZE;
		entity->velocity_y = 0.0f;
		entity->collidedTop = true;
	}
}

float ClassDemoApp::checkPointForGridCollisionY(float x, float y){
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 21 || gridY < 0 || gridY > 15){
		return 0.0f;
	}
	if (isSolid(levelData[gridY][gridX])){
		float yCoordinate = (float)((gridY*TILE_SIZE) - (TILE_SIZE*8.0));
		return -y - yCoordinate;
	}
	return 0.0f;
}

void ClassDemoApp::doLevelCollisionX(Entity* entity){
	//check left
	float adjust = checkPointForGridCollisionX(entity->x - entity->height*0.5, entity->y);
	if (adjust != 0.0f){
		entity->y += adjust;
		entity->velocity_y = 0.0f;
		entity->collidedLeft = true;
	}

	//check right
	adjust = checkPointForGridCollisionX(entity->x + entity->height*0.5, entity->y);
	if (adjust != 0.0f){
		entity->y += adjust - TILE_SIZE;
		entity->velocity_y = 0.0f;
		entity->collidedRight = true;
	}
}

float ClassDemoApp::checkPointForGridCollisionX(float x, float y){
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 21 || gridY < 0 || gridY > 15){
		return 0.0f;
	}
	if (isSolid(levelData[gridY][gridX])){
		float xCoordinate = (float)((gridX*TILE_SIZE) - (TILE_SIZE*16.0));
		return -x - xCoordinate;
	}
	return 0.0f;
}

bool ClassDemoApp::isSolid(unsigned char index){
	// if clouds or empty space return false
	if (index == 0 || index == 97 || index == 98 || index == 114 || index == 115 || index == 116)
		return false;
	else
		return true;
}

void ClassDemoApp::fixedUpdate(){
	// check if enemies collided with walls and set flags false
	for (size_t i = 0; i < entities.size(); i++){
		if ((entities[i]->collidedLeft || entities[i]->collidedRight) && entities[i]->enemy)
			entities[i]->acceleration_x *= -1.0;

		entities[i]->collidedBottom = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;
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
		// x-axis
		entities[i]->x += entities[i]->velocity_x *FIXED_TIMESTEP;
		if (!entities[i]->isStatic && entities[i]->enableCollisions){
			// collision detection with tiles
			doLevelCollisionY(entities[i]);
			doLevelCollisionX(entities[i]);

			// collision detection with other entities
			for (size_t j = 0; j < entities.size(); j++){
				if ((i != j) && (entities[i]->collidesWith(entities[j]))){
					if (entities[i]->player || entities[j]->player){
						player.x = spawnLocX;
						player.y = spawnLocY;
					}
				}
			}
		}

		// check if exiting sides
//		if (entities[i]->x > 1.33f)
//			entities[i]->x = 1.33f;
//		if (entities[i]->x < -1.33f)
//			entities[i]->x = -1.33f;
	}

	// if offscreen, respawn
	if (player.y < -2.0f){
		player.x = spawnLocX;
		player.y = spawnLocY;
	}
}

void ClassDemoApp::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-player.x, -player.y, 0.0f);
	// draw level
	drawBlocks();
	// draw entities
	for (size_t i = 0; i < entities.size(); i++){
		if (entities[i]->visible)
			entityDraw(entities[i]);
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