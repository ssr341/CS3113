#include "GameOver.h"

GameOver::GameOver(){}

void GameOver::Init(){
	screenText = LoadTexture("font1.png");
}

GLuint GameOver::LoadTexture(const char *image_path) {
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

void GameOver::DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a) {
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

bool GameOver::ProcessEvents(){
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	// if space is pressed move to gameplay
	if (keys[SDL_SCANCODE_SPACE] == 1){
		done = true;
	}

	return done;
}

void GameOver::Render(bool death){
	if (death){
		// game over text
		glLoadIdentity();
		glTranslatef(-0.25, 0.25, 0.0);
		DrawText(screenText, "Game", 0.2f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.25, 0.0, 0.0);
		DrawText(screenText, "Over", 0.2f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.45f, -0.25f, 0.0f);
	}

	else{
		// congrats text
		glLoadIdentity();
		glTranslatef(-0.25, 0.25, 0.0);
		DrawText(screenText, "You're", 0.2f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.25, 0.0, 0.0);
		DrawText(screenText, "Winner!", 0.2f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.45f, -0.25f, 0.0f);
	}

	glLoadIdentity();
	glTranslatef(-0.75f, -0.6f, 0.0f);
	DrawText(screenText, "Press Space to Continue", 0.07f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);

}
