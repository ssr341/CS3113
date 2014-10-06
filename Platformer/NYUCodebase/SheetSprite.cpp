//#include "SheetSprite.h"
//
//SheetSprite::SheetSprite(){}
//
//SheetSprite::SheetSprite(unsigned int texture, int location, int spriteCountX, int spriteCountY): textureID(texture){
//	u = (float)(((int)location) % spriteCountX) / (float)spriteCountX;
//	v = (float)(((int)location) / spriteCountX) / (float)spriteCountY;
//
//	spriteHeight = 1.0f / (float)spriteCountX;
//	spriteWidth = 1.0f / (float)spriteCountY;
//}
//
//void SheetSprite::draw(){
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glMatrixMode(GL_MODELVIEW);
//
//	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
//	glVertexPointer(2, GL_FLOAT, 0, quad);
//	glEnableClientState(GL_VERTEX_ARRAY);
//
//	GLfloat uvs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
//	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glDisableClientState(GL_COLOR_ARRAY);
//
//	glDrawArrays(GL_QUADS, 0, 4);
//	glDisable(GL_TEXTURE_2D);
//}