
#include "cDisparo.h"
#include "cNivel.h"
#include <gl/glut.h>



int tiroJefe1[4][4] = {
	{ 576, 2063, 21, 20 },
	{ 599, 2063, 22, 19 },
	{ 623, 2063, 21, 20 },
	{ 646, 2064, 22, 19 }
};
int tiroJefe1Mid = 2072;


cDisparoJefe1::cDisparoJefe1(cSistema* sis, int x, int y, float xVec, float yVec) : cDisparo(sis, x, y) {
	_sis->cargaTextura(TEX_JEFE1, "img\\r-typesheet30.png");

	_dano = DANO_DISPARO_JEFE1;
	_malo = true;
	
	_xF = float(_x);
	_yF = float(_y);

	_state = DISPARO_VIVE;
	_seq = 0;
	_delay = DISPARO_ENEMIGO_DELAY;

	float dist = sqrt(xVec*xVec + yVec*yVec);
	_xVec = xVec/dist;
	_yVec = yVec/dist;

	_sis->playSonido(SOUND_DISPARO_JEFE1);
}

cDisparoJefe1::~cDisparoJefe1() {
}

void cDisparoJefe1::explota() {
	muerete();
}

void cDisparoJefe1::muerete() {
	_state = DISPARO_EXPLO;
	_seq = 3;
	_delay = DISPARO_EXPLO_DELAY;
}


void cDisparoJefe1::colision(cRect &rect, int &colMask) const {
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

void cDisparoJefe1::caja(cRect &rect) const {
	rect.w = tiroJefe1[_seq][2];
	rect.h = tiroJefe1[_seq][3];
	rect.x = _x - (rect.w>>1);
	int yPixOffsetTiro = tiroJefe1Mid - tiroJefe1[_seq][1];
	rect.y = _y - yPixOffsetTiro;
}

void cDisparoJefe1::logica() {
	if (_state == DISPARO_VIVE) {
		cNivel* nivel = (cNivel*)_sis->nivel();
		_xF += _xVec*VELOCIDAD_DISPARO_JEFE1;
		_yF += _yVec*VELOCIDAD_DISPARO_JEFE1;
	
		_x = int(_xF);
		_y = int(_yF);

		cRect rect;
		caja(rect);
		if (nivel->fueraLimites(rect)) {
			_muerto = true;
			return;
		}

		if (_delay) --_delay;
		else {
			_seq = (_seq + 1) % 4;
			_delay = DISPARO_JEFE1_DELAY;
		}
	} else if (_state == DISPARO_EXPLO) {
		if (_delay) --_delay;
		else {
 			_muerto = true;
		}
	}
}

void cDisparoJefe1::pinta() const {
	if (_state == DISPARO_MUERE) return;

	int tex = _sis->idTextura(TEX_JEFE1);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_JEFE1, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
	int xPixTiro, yPixTiro, yPixOffsetTiro, wPixTiro, hPixTiro;

	//if (_state == DISPARO_VIVE) {
	xTexTiro = tiroJefe1[_seq][0] / (float)wTex;
	yTexTiro = tiroJefe1[_seq][1] / (float)hTex;
	wTexTiro = tiroJefe1[_seq][2] / (float)wTex;
	hTexTiro = tiroJefe1[_seq][3] / (float)hTex;
	wPixTiro = tiroJefe1[_seq][2];
	hPixTiro = tiroJefe1[_seq][3];
	xPixTiro = _x - (wPixTiro>>1);
	yPixOffsetTiro = tiroJefe1Mid - tiroJefe1[_seq][1];
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


