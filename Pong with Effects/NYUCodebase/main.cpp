// screen shake not working, unsure how to use squash

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
#include <SDL_mixer.h>

using namespace std;

SDL_Window* displayWindow;
bool ballScorer = true;  // person who scored last ball. True 1, False 2
int scoreCount = 0;
bool screenShake = false; // shake on collision
float stageAnimationTime = 0.0f;
float screenShakeValue = 0.0f;
Mix_Chunk* collideSound; // sound of collision
Mix_Chunk* scoreSound; // sound when score
Mix_Music* music;   // music that plays whole game

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
	float retVal = dstMin + ((value - srcMin) / (srcMax - srcMin) * (dstMax - dstMin));
	if (retVal < dstMin) {
		retVal = dstMin;
	}
	if (retVal > dstMax) {
		retVal = dstMax;
	}
	return retVal;
}

class Entity{
public:
	void Draw(){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// map velocity_y 0.0 - 5.0 to 1.0 - 1.6 Y scale and 1.0 - 0.8 X scale
		float scale_y = mapValue(fabs(speed), 0.0, 5.0, 1.0, 1.6);
		float scale_x = mapValue(fabs(speed), 5.0, 0.0, 0.8, 1.0);
		glTranslatef(x, y, 0.0);
		//glScalef(width, height, 0.0);
		glScalef(width*scale_x, height*scale_y, 0.0);
		//glRotatef(rotation, 0.0, 0.0, 1.0);
		GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
	void fireDraw(){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glScalef(width, height, 0.0);
		//glRotatef(rotation, 0.0, 0.0, 1.0);
		GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}

	void setSize(float w, float h){
		width = w;
		height = h;
	}

	float x;
	float y;
	float rotation;

	int textureID;

	float width;
	float height;

	float speed;
	float direction_x;
	float direction_y;
};

// entities to be used throughout the code
Entity ball;
Entity fire;
Entity paddle1;
Entity paddle2;

float lerp(float from, float to, float time) {
	return (1.0 - time)*from + time*to;
}

float easeOut(float from, float to, float time) {
	float oneMinusT = 1.0f - time;
	float tVal = 1.0f - (oneMinusT * oneMinusT * oneMinusT *
		oneMinusT * oneMinusT);
	return (1.0f - tVal)*from + tVal*to;
}

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textureID;
}

void Setup(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	collideSound = Mix_LoadWAV("collision.wav");
	scoreSound = Mix_LoadWAV("score.wav");
	music = Mix_LoadMUS("background.mp3"); // stolen from Mario Tennis music. Sorry Nintendo.
	Mix_PlayMusic(music, -1);

}

bool ProcessEvents(){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	// arrow keys used for player 1
	if (keys[SDL_SCANCODE_W]){
		// if w pressed, set direction positive
		paddle2.direction_y = 1.0;
	}
	else if (keys[SDL_SCANCODE_S]){
		// if s pressed, set direction negative
		paddle2.direction_y = -1.0;
	}

	// w and s keys used for player 2
	if (keys[SDL_SCANCODE_UP] == 1){
		// if up pressed, set direction positive
		paddle1.direction_y = 1.0;
	}
	else if (keys[SDL_SCANCODE_DOWN]){
		// if down pressed, set direction negative
		paddle1.direction_y = -1.0;
	}
	

	// ball movement and collision
	if (ball.x == 0 && ball.y == 0 && ball.direction_x == 0 && ball.direction_y == 0){  // ball is neutral
		if (ballScorer)  // go to right if player 1 last scored
			ball.direction_x = 1.0;
		else             // go to left if player 2 last scored
			ball.direction_x = -1.0;
	}

	float ballRight = ball.x + ball.width / 2;
	float ballLeft = ball.x + ball.width / -2;
	float ballTop = ball.y + ball.height / 2;
	float ballBottom = ball.y + ball.height / -2;

	float paddle1Right = paddle1.x + paddle1.width / 2;
	float paddle1Left = paddle1.x + paddle1.width / -2;
	float paddle1Top = paddle1.y + paddle1.height / 2;
	float paddle1Bottom = paddle1.y + paddle1.height / -2;

	float paddle2Right = paddle2.x + paddle2.width / 2;
	float paddle2Left = paddle2.x + paddle2.width / -2;
	float paddle2Top = paddle2.y + paddle2.height / 2;
	float paddle2Bottom = paddle2.y + paddle2.height / -2;

	// check for collision with paddle1
	// collides with right of paddle
	if ((ballLeft <= paddle1Right) && (ballTop >= paddle1Bottom) && (ballBottom <= paddle1Top)){
		ball.direction_x *= -1;  // reverse x direction
		ball.direction_y = paddle1.direction_y * paddle1.speed; // set y direction
		Mix_PlayChannel(-1, collideSound, 0);
		screenShake = true;
	}
	// collides with bottom of paddle
	if ((ballTop >= paddle1Bottom) && (ballBottom <= paddle1Top) && (ballLeft <= paddle1Right)){
		ball.direction_x *= -1;  // reverse x direction
		ball.direction_y = paddle1.direction_y * paddle1.speed; // set y direction
		Mix_PlayChannel(-1, collideSound, 0);
		screenShake = true;
	}
	// collides with top of paddle
	if ((ballBottom <= paddle1Top) && (ballLeft <= paddle1Right) && (ballTop >= paddle1Bottom)){
		ball.direction_x *= -1;  // reverse x direction
		ball.direction_y = paddle1.direction_y * paddle1.speed; // set y direction
		Mix_PlayChannel(-1, collideSound, 0);
		screenShake = true;
	}

	// check for collision with paddle2
	if ((ballRight >= paddle2Left) && (ballBottom <= paddle2Top) && (ballTop >= paddle2Bottom)){
		ball.direction_x *= -1;  // reverse x direction
		ball.direction_y = paddle2.direction_y * paddle2.speed; // set y direction
		Mix_PlayChannel(-1, collideSound, 0);
		screenShake = true;
	}
	// collides with bottom of paddle
	if ((ballTop >= paddle2Bottom) && (ballBottom <= paddle2Top) && (ballRight >= paddle2Left)){
		ball.direction_x *= -1;  // reverse x direction
		ball.direction_y = paddle2.direction_y * paddle2.speed; // set y direction
		Mix_PlayChannel(-1, collideSound, 0);
		screenShake = true;
	}
	// collides with top of paddle
	if ((ballBottom <= paddle2Top) && (ballRight >= paddle2Right) && (ballTop >= paddle2Bottom)){
		ball.direction_x *= -1;  // reverse x direction
		ball.direction_y = paddle2.direction_y * paddle2.speed; // set y direction
		Mix_PlayChannel(-1, collideSound, 0);
		screenShake = true;
	}

	// check collision with ceilings
	if ((ballTop >= 1.0) || (ballBottom <= -1.0)){
		ball.direction_y *= -1.0;
		Mix_PlayChannel(-1, collideSound, 0);
	}

	return true;
}

void update(float elapsed){
	// paddle 1
	paddle1.y += paddle1.direction_y * elapsed;
	paddle1.speed += fabs(paddle1.direction_y * elapsed)/1000 + 0.0001f;
	// screen boundaries for paddle 1
	if (paddle1.y > 0.825f)
		paddle1.y = 0.825f;
	if (paddle1.y < -0.825f)
		paddle1.y = -0.825f;
	paddle1.direction_y = 0;  // stops further movement

	// paddle 2
	paddle2.y += paddle2.direction_y * elapsed;
	paddle2.speed += fabs(paddle2.direction_y * elapsed)/1000 + 0.0001f;
	// screen boundaries for paddle 2
	if (paddle2.y > 0.825f)
		paddle2.y = 0.825f; 
	if (paddle2.y < -0.825f)
		paddle2.y = -0.825f;
	paddle2.direction_y = 0;  // stops further movement

	// ball
	ball.x += ball.direction_x * elapsed;
	ball.y += ball.direction_y * elapsed;
	fire.x = ball.x;
	fire.y = ball.y;

	stageAnimationTime += elapsed;
	screenShakeValue += elapsed;
}

void render(){
	glMatrixMode(GL_MODELVIEW);
	// if no score, neutral color
	if (scoreCount == 0){
		glClearColor(0.4f, 0.5f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// if there is a score, change color to one that last scored
	if (scoreCount){
		if (ballScorer == true){
			glClearColor(0.8f, 0.5f, 0.4f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (ballScorer == false){
			glClearColor(0.4f, 0.5f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	// if collision shake screen
	if (screenShake){
		glLoadIdentity();
		float animationValue;
		animationValue = mapValue(stageAnimationTime, 0.0, 0.5, 0.0, 1.0);
		float screenShakeIntensity = easeOut(0.0, 0.2, animationValue)*200.0f;
		glTranslatef(0.0f, sin(screenShakeValue * 500)* screenShakeIntensity * 200, 0.0f);
	}
	screenShake = false;
	stageAnimationTime = 0.0f;
	screenShakeValue = 0.0f;

	ball.Draw();
	fire.fireDraw();
	paddle1.Draw();
	paddle2.Draw();

	// if ball is scored
	if (ball.x > 1.33){
		++scoreCount;
		ball.x = 0.0;
		ball.y = 0.0;
		Mix_PlayChannel(-1, scoreSound, 0);
		ball.direction_x = 0.0;
		ball.direction_y = 0.0;
		ballScorer = true;
	}
	else if (ball.x < -1.33){
		++scoreCount;
		ball.x = 0.0;
		ball.y = 0.0;
		Mix_PlayChannel(-1, scoreSound, 0);
		ball.direction_x = 0.0;
		ball.direction_y = 0.0;
		ballScorer = false;
	}

	SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{
	Setup();

	float lastFrameTicks = 0.0f;

	ball.textureID = LoadTexture("ballGrey.png");
	ball.setSize(0.1f, 0.1f);
	ball.x = 0.0f;
	ball.y = 0.0f;
	ball.direction_x = 0.0f;
	ball.direction_y = 0.0f;

	fire.textureID = LoadTexture("fire.png");
	fire.setSize(0.2f, 0.2f);
	fire.x = 0.0f;
	fire.y = 0.0f;
	fire.direction_x = 0.0f;
	fire.direction_y = 0.0f;

	paddle1.textureID = LoadTexture("paddleRed.png");
	paddle1.setSize(0.1f, 0.35f);
	paddle1.x = -1.0f;
	paddle1.y = 0.0f;

	paddle2.textureID = LoadTexture("paddleBlu.png");
	paddle2.setSize(0.1f, 0.35f);
	paddle2.x = 1.0f;
	paddle2.y = 0.0f;


	while (ProcessEvents()){
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		update(elapsed);
		render();
	}
	
	Mix_FreeChunk(collideSound);
	Mix_FreeChunk(scoreSound);
	Mix_FreeMusic(music);
	SDL_Quit();
	return 0;
}