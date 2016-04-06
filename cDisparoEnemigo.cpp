
#include "cDisparo.h"
#include "cNivel.h"
#include <gl/glut.h>


/*
// tiro viejo (TEX_NAVE1)
int tiroEnemigo[4][4] = {
	{ 212, 279, 6, 6 },
	{ 227, 278, 10, 8 },
	{ 243, 277, 12, 10 },
	{ 272, 276, 14, 12 }
};
int tiroEnemigoMid = 281;
*/

int tiroEnemigo[4][4] = {
	{ 136, 6, 7, 6 },
	{ 153, 6, 7, 6 },
	{ 170, 6, 7, 6 },
	{ 188, 6, 7, 6 }
};

int tiroEnemigoExplo[5][4] = {
	{ 118, 22, 9, 8 },
	{ 135, 22, 9, 8 },
	{ 151, 21, 11, 10 },
	{ 167, 20, 13, 12 },
	{ 183, 19, 15, 14 }
};

cDisparoEnemigo::cDisparoEnemigo(cSistema* sis, int x, int y, float xu, float yu) : cDisparo(sis, x, y) {
	_sis->cargaTextura(TEX_EXPLO1, "img\\r-typesheet43.png");

	_dano = DANO_DISPARO_ENEMIGO;
	_malo = true;
	
	_xF = float(_x);
	_yF = float(_y);

	_state = DISPARO_VIVE;
	_seq = 0;
	_delay = DISPARO_ENEMIGO_DELAY;

	_pixelsAvanzaX = xu;
	_pixelsAvanzaY = yu;


	_sis->playSonido(SOUND_DISPARO_ENEMIGO);
}

cDisparoEnemigo::~cDisparoEnemigo() {
}

void cDisparoEnemigo::explota() {
	muerete();
}

void cDisparoEnemigo::muerete() {
	_state = DISPARO_EXPLO;
	_seq = 0;
	_delay = DISPARO_EXPLO_DELAY;
}


void cDisparoEnemigo::colision(cRect &rect, int &colMask) const {
	colMask = 0;
	if (_state == DISPARO_VIVE) {
		cRect myRect;
		caja(myRect);
		if (myRect.x < rect.x+rect.w && myRect.x+myRect.w > rect.x &&
			myRect.y < rect.y+rect.h && myRect.y+myRect.h > rect.y) {
				colMask = 1;
		}
	}
}

void cDisparoEnemigo::caja(cRect &rect) const {
	rect.w = tiroEnemigo[_seq][2];
	rect.h = tiroEnemigo[_seq][3];
	rect.x = _x - (rect.w>>1);
	rect.y = _y - (rect.h>>1);
}


void cDisparoEnemigo::offset(int x, int y) {
	_xF = float(_xF + x);
	_yF = float(_yF + y);
}

void cDisparoEnemigo::logica() {
	if (_state == DISPARO_VIVE) {
		cNivel* nivel = ((cNivel*)_sis->nivel());
		float velo = float(_sis->dificultad() == DIFICULTAD_NORMAL ?
			VELOCIDAD_DISPARO_ENEMIGO : VELOCIDAD_DISPARO_ENEMIGO_HARD);
		velo *= nivel->factorDificultad();
		_xF += _pixelsAvanzaX*velo;
		_yF += _pixelsAvanzaY*velo;
	
		_x = int(_xF);
		_y = int(_yF);

		cRect rect;
		caja(rect);
		if (nivel->fueraLimites(rect)) {
			_muerto = true;
			return;
		}

		int colMask;
  		nivel->colision(rect, colMask);
		if (colMask) {
			explota();
			return;
		}

		// animacion
		if (_delay) --_delay;
		else {
			_seq = (_seq+1) % DISPARO_ENEMIGO_VIVE_FRAMES;
			_delay = DISPARO_ENEMIGO_DELAY;
		}
	} else if (_state == DISPARO_EXPLO) {
		if (_delay) --_delay;
		else {
			++_seq;
			if (_seq == DISPARO_ENEMIGO_EXPLO_FRAMES) {
				_state = DISPARO_MUERE;
 				_muerto = true;
			}
			_delay = DISPARO_ENEMIGO_DELAY;
		}
	}
}

void cDisparoEnemigo::pinta() const {
	if (_state == DISPARO_MUERE) return;

	int tex = _sis->idTextura(TEX_EXPLO1);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_EXPLO1, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
	int xPixTiro, yPixTiro, wPixTiro, hPixTiro;

	if (_state == DISPARO_VIVE) {
		xTexTiro = tiroEnemigo[_seq][0] / (float)wTex;
		yTexTiro = tiroEnemigo[_seq][1] / (float)hTex;
		wTexTiro = tiroEnemigo[_seq][2] / (float)wTex;
		hTexTiro = tiroEnemigo[_seq][3] / (float)hTex;
		wPixTiro = tiroEnemigo[_seq][2];
		hPixTiro = tiroEnemigo[_seq][3];
		xPixTiro = _x - (wPixTiro>>1);
		yPixTiro = GAME_HEIGHT - (_y + (hPixTiro>>1));
	} else if (_state == DISPARO_EXPLO) {
		xTexTiro = tiroEnemigoExplo[_seq][0] / (float)wTex;
		yTexTiro = tiroEnemigoExplo[_seq][1] / (float)hTex;
		wTexTiro = tiroEnemigoExplo[_seq][2] / (float)wTex;
		hTexTiro = tiroEnemigoExplo[_seq][3] / (float)hTex;
		wPixTiro = tiroEnemigoExplo[_seq][2];
		hPixTiro = tiroEnemigoExplo[_seq][3];
		xPixTiro = _x - (wPixTiro>>1);
		yPixTiro = GAME_HEIGHT - (_y + (hPixTiro>>1));
	}

	glTexCoord2f(xTexTiro, yTexTiro + hTexTiro);			glVertex2i(xPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro + hTexTiro);	glVertex2i(xPixTiro + wPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro);			glVertex2i(xPixTiro + wPixTiro, yPixTiro + hPixTiro);
	glTexCoord2f(xTexTiro, yTexTiro);						glVertex2i(xPixTiro, yPixTiro + hPixTiro);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}


