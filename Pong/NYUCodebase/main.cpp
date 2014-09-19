
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>

using namespace std;

SDL_Window* displayWindow;
bool ballScorer = true;  // person who scored last ball. True 1, False 2

class Entity{
public:
	void Draw(){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glScalef(width, height, 0.0);
		glRotatef(rotation, 0.0, 0.0, 1.0);
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

	void setSize(float x, float y){
		width = x;
		height = y;
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
Entity paddle1;
Entity paddle2;

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
	if (keys[SDL_SCANCODE_UP] == 1){
		// if up pressed, set direction positive
		paddle1.direction_y = 1.0;
	}
	else if (keys[SDL_SCANCODE_DOWN]){
		// if down pressed, set direction negative
		paddle1.direction_y = -1.0;
	}
	// mouse clicks used for player 2
	else if (event.type == SDL_MOUSEBUTTONDOWN){
		// left click goes up
		if (event.button.button == 1)
			paddle2.direction_y = 1.0;
		// right click goes down
		if (event.button.button == 3)
			paddle2.direction_y = -1.0;
	}

	// ball movement and collision
	if (ball.x == 0 && ball.y == 0 && ball.direction_x == 0 && ball.direction_y == 0){  // ball is neutral
		if (ballScorer)  // go to right if player 1 last scored
			ball.direction_x = 1.0;
		else             // go to left if player 2 last scored
			ball.direction_x = -1.0;
	}

	float ballRight = ball.width / 2;
	float ballLeft = ball.width / -2;
	float ballTop = ball.height / 2;
	float ballBottom = ball.height / -2;

	float paddle1Right = paddle1.width / 2;
	float paddle1Left = paddle1.width / -2;
	float paddle1Top = paddle1.height / 2;
	float paddle1Bottom = paddle1.height / -2;

	float paddle2Right = paddle2.width / 2;
	float paddle2Left = paddle2.width / -2;
	float paddle2Top = paddle2.height / 2;
	float paddle2Bottom = paddle2.height / -2;

	// check for collision with paddle1
	if ((ballLeft <= paddle1Right) && (ballTop >= paddle1Bottom) && (ballBottom <= paddle1Top)){
		ball.direction_x *= -1;  // reverse x direction
		ball.direction_y = paddle1.direction_y * paddle1.speed; // set y direction
	}

	// check for collision with paddle2
	if ((ballRight >= paddle2Left) && (ballBottom <= paddle2Top) && (ballTop >= paddle2Bottom)){
		ball.direction_x *= -1;  // reverse x direction
		ball.direction_y = paddle2.direction_y * paddle2.speed; // set y direction
	}

	// check collision with ceilings
	if ((ballTop >= 1.0) || (ballBottom <= -1.0))
		ball.direction_y *= -1.0;

	return true;
}

void update(float elapsed){
	// paddle 1
	paddle1.y += paddle1.direction_y * elapsed;
	paddle1.speed = fabs(paddle1.direction_y * elapsed) + 0.1;
	// screen boundaries for paddle 1
	if (paddle1.y > 0.825)
		paddle1.y = 0.825;
	if (paddle1.y < -0.825)
		paddle1.y = -0.825;
	paddle1.direction_y = 0;  // stops further movement

	// paddle 2
	paddle2.y += paddle2.direction_y * elapsed;
	paddle2.speed = fabs(paddle2.direction_y * elapsed) + 0.1;
	// screen boundaries for paddle 2
	if (paddle2.y > 0.825)
		paddle2.y = 0.825; 
	if (paddle2.y < -0.825)
		paddle2.y = -0.825;
	paddle2.direction_y = 0;  // stops further movement

	// ball
	ball.x += ball.direction_x * elapsed;
	ball.y += ball.direction_y * elapsed;
}

void render(){
	glClearColor(0.4f, 0.5f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ball.Draw();
	paddle1.Draw();
	paddle2.Draw();

	// if ball is scored
	if (ball.x > 1.33){
		glClearColor(0.4f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ball.x = 0.0;
		ball.y = 0.0;
		ballScorer = true;
	}
	else if (ball.x < -1.33){
		glClearColor(0.8f, 0.5f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ball.x = 0.0;
		ball.y = 0.0;
		ballScorer = false;
	}

	SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{
	Setup();

	float lastFrameTicks = 0.0;

	ball.textureID = LoadTexture("ballGrey.png");
	ball.setSize(0.1, 0.1);

	paddle1.textureID = LoadTexture("paddleRed.png");
	paddle1.setSize(0.1, 0.35);
	paddle1.x = -1.0;

	paddle2.textureID = LoadTexture("paddleBlu.png");
	paddle2.setSize(0.1, 0.35);
	paddle2.x = 1.0;


	while (ProcessEvents()){
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		update(elapsed);
		render();
	}
	
	SDL_Quit();
	return 0;
}