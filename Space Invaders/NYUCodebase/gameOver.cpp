#include "gameOver.h"

GLuint mainMenu::LoadTexture(const char *image_path) {
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

void mainMenu::DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a) {
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

void gameOver::Render(int score){
	// game over text
	glLoadIdentity();
	glTranslatef(-0.25, 0.25, 0.0);
	DrawText(screenText, "Game", 0.2, 0.001, 1.0, 0.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.25, 0.0, 0.0);
	DrawText(screenText, "Over", 0.2, 0.001, 1.0, 0.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.45, -0.25, 0.0);

	std::string scoreString = std::to_string(score);
	DrawText(screenText, "Final Score:", 0.07, 0.001, 1.0, 0.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(0.5, -0.25, 0.0);
	DrawText(screenText, scoreString, 0.07, 0.001, 1.0, 0.0, 0.0, 1.0);

	SDL_GL_SwapWindow(displayWindow);
}

int gameOver::Update(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed += ticks;
	if (ticks > 1000)
		return 0;
}