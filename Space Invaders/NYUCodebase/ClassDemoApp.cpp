//#include "ClassDemoApp.h"
//
//ClassDemoApp::ClassDemoApp(){
//	Init();
//	done = false;
//	lastFrameTicks = 0.0f;
//}
//
//void ClassDemoApp::Init(){}
//
//void ClassDemoApp::shootBullet(){
//	//Bullet newBullet;
//	//
//	//newBullet.visible = true;
//	//newBullet.x = -1.2f;
//	//newBullet.y = 0.0f;
//	//newBullet.angle = rand();
//	//newBullet.speed = 2.0f;
//
//	//bullets.push_back(newBullet);
//
//	bullets[bulletIndex].visible = true;
//	bullets[bulletIndex].x = -1.2f;
//	bullets[bulletIndex].y = 0.0f;
//	bullets[bulletIndex].angle = rand();
//	bullets[bulletIndex].size = 0.02f;
//	bullets[bulletIndex].speed = 2.0f;
//
//	bulletIndex++;
//	if (bulletIndex > MAX_BULLETS - 1)
//		bulletIndex = 0;
//}
//
//ClassDemoApp::~ClassDemoApp(){
//	SDL_Quit;
//}
//
//void ClassDemoApp::Render(){
//	//render stuff
//
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	for (int i = 0; i < bullets.size(); i++){
//		bullets[i].Draw();
//	}
//
//	SDL_GL_SwapWindow(displayWindow);
//}
//
//void ClassDemoApp::Update(float elapsed){
//	SDL_Event event;
//
//	while (SDL_PollEvent(Event)){}
//}