
#include "cMenu.h"
#include "cNaveEspacial.h"
#include "cEscudo.h"
#include <gl/glut.h>
#include <vector>
using namespace std;



int title[4] = { 33, 107, 572, 117 };
int boldCaracteres[4] = { 155, 232, 8, 7 };
int caracteres[4] = { 155, 240, 8, 7 };
int continueMov[3][4] = {
	{ 238, 327, 72, 7 },
	{ 238, 342, 72, 7 },
	{ 238, 356, 72, 7 }
};
int continueOpts[2][4] = {
	{ 321, 333, 51, 7 },
	{ 321, 351, 51, 7 }
};
int dificultadOpts[2][4] = {
	{ 330, 287, 32, 7 },
	{ 330, 295, 32, 7 }
};
int theEnd[4] = { 238, 379, 111, 14 };
int gameOver[4] = { 239, 405, 144, 14 };


cMenu::cMenu(cSistema* sis) : cSprite(sis) {
	_sis->cargaTextura(TEX_MENU, "img\\menu.png");

	_state = PANTALLA_INICIO;
	_seleccionado = INICIO_START;
	_dificultad = DIFICULTAD_NORMAL;
	_continua = GAME_OVER_CONTINUE;

	_tiempoVida = 0;
	_ultimoInput = -MENU_INTERVALO_INPUT;
}


void cMenu::setDificultad(int dificultad) {
	_dificultad = dificultad;
}

// pre: pantalla es (PANTALLA_INICIO, PANTALLA_INSTRUCCIONES, PANTALLA_CREDITOS, o PANTALLA_GAME_OVER)
void cMenu::setPantalla(int pantalla) {
	_state = pantalla;

	if (_state == PANTALLA_SCORE) _tiempoScore = 0;
}

void cMenu::setScore(int nivel, long long puntos) {
	_nivel = nivel;
	_puntos = puntos;
}


int cMenu::dificultad() const {
	return _dificultad;
}

void cMenu::procesaTeclas(unsigned char* keys) {

	// limpiar mierda
	if (_state == PANTALLA_SCORE && !_tiempoScore) {
		unsigned char keysVoid[256];
		for (unsigned int i=0; i<256; ++i) keysVoid[i]=0;
		((cNaveEspacial*)_sis->naveEspacial())->procesaTeclas(keysVoid);
	}

	long long intervalo = _tiempoVida - _ultimoInput;
	if (intervalo < MENU_INTERVALO_INPUT) return;

	if (_state == PANTALLA_INICIO) {
		if (keys[MENU_ENTER_KEY]||keys[' ']) {
			if (_seleccionado == INICIO_START) {
				_sis->arrancaPartida();
			} else if (_seleccionado == INICIO_DIFICULTAD) {
				_dificultad = (_dificultad+1)%NUM_DIFICULTADES;
			} else if (_seleccionado == INICIO_INSTRUCCIONES) {
				_state = PANTALLA_INSTRUCCIONES;
			} else if (_seleccionado == INICIO_CREDITOS) {
				_state = PANTALLA_CREDITOS;
			}
			_ultimoInput = _tiempoVida;
		} else {
			if (keys[GLUT_KEY_UP] || keys[GLUT_KEY_DOWN]) {
				if (keys[GLUT_KEY_UP]) _seleccionado = (_seleccionado-1+INICIO_NUM_OPCIONES)%INICIO_NUM_OPCIONES;
				if (keys[GLUT_KEY_DOWN]) _seleccionado = (_seleccionado+1)%INICIO_NUM_OPCIONES;
				_ultimoInput = _tiempoVida;
			} else {
				if (_seleccionado == INICIO_DIFICULTAD) {
					if (keys[GLUT_KEY_LEFT] || keys[GLUT_KEY_RIGHT]) {
						if (keys[GLUT_KEY_LEFT]) _dificultad = (_dificultad-1+NUM_DIFICULTADES)%NUM_DIFICULTADES;
						if (keys[GLUT_KEY_RIGHT]) _dificultad = (_dificultad+1)%NUM_DIFICULTADES;
						_ultimoInput = _tiempoVida;
					}
				}
			}
		}
	} else if (_state == PANTALLA_INSTRUCCIONES) {
		if (keys[MENU_ENTER_KEY]||keys[' ']) {
			_state = PANTALLA_INICIO;
			_ultimoInput = _tiempoVida;
		}
	} else if (_state == PANTALLA_CREDITOS) {
		if (keys[MENU_ENTER_KEY]||keys[' ']) {
			_state = PANTALLA_INICIO;
			_ultimoInput = _tiempoVida;
		}
	} else if (_state == PANTALLA_GAME_OVER) {
		if (keys[MENU_ENTER_KEY]||keys[' ']) {
			if (_continua == GAME_OVER_CONTINUE) {
				_sis->continuePartida();
			} else if (_continua == GAME_OVER_QUIT) {
				_state = PANTALLA_INICIO;
			}
			_ultimoInput = _tiempoVida;
		} else {
			if (keys[GLUT_KEY_LEFT] || keys[GLUT_KEY_RIGHT]) {
				if (keys[GLUT_KEY_LEFT]) _continua = (_continua-1+GAME_OVER_NUM_OPCIONES)%GAME_OVER_NUM_OPCIONES;
				if (keys[GLUT_KEY_RIGHT]) _continua = (_continua+1)%GAME_OVER_NUM_OPCIONES;
				_ultimoInput = _tiempoVida;
			}
		}
	}
}

void cMenu::logica() {
	++_tiempoVida;

	if (_state == PANTALLA_INICIO) {

	} else if (_state == PANTALLA_INSTRUCCIONES) {

	} else if (_state == PANTALLA_CREDITOS) {

	} else if (_state == PANTALLA_GAME_OVER) {

	} else if (_state == PANTALLA_SCORE) {
		++_tiempoScore;
		if (_tiempoScore == MENU_TIEMPO_SCORE) {
			_sis->cargaNivel();
			return;
		}

		// llevar la nave al centro
		int xMid = GAME_WIDTH>>1;
		int yMid = GAME_HEIGHT>>1;
		int xNave, yNave;
		_sis->naveEspacial()->getPosicion(xNave, yNave);
		if (xMid!=xNave || yMid!=yNave) {
			float xVec = float(xMid-xNave);
			float yVec = float(yMid-yNave);
			float dist = sqrt(xVec*xVec + yVec*yVec);
			xVec /= dist;
			yVec /= dist;
			xVec *= MENU_AVANCE_NAVE;
			yVec *= MENU_AVANCE_NAVE;
			_sis->naveEspacial()->offset(int(xVec), int(yVec));
		}
		vector<cEscudo*> &escudos = ((cNaveEspacial*)_sis->naveEspacial())->escudos();
		for (unsigned int i=0; i<escudos.size(); ++i) escudos[i]->logica();
	}
}

inline int code(char c) {
	if (c>='a' && c<='z') return c-'a';
	if (c>='A' && c<='Z') return c-'A';
	if (c>='0' && c<='9') return 34+c-'0';
	if (c==' ') return 44;
	if (c=='!') return 26;
	if (c=='?') return 27;
	if (c==';') return 28;
	if (c==',') return 29;
	if (c=='_') return 30;
	if (c=='.') return 31;
	if (c=='-') return 32;
	if (c=='/') return 33;
	return 44;
}

void cMenu::pintaNumero(int x, int y, int* alfabeto, long long num) const {
	string str;
	if (num == 0) str = "0";
	else {
		vector<char> numVec;
		while (num) {
			numVec.push_back('0'+(num%10));
			num /= 10;
		}
		for (int i=numVec.size()-1; i>=0; --i) str += numVec[i];
	}
	pintaString(x, y, alfabeto, str);
}

void cMenu::pintaString(int x, int y, int* alfabeto, const string &str) const {

	int wTex, hTex;
	_sis->tamanoTextura(TEX_MENU, wTex, hTex);

	int wPixChar = alfabeto[2];
	int hPixChar = alfabeto[3];

	int xPixChar = x;
	int yPixChar = GAME_HEIGHT - (y + hPixChar);

	float wTexChar = wPixChar/(float)wTex;
	float hTexChar = hPixChar/(float)hTex;

	float xTexChar;
	float yTexChar = alfabeto[1]/(float)hTex;
	
	for (unsigned int i=0; i<str.size(); ++i) {
		int offset = code(str[i])*(wPixChar);
		xTexChar = (alfabeto[0]+offset)/(float)wTex;

		glTexCoord2f(xTexChar, yTexChar + hTexChar);			glVertex2i(xPixChar, yPixChar);
		glTexCoord2f(xTexChar + wTexChar, yTexChar + hTexChar);	glVertex2i(xPixChar + wPixChar, yPixChar);
		glTexCoord2f(xTexChar + wTexChar, yTexChar);			glVertex2i(xPixChar + wPixChar, yPixChar + hPixChar);
		glTexCoord2f(xTexChar, yTexChar);						glVertex2i(xPixChar, yPixChar + hPixChar);

		xPixChar += wPixChar+1;
	}

}

void cMenu::pinta() const {
	int tex = _sis->idTextura(TEX_MENU);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_MENU, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	if (_state == PANTALLA_INICIO) {

		// pinta titulo
		int yTit = 120;
		int wTex, hTex;
		_sis->tamanoTextura(TEX_MENU, wTex, hTex);
		int wPixTit = title[2];
		int hPixTit = title[3];
		int xPixTit = (GAME_WIDTH>>1)-(wPixTit>>1);
		int yPixTit = GAME_HEIGHT - (yTit + hPixTit);
		float wTexTit = wPixTit/(float)wTex;
		float hTexTit = hPixTit/(float)hTex;
		float xTexTit = title[0]/(float)wTex;
		float yTexTit = title[1]/(float)hTex;
		glTexCoord2f(xTexTit, yTexTit + hTexTit);			glVertex2i(xPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit + hTexTit);	glVertex2i(xPixTit + wPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit);			glVertex2i(xPixTit + wPixTit, yPixTit + hPixTit);
		glTexCoord2f(xTexTit, yTexTit);						glVertex2i(xPixTit, yPixTit + hPixTit);

		// cabecera
		int xCap = 420;
		int yCap = 80;
		int incCap = 8;
		pintaString(xCap, yCap, boldCaracteres, "blast off and strike");
		pintaString(xCap, yCap+=incCap, boldCaracteres, "the evil bydo empire!");

		// opciones
		int xOpts = 240;
		int yOpts = 320;
		int incOpts = 12;
		pintaString(xOpts, yOpts, _seleccionado==INICIO_START ? boldCaracteres : caracteres, "start game");
		pintaString(xOpts, yOpts+=incOpts, _seleccionado==INICIO_DIFICULTAD ? boldCaracteres : caracteres, "difficulty");
		
		// dificultad seleccionada
		int wPixDif = dificultadOpts[_dificultad][2];
		int hPixDif = dificultadOpts[_dificultad][3];
		int xPixDif = xOpts+104;
		int yPixDif = GAME_HEIGHT - (yOpts + hPixDif);
		float wTexDif = wPixDif/(float)wTex;
		float hTexDif = hPixDif/(float)hTex;
		float xTexDif = dificultadOpts[_dificultad][0]/(float)wTex;
		float yTexDif = dificultadOpts[_dificultad][1]/(float)hTex;
		glTexCoord2f(xTexDif, yTexDif + hTexDif);			glVertex2i(xPixDif, yPixDif);
		glTexCoord2f(xTexDif + wTexDif, yTexDif + hTexDif);	glVertex2i(xPixDif + wPixDif, yPixDif);
		glTexCoord2f(xTexDif + wTexDif, yTexDif);			glVertex2i(xPixDif + wPixDif, yPixDif + hPixDif);
		glTexCoord2f(xTexDif, yTexDif);						glVertex2i(xPixDif, yPixDif + hPixDif);

		pintaString(xOpts, yOpts+=incOpts, _seleccionado==INICIO_INSTRUCCIONES ? boldCaracteres : caracteres, "controls");
		pintaString(xOpts, yOpts+=incOpts, _seleccionado==INICIO_CREDITOS ? boldCaracteres : caracteres, "credits");

	} else if (_state == PANTALLA_INSTRUCCIONES) {
		
		int xControls = 100;
		int xControlsOffset = 120;
		int yControls = 100;
		int incControls = 10;
		
		pintaString(xControls, yControls, boldCaracteres, "move ship");
		pintaString(xControls+xControlsOffset, yControls, caracteres, "wsad");
		
		pintaString(xControls, yControls+=incControls, boldCaracteres, "shoot");
		pintaString(xControls+xControlsOffset, yControls, caracteres, "space bar");

		// volver
		int xBack = 100;
		int yBack = 400;
		pintaString(xBack, yBack, boldCaracteres, "back");

	} else if (_state == PANTALLA_CREDITOS) {

		int xCredits = 100;
		int xCreditsOffset = 120;
		int yCredits = 100;
		int incCredits = 10;

		pintaString(xCredits, yCredits, boldCaracteres, "player 1");
		pintaString(xCredits+xCreditsOffset, yCredits, caracteres, "albert lobo");
		
		pintaString(xCredits, yCredits+=incCredits, boldCaracteres, "player 2");
		pintaString(xCredits+xCreditsOffset, yCredits, caracteres, "oriol momino");
		
		// volver
		int xBack = 100;
		int yBack = 400;
		pintaString(xBack, yBack, boldCaracteres, "back");

	} else if (_state == PANTALLA_GAME_OVER) {

		// pinta titulo
		int yTit = 160;
		int wTex, hTex;
		_sis->tamanoTextura(TEX_MENU, wTex, hTex);
		int wPixTit = gameOver[2];
		int hPixTit = gameOver[3];
		int xPixTit = (GAME_WIDTH>>1)-(wPixTit>>1);
		int yPixTit = GAME_HEIGHT - (yTit + hPixTit);
		float wTexTit = wPixTit/(float)wTex;
		float hTexTit = hPixTit/(float)hTex;
		float xTexTit = gameOver[0]/(float)wTex;
		float yTexTit = gameOver[1]/(float)hTex;
		glTexCoord2f(xTexTit, yTexTit + hTexTit);			glVertex2i(xPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit + hTexTit);	glVertex2i(xPixTit + wPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit);			glVertex2i(xPixTit + wPixTit, yPixTit + hPixTit);
		glTexCoord2f(xTexTit, yTexTit);						glVertex2i(xPixTit, yPixTit + hPixTit);

		// continue?
		yTit = 270;
		int contSeq = (_tiempoVida/16)%3;
		wPixTit = continueMov[contSeq][2];
		hPixTit = continueMov[contSeq][3];
		xPixTit = (GAME_WIDTH>>1)-(wPixTit>>1);
		yPixTit = GAME_HEIGHT - (yTit + hPixTit);
		wTexTit = wPixTit/(float)wTex;
		hTexTit = hPixTit/(float)hTex;
		xTexTit = continueMov[contSeq][0]/(float)wTex;
		yTexTit = continueMov[contSeq][1]/(float)hTex;
		glTexCoord2f(xTexTit, yTexTit + hTexTit);			glVertex2i(xPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit + hTexTit);	glVertex2i(xPixTit + wPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit);			glVertex2i(xPixTit + wPixTit, yPixTit + hPixTit);
		glTexCoord2f(xTexTit, yTexTit);						glVertex2i(xPixTit, yPixTit + hPixTit);
		
		// opcion seleccionada
		yTit = 284;
		wPixTit = continueOpts[_continua][2];
		hPixTit = continueOpts[_continua][3];
		xPixTit = (GAME_WIDTH>>1)-(wPixTit>>1);
		yPixTit = GAME_HEIGHT - (yTit + hPixTit);
		wTexTit = wPixTit/(float)wTex;
		hTexTit = hPixTit/(float)hTex;
		xTexTit = continueOpts[_continua][0]/(float)wTex;
		yTexTit = continueOpts[_continua][1]/(float)hTex;
		glTexCoord2f(xTexTit, yTexTit + hTexTit);			glVertex2i(xPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit + hTexTit);	glVertex2i(xPixTit + wPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit);			glVertex2i(xPixTit + wPixTit, yPixTit + hPixTit);
		glTexCoord2f(xTexTit, yTexTit);						glVertex2i(xPixTit, yPixTit + hPixTit);

	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (_state == PANTALLA_SCORE) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);
		// stage clear
		int xStage = 260;
		int xStageOffset1 = 60;
		int xStageOffset2 = 80;
		int yStage = 160;
		int incStage = 24;
		
		pintaString(xStage, yStage, boldCaracteres, "stage");
		pintaNumero(xStage+xStageOffset1, yStage, boldCaracteres, _nivel);
		pintaString(xStage+xStageOffset2, yStage, boldCaracteres, "clear!");
		
		// pinta puntuacion
		xStage+=8;
		pintaString(xStage, yStage+=incStage, boldCaracteres, "score");
		pintaNumero(xStage+xStageOffset1, yStage, boldCaracteres, _puntos*100);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		// pinta nave y escudos
		_sis->naveEspacial()->pinta();
		const vector<cEscudo*> &escudos = ((cNaveEspacial*)_sis->naveEspacial())->escudos();
		for (unsigned int i=0; i<escudos.size(); ++i) escudos[i]->pinta();
	}

}