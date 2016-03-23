
#include "cDisparo.h"
#include "cNivel.h"
#include <gl/glut.h>


#include "cDisparo.h"


int tiroEnemigo[4][4] = {
	{ 212, 279, 6, 6 },
	{ 227, 278, 10, 8 },
	{ 243, 277, 12, 10 },
	{ 272, 276, 14, 12 }
};
int tiroEnemigoMid = 281;


cDisparoEnemigo::cDisparoEnemigo(cSistema* sis, int x, int y, float xu, float yu) : cDisparo(sis, x, y) {
	_sis->cargaTextura(TEX_NAVE1, "img\\r-typesheet1.png");

	_dano = DANO_DISPARO_ENEMIGO;
	
	_xF = float(_x);
	_yF = float(_y);

	_state = DISPARO_VIVE;
	_seq = 0;
	_delay = DISPARO_ENEMIGO_DELAY;

	_pixelsAvanzaX = xu;
	_pixelsAvanzaY = yu;
}

cDisparoEnemigo::~cDisparoEnemigo() {
}

void cDisparoEnemigo::muerete() {
	_state = DISPARO_EXPLO;
	_seq = 3;
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
	rect.x = _x;
	rect.y = _y;
}

void cDisparoEnemigo::logica() {
	if (_state == DISPARO_VIVE) {
		cNivel* nivel = ((cNivel*)_sis->nivel());
		_xF += _pixelsAvanzaX*VELOCIDAD_DISPARO_ENEMIGO;
		_yF += _pixelsAvanzaY*VELOCIDAD_DISPARO_ENEMIGO;
	
		_x = int(_xF);
		_y = int(_yF);

		cRect rect;
		caja(rect);
		if (nivel->fueraLimites(rect)) {
			_muerto = true;
			return;
		}
	} else if (_state == DISPARO_EXPLO) {
		if (_delay) --_delay;
		else {
 			_muerto = true;
		}
	}
}

void cDisparoEnemigo::pinta() const {
	if (_state == DISPARO_MUERE) return;

	int tex = _sis->idTextura(TEX_NAVE1);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_NAVE1, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
	int xPixTiro, yPixTiro, yPixOffsetTiro, wPixTiro, hPixTiro;

	//if (_state == DISPARO_VIVE) {
		xTexTiro = tiroEnemigo[_seq][0] / (float)wTex;
		yTexTiro = tiroEnemigo[_seq][1] / (float)hTex;
		wTexTiro = tiroEnemigo[_seq][2] / (float)wTex;
		hTexTiro = tiroEnemigo[_seq][3] / (float)hTex;
		wPixTiro = tiroEnemigo[_seq][2];
		hPixTiro = tiroEnemigo[_seq][3];
		xPixTiro = _x;
		yPixOffsetTiro = tiroEnemigoMid - tiroEnemigo[_seq][1];
		yPixTiro = GAME_HEIGHT - (_y - yPixOffsetTiro + hPixTiro);
	//} else if (_state == DISPARO_EXPLO) {

	//}

	glTexCoord2f(xTexTiro, yTexTiro + hTexTiro);			glVertex2i(xPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro + hTexTiro);	glVertex2i(xPixTiro + wPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro);			glVertex2i(xPixTiro + wPixTiro, yPixTiro + hPixTiro);
	glTexCoord2f(xTexTiro, yTexTiro);						glVertex2i(xPixTiro, yPixTiro + hPixTiro);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}


