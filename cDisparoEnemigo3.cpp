
#include "cDisparo.h"
#include "cNivel.h"
#include <gl/glut.h>



int tiroEnemigo3[4] = { 166, 202, 12, 12 };

int tiroEnemigo3Explo[5][4] = {
	{ 118, 22, 9, 8 },
	{ 135, 22, 9, 8 },
	{ 151, 21, 11, 10 },
	{ 167, 20, 13, 12 },
	{ 183, 19, 15, 14 }
};


cDisparoEnemigo3::cDisparoEnemigo3(cSistema* sis, int x, int y, float xu, float yu) : cDisparo(sis, x, y) {
	_sis->cargaTextura(TEX_ENE3, "img\\r-typesheet31.png");
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
}

cDisparoEnemigo3::~cDisparoEnemigo3() {
}

void cDisparoEnemigo3::explota() {
	muerete();
}

void cDisparoEnemigo3::muerete() {
	_state = DISPARO_EXPLO;
	_seq = 0;
	_delay = DISPARO_ENEMIGO3_EXPLO_DELAY;
}


void cDisparoEnemigo3::colision(cRect &rect, int &colMask) const {
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

void cDisparoEnemigo3::caja(cRect &rect) const {
	rect.w = tiroEnemigo3[2];
	rect.h = tiroEnemigo3[3];
	rect.x = _x - (rect.w>>1);
	rect.y = _y - (rect.h>>1);
}

void cDisparoEnemigo3::logica() {
	if (_state == DISPARO_VIVE) {
		cNivel* nivel = ((cNivel*)_sis->nivel());
		_xF += _pixelsAvanzaX*VELOCIDAD_DISPARO_ENEMIGO;
		_yF += _pixelsAvanzaY*VELOCIDAD_DISPARO_ENEMIGO;
	
		_x = int(_xF);
		_y = int(_yF);

		cRect rect;
		caja(rect);
		if (nivel->fueraLimites(rect)) {
			_state = DISPARO_MUERE;
			_muerto = true;
			return;
		}

		int colMask;
  		nivel->colision(rect, colMask);
		if (colMask) {
			explota();
			return;
		}

	} else if (_state == DISPARO_EXPLO) {
		if (_delay) --_delay;
		else {
			++_seq;
			if (_seq == DISPARO_ENEMIGO3_EXPLO_FRAMES) {
				_state = DISPARO_MUERE;
				_muerto = true;
			}
			_delay = DISPARO_ENEMIGO3_EXPLO_DELAY;
		}
	}
}

void cDisparoEnemigo3::pinta() const {
	if (_state == DISPARO_MUERE) return;

	if (_state == DISPARO_VIVE) {
		int tex = _sis->idTextura(TEX_ENE3);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_ENE3, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
		int xPixTiro, yPixTiro, wPixTiro, hPixTiro;
		xTexTiro = tiroEnemigo3[0] / (float)wTex;
		yTexTiro = tiroEnemigo3[1] / (float)hTex;
		wTexTiro = tiroEnemigo3[2] / (float)wTex;
		hTexTiro = tiroEnemigo3[3] / (float)hTex;
		wPixTiro = tiroEnemigo3[2];
		hPixTiro = tiroEnemigo3[3];
		xPixTiro = _x - (wPixTiro>>1);
		yPixTiro = GAME_HEIGHT - (_y + (hPixTiro>>1));

		glTexCoord2f(xTexTiro, yTexTiro + hTexTiro);			glVertex2i(xPixTiro, yPixTiro);
		glTexCoord2f(xTexTiro + wTexTiro, yTexTiro + hTexTiro);	glVertex2i(xPixTiro + wPixTiro, yPixTiro);
		glTexCoord2f(xTexTiro + wTexTiro, yTexTiro);			glVertex2i(xPixTiro + wPixTiro, yPixTiro + hPixTiro);
		glTexCoord2f(xTexTiro, yTexTiro);						glVertex2i(xPixTiro, yPixTiro + hPixTiro);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	} else if (_state == DISPARO_EXPLO) {
		int tex = _sis->idTextura(TEX_EXPLO1);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_EXPLO1, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
		int xPixTiro, yPixTiro, wPixTiro, hPixTiro;
		xTexTiro = tiroEnemigo3Explo[_seq][0] / (float)wTex;
		yTexTiro = tiroEnemigo3Explo[_seq][1] / (float)hTex;
		wTexTiro = tiroEnemigo3Explo[_seq][2] / (float)wTex;
		hTexTiro = tiroEnemigo3Explo[_seq][3] / (float)hTex;
		wPixTiro = tiroEnemigo3Explo[_seq][2];
		hPixTiro = tiroEnemigo3Explo[_seq][3];
		xPixTiro = _x - (wPixTiro>>1);
		yPixTiro = GAME_HEIGHT - (_y + (hPixTiro>>1));

		glTexCoord2f(xTexTiro, yTexTiro + hTexTiro);			glVertex2i(xPixTiro, yPixTiro);
		glTexCoord2f(xTexTiro + wTexTiro, yTexTiro + hTexTiro);	glVertex2i(xPixTiro + wPixTiro, yPixTiro);
		glTexCoord2f(xTexTiro + wTexTiro, yTexTiro);			glVertex2i(xPixTiro + wPixTiro, yPixTiro + hPixTiro);
		glTexCoord2f(xTexTiro, yTexTiro);						glVertex2i(xPixTiro, yPixTiro + hPixTiro);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}


