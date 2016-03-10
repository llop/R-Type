#include "cGame.h"
#include "Globals.h"


cGame::cGame(void) {
	sis = new cSistema();
}

cGame::~cGame(void) {
	delete sis;
}

bool cGame::Init() {
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,GAME_WIDTH,0,GAME_HEIGHT,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	//res = Data.LoadImage(IMG_BLOCKS,"blocks.png",GL_RGBA);
	//if(!res) return false;
	//res = Scene.LoadLevel(1);
	//if(!res) return false;

	//Player initialization
	//res = Data.LoadImage(IMG_PLAYER,"bub.png",GL_RGBA);
	//if(!res) return false;
	//Player.SetWidthHeight(32,32);
	//Player.SetTile(4,1);
	//Player.SetWidthHeight(32,32);
	//Player.SetState(STATE_LOOKRIGHT);

	return res;
}

bool cGame::Loop() {
	bool res = Process();
	if (res) Render();
	return res;
}

void cGame::Finalize() {
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press) {
	keys[key] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y) {
}

//Process
bool cGame::Process() {
	bool res = true;
	
	// termina juego
	if(keys[27]) res = false;

	sis->procesaTeclas(keys);
	sis->logica();

	return res;
}

//Output
void cGame::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	sis->pinta();
	glutSwapBuffers();
}
