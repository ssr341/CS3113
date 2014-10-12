#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp(){
	Init();
	done = false;
	gravity_y = -11.0f;
	timeLeftOver = 0.0f;

	// player creation
	unsigned int playerTexture = LoadTexture("alienPink.png");
	player.textureID = playerTexture;
	player.player = true;
	player.width = 0.2f;
	player.height = 0.2f;
	player.friction_x = 50.0f;
	player.isStatic = false;
	player.y = -0.70f;
	player.x = -1.25f;
	/*player.y = 0.76f;
	player.x = -1.0f;*/
	player.enableCollisions = true;
	entities.push_back(&player);

	buildLevel();
}

ClassDemoApp::~ClassDemoApp(){
	SDL_Quit();
}

void ClassDemoApp::Init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Key Holed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
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

void ClassDemoApp::buildLevel(){
	unsigned int blockTexture = LoadTexture("boxEmpty.png");
	unsigned int logTexture = LoadTexture("bridgeLogs.png");
	unsigned int keyTexture = LoadTexture("keyRed.png");
	unsigned int bridgeTexture = LoadTexture("bridge.png");
	unsigned int holeTexture = LoadTexture("lockRed.png");

	int blockIndex = 0;

	// keyhole
	keyhole.textureID = holeTexture;
	keyhole.width = 0.15f;
	keyhole.height = 0.2f;
	keyhole.isStatic = true;
	keyhole.enableCollisions = true;
	keyhole.keyhole = true;
	keyhole.y = -0.9f;
	keyhole.x = -1.28f;
	entities.push_back(&keyhole);

	// starting platform
	for (int i = 0; i < 5; i++){
		//blocks[blockIndex].sprite = SheetSprite(blockTexture, 2, 16, 8);  // draw 2nd block. sheet is 16 units wide, 8 high
		blocks[blockIndex].textureID = blockTexture;
		blocks[blockIndex].width = 0.15f;
		blocks[blockIndex].height = 0.25f;
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].enableCollisions = true;
		/*blocks[blockIndex].y = (2.0f / 8.0f * -4) - 1.0f / 8.0f;
		blocks[blockIndex].x = (-2.0f / 8.0f * 4) + (2.0f / 8.0f * i);*/
		blocks[blockIndex].y = -0.93f;
		blocks[blockIndex].x = -1.135f + 0.15f * i;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
	}

	// key platform
	for (int i = 0; i < 4; i++){
		blocks[blockIndex].textureID = logTexture;
		blocks[blockIndex].width = 0.2f;
		blocks[blockIndex].height = 0.2f;
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].enableCollisions = true;
		blocks[blockIndex].y = 0.75f;
		blocks[blockIndex].x = -1.33f + 2.0f / 10.0f * i;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
	}

	// key
	key.textureID = keyTexture;
	key.key = true;
	key.width = 0.25f;
	key.height = 0.25f;
	key.isStatic = true;
	key.enableCollisions = false;
	key.y = 0.76f;
	key.x = -1.26f;
	entities.push_back(&key);

	// static platforms
	float1.textureID = bridgeTexture;
	float1.width = 0.3f;
	float1.height = 0.2f;
	float1.isStatic = true;
	float1.enableCollisions = true;
	float1.y = -0.45f;
	float1.x = -0.25f;
	entities.push_back(&float1);

	float2.textureID = bridgeTexture;
	float2.width = 0.3f;
	float2.height = 0.2f;
	float2.isStatic = true;
	float2.enableCollisions = true;
	float2.y = -0.25f;
	float2.x = 0.3f;
	entities.push_back(&float2);

	// right side platform
	for (int i = 0; i < 4; i++){
		blocks[blockIndex].textureID = logTexture;
		blocks[blockIndex].width = 0.2f;
		blocks[blockIndex].height = 0.2f;
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].enableCollisions = true;
		blocks[blockIndex].y = 0.0f;
		blocks[blockIndex].x = 1.33f - 2.0f / 10.0f * i;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
	}

	// statics platforms near the top
	float3.textureID = bridgeTexture;
	float3.width = 0.3f;
	float3.height = 0.2f;
	float3.isStatic = true;
	float3.enableCollisions = true;
	float3.y = 0.4f;
	float3.x = 1.2f;
	entities.push_back(&float3);

	float4.textureID = bridgeTexture;
	float4.width = 0.3f;
	float4.height = 0.2f;
	float4.isStatic = true;
	float4.enableCollisions = true;
	float4.y = 0.55f;
	float4.x = 0.5f;
	entities.push_back(&float4);

	float5.textureID = bridgeTexture;
	float5.width = 0.3f;
	float5.height = 0.2f;
	float5.isStatic = true;
	float5.enableCollisions = true;
	float5.y = 0.75f;
	float5.x = -0.15f;
	entities.push_back(&float5);
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
		if(entities[i]->x > 1.33f)
			entities[i]->x = 1.33f;
		if(entities[i]->x < -1.33f)
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