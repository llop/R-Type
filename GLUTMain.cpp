
#include "Globals.h"
#include "cGame.h"


//Delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#define FPS 60


cGame Game;

// fps control
int lastElapsed;
int elapsedInterval;

void AppRender() {
	Game.Render();
}

void AppKeyboard(unsigned char key, int x, int y) {
	Game.ReadKeyboard(key,x,y,true);
}

void AppKeyboardUp(unsigned char key, int x, int y) {
	Game.ReadKeyboard(key,x,y,false);
}

void AppSpecialKeys(int key, int x, int y) {
	Game.ReadKeyboard(key,x,y,true);
}

void AppSpecialKeysUp(int key, int x, int y) {
	Game.ReadKeyboard(key,x,y,false);
}

void AppMouse(int button, int state, int x, int y) {
	Game.ReadMouse(button,state,x,y);
}

void AppIdle() {

	// skip if this is too fast
	int elapsed = glutGet(GLUT_ELAPSED_TIME);
	int interval = elapsed - lastElapsed;
	if (interval < elapsedInterval) return;
	lastElapsed = elapsed;

	if(!Game.Loop()) exit(0);
}

void main(int argc, char** argv) {
	int res_x,res_y,pos_x,pos_y;

	// vars
	lastElapsed = 0;
	elapsedInterval = 1000 / FPS;

	//GLUT initialization
	glutInit(&argc, argv);

	//RGBA with double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	//Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x>>1)-(GAME_WIDTH>>1);
	pos_y = (res_y>>1)-(GAME_HEIGHT>>1);
	
	glutInitWindowPosition(pos_x,pos_y);
	glutInitWindowSize(GAME_WIDTH,GAME_HEIGHT);
	glutCreateWindow("R-Type");

	/*glutGameModeString("800x600:32");
	glutEnterGameMode();*/

	//Make the default cursor disappear
	//check out available cursors https://www.opengl.org/documentation/specs/glut/spec3/node28.html
	//glutSetCursor(GLUT_CURSOR_NONE);

	//Register callback functions
	glutDisplayFunc(AppRender);			
	glutKeyboardFunc(AppKeyboard);		
	glutKeyboardUpFunc(AppKeyboardUp);	
	glutSpecialFunc(AppSpecialKeys);	
	glutSpecialUpFunc(AppSpecialKeysUp);
	glutMouseFunc(AppMouse);
	glutIdleFunc(AppIdle);

	//Game initializations
	Game.Init();

	//Application loop
	glutMainLoop();	
}
