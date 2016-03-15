
#include "cDisparo.h"
#include "cNivel.h"
#include <gl/glut.h>


#include "cDisparo.h"


int tiroNave[14][4] = {
	{ 303, 85, 14, 12 },
	{ 288, 87, 11, 9 },
	{ 249, 90, 16, 4 },
	{ 249, 90, 16, 4 },
	{ 249, 105, 16, 8 },
	{ 232, 103, 16, 12 },
	{ 200, 121, 32, 10 },
	{ 233, 120, 32, 12 },
	{ 168, 137, 48, 12 },
	{ 217, 136, 48, 14 },
	{ 136, 154, 64, 14 },
	{ 201, 154, 64, 14 },
	{ 104, 171, 80, 14 },
	{ 185, 170, 80, 16 }
};
int tiroNaveMid[6] = { 91, 108, 125, 142, 160, 177 };


cDisparoNave::cDisparoNave(cSistema* sis, int x, int y, int tamano) : cDisparo(sis, x, y) {
	_tamano = tamano;
	_dano = (_tamano + 1) * 20;

	_malo = false;

	_state = DISPARO_VIVE;
	_seq = 0;
	_delay = DISPARO_NAVE_DELAY;

	_pixelsAvanza = tiroNave[0][2] - _tamano - (_tamano>>1);
}

cDisparoNave::~cDisparoNave() {
}

void cDisparoNave::getCaja(cRect &rect) const {
	int numSeq = 2 + (2 * _tamano);
	rect.w = tiroNave[numSeq][2];
	rect.h = tiroNave[numSeq][3];
	rect.x = _x;
	rect.y = _y;
}



void cDisparoNave::colision(cRect &rect, int &colMask) const {
	colMask = 0;
	if (_state == DISPARO_VIVE) {
		cRect myRect;
		getCaja(myRect);
		if (myRect.x < rect.x+rect.w && myRect.x+myRect.w > rect.x &&
			myRect.y < rect.y+rect.h && myRect.y+myRect.h > rect.y) {
				colMask = 1;
		}
	}
}

void cDisparoNave::muerete() {
	if (!_tamano) {
		_state = DISPARO_EXPLO;
		_seq = 0;
		_delay = DISPARO_EXPLO_DELAY;
	}
}

void cDisparoNave::logica() {
	if (_state == DISPARO_VIVE) {
		cNivel* nivel = ((cNivel*)_sis->getNivel());
		_x += _pixelsAvanza;
		if (_x > GAME_WIDTH + nivel->getPosicion()) {
			// matar el tiro cuando queda fuera de la pantalla
			_muerto = true;
			return;
		}

		// si choca contra el escenario, muerte
		cRect rect;
		getCaja(rect);
		int colMask;
		nivel->colision(rect, colMask);
		if (colMask) {
			// aunque pueden atravesar varios enemigos de una, 
			// los disparos grandes mueren contra el escenario
			_state = DISPARO_EXPLO;
			_seq = 0;
			_delay = DISPARO_EXPLO_DELAY;
			return;
		}

		if (_delay) --_delay;
		else {
			_seq = !_seq;
			_delay = DISPARO_NAVE_DELAY;
		}
	} else if (_state == DISPARO_EXPLO) {
		if (_delay) --_delay;
		else {
			if (_seq) {
				_state = DISPARO_MUERE;
				_muerto = true;
			} else {
				_seq = !_seq;
				_delay = DISPARO_EXPLO_DELAY;
			}
		}
	}
}

void cDisparoNave::pinta() const {
	if (_state == DISPARO_MUERE) return;

	int tex = _sis->getIdTextura(TEX_NAVE);
	int wTex, hTex;
	_sis->getTamanoTextura(TEX_NAVE, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	
	float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
	int xPixTiro, yPixTiro, yPixOffsetTiro, wPixTiro, hPixTiro;
	
	if (_state == DISPARO_VIVE) {
		int seqTiro = 2 + (2 * _tamano) + _seq;
		xTexTiro = tiroNave[seqTiro][0]/(float)wTex;
		yTexTiro = tiroNave[seqTiro][1]/(float)hTex;
		wTexTiro = tiroNave[seqTiro][2]/(float)wTex;
		hTexTiro = tiroNave[seqTiro][3]/(float)hTex;
		wPixTiro = tiroNave[seqTiro][2];
		hPixTiro = tiroNave[seqTiro][3];
		xPixTiro = _x;
		yPixOffsetTiro = tiroNaveMid[_tamano] - tiroNave[seqTiro][1];
		yPixTiro = GAME_HEIGHT - (_y - yPixOffsetTiro + hPixTiro);
	} else if (_state == DISPARO_EXPLO) {
		xTexTiro = tiroNave[_seq][0]/(float)wTex;
		yTexTiro = tiroNave[_seq][1]/(float)hTex;
		wTexTiro = tiroNave[_seq][2]/(float)wTex;
		hTexTiro = tiroNave[_seq][3]/(float)hTex;
		wPixTiro = tiroNave[_seq][2];
		hPixTiro = tiroNave[_seq][3];
		xPixTiro = _x + tiroNave[2 + (2 * _tamano)][2] - tiroNave[_seq][2];
		yPixOffsetTiro = tiroNaveMid[0] - tiroNave[_seq][1];
		yPixTiro = GAME_HEIGHT - (_y - yPixOffsetTiro + hPixTiro);
	}

	glTexCoord2f(xTexTiro, yTexTiro + hTexTiro);			glVertex2i(xPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro + hTexTiro);	glVertex2i(xPixTiro + wPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro);			glVertex2i(xPixTiro + wPixTiro, yPixTiro + hPixTiro);
	glTexCoord2f(xTexTiro, yTexTiro);						glVertex2i(xPixTiro, yPixTiro + hPixTiro);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


