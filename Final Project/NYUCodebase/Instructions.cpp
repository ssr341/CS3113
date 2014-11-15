#include "Instructions.h"

void Instructions::Init(){
	screenText = LoadTexture("pixel_font.png");
}

GLuint Instructions::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID = -1;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);
	return textureID;
}

void Instructions::DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a) {
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

int Instructions::ProcessEvents(){
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	int state = 1;

	// if space is pressed move to first stage
	if (keys[SDL_SCANCODE_SPACE] == 1){
		state = 2;
	}
	// if q is pressed quit
	if (keys[SDL_SCANCODE_Q] == 1){
		SDL_Quit();
	}

	return state;
}

void Instructions::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(-1.3, 0.9, 0.0);
	DrawText(screenText, "Press Q at any time to quit", 0.03f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, 0.75, 0.0);
	DrawText(screenText, "Player 1:", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(0.4, 0.75, 0.0);
	DrawText(screenText, "Player 2:", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, 0.6, 0.0);
	DrawText(screenText, "E to move up", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(0.4, 0.6, 0.0);
	DrawText(screenText, "Up to move up", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, 0.5, 0.0);
	DrawText(screenText, "D to move down", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(0.4, 0.5, 0.0);
	DrawText(screenText, "Down to move down", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, 0.4, 0.0);
	DrawText(screenText, "F to shoot", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(0.4, 0.4, 0.0);
	DrawText(screenText, "0 to shoot", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, 0.2, 0.0);
	DrawText(screenText, "Goal is to beat your opponent in a best of 3.", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, 0.1, 0.0);
	DrawText(screenText, "One shot means death", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, 0.0, 0.0);
	DrawText(screenText, "Get powerups by shooting down UFOs: ", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.10, -0.1, 0.0);
	DrawText(screenText, "3 UFOs - Bullets travel faster", 0.035f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.10, -0.2, 0.0);
	DrawText(screenText, "6 UFOs - Bullets are larger", 0.035f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.10, -0.3, 0.0);
	DrawText(screenText, "10 UFOs - Move slightly faster and shoot more often", 0.035f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, -0.4, 0.0);
	DrawText(screenText, "If third stage reached, players start with all", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-1.25, -0.5, 0.0);
	DrawText(screenText, "powerups and the UFO cannot be killed", 0.05f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	glLoadIdentity();
	glTranslatef(-0.65f, -0.9f, 0.0f);
	DrawText(screenText, "Press Space to Continue", 0.05f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);

}
