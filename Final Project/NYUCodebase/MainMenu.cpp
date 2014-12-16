#include "MainMenu.h"

void MainMenu::Init(){
	screenText = LoadTexture("pixel_font.png");
	time = 0.0f;
	wordX = 1.3f;
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

	glDisableClientState(GL_COLOR_ARRAY);
}

int MainMenu::ProcessEvents(SDL_Event* event, bool& done){
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	int state = 0;

	if (event->type == SDL_KEYDOWN) {
		// if space is pressed move to stage 1
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE){
			state = 2;
			time = 11.0f; // set time > 10 so doesn't play animations when return to menu
		}
		// if i is pressed move to instructions
		if (event->key.keysym.scancode == SDL_SCANCODE_I){
			state = 1;
			time = 11.0f; // set time > 10 so doesn't play animations when return to menu
		}
		// if escape is pressed quit
		if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			done = true;
	}

	return state;
}

void MainMenu::Render(float fixedElapsed){
	// start screen text
	if (time < 10.0f) // only keep track until 10, no need to otherwise
		time += fixedElapsed;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glTranslatef(-1.3f, 0.9f, 0.0f);
	DrawText(screenText, "Press Esc at any time to quit", 0.03f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

	if (time < 10.0f){
		glLoadIdentity();
		glTranslatef(wordX, 0.25f, 0.0f);
		DrawText(screenText, "Galaxy", 0.15f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(wordX, 0.0f, 0.0f);
		DrawText(screenText, "Defender", 0.15f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

		wordX -= fixedElapsed/2.5f;
	}

	if (time > 10.0f){
		glLoadIdentity();
		glTranslatef(-0.35f, 0.25f, 0.0f);
		DrawText(screenText, "Galaxy", 0.15f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-0.5f, 0.0f, 0.0f);
		DrawText(screenText, "Defender", 0.15f, 0.001f, 1.0f, 1.0f, 0.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-0.8f, -0.4f, 0.0f);
		DrawText(screenText, "Press Space to Continue", 0.07f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);

		glLoadIdentity();
		glTranslatef(-0.8f, -0.6f, 0.0f);
		DrawText(screenText, "Press I for Instructions", 0.07f, 0.001f, 1.0f, 0.0f, 0.0f, 1.0f);
	}
}
