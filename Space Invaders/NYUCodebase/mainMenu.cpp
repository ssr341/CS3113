#include "MainMenu.h"

MainMenu::MainMenu(){ done = false; }

void MainMenu::Init(){
	screenText = LoadTexture("font1.png");
}

GLuint MainMenu::LoadTexture(const char *image_path) {
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

void MainMenu::DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a) {
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

bool MainMenu::ProcessEvents(){
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	// if space is pressed move to gameplay
	if (keys[SDL_SCANCODE_SPACE] == 1){
		done = true;
	}
	return done;
}

void MainMenu::Render(){
	// start screen text
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.25, 0.25, 0.0);
	DrawText(screenText, "Space", 0.15f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);
	
	glLoadIdentity();
	glTranslatef(-0.5, 0.0, 0.0);
	DrawText(screenText, "Invaders", 0.15f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);
	glLoadIdentity();
	glTranslatef(-0.75f, -0.4f, 0.0f);
	DrawText(screenText, "Press Space to Continue", 0.07f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);
	
	//SDL_GL_SwapWindow(displayWindow);
}
