
#include "cDisparo.h"
#include "cNivel.h"
#include <gl/glut.h>



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


int tiroCirMov[24][4] = {
	{ 54, 552, 12, 36 },
	{ 71, 547, 18, 46 },
	{ 104, 543, 27, 54 },
	{ 137, 542, 37, 56 },
	{ 187, 542, 47, 56 },
	{ 235, 542, 55, 56 },
	{ 40, 608, 61, 56 },
	{ 105, 608, 64, 56 },
	{ 172, 608, 60, 56 },
	{ 235, 608, 64, 56 },
	{ 40, 674, 63, 56 },
	{ 105, 674, 60, 56 },
	{ 170, 676, 58, 52 },
	{ 238, 678, 58, 48 },
	{ 40, 745, 64, 44 },
	{ 105, 750, 64, 32 },
	{ 37, 470, 64, 32 },
	{ 103, 470, 59, 32 },
	{ 169, 470, 59, 32 },
	{ 237, 470, 62, 32 },
	{ 37, 504, 64, 32 },
	{ 103, 504, 59, 32 },
	{ 169, 504, 59, 32 },
	{ 237, 504, 62, 32 }
};
int tiroCircMid[24] = {
	569, 569, 569, 569, 569, 569, 635, 635, 635, 635, 701, 701, 701, 701, 765, 765, 485, 485, 485, 485, 519, 519, 519, 519
};
int tiroCircAva[24] = {
	0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

cDisparoNave::cDisparoNave(cSistema* sis, int x, int y, int tipo, int tamano) : cDisparo(sis, x, y) {
	_sis->cargaTextura(TEX_NAVE1, "img\\r-typesheet1.png");
	_sis->cargaTextura(TEX_NAVE2, "img\\r-typesheet2.png");

	_tamano = tamano;
	_dano = 20 + _tamano*100;
	_malo = false;

	_tipo = tipo;
	_pixelsAvanza = tiroNave[0][2] - _tamano - (_tamano>>1);
	

	_state = DISPARO_VIVE;
	_seq = 0;
	_delay = !_tamano && _tipo==DISPARO_NAVE_CIRCULAR ? DISPARO_NAVE_CIR_DELAY : DISPARO_NAVE_DELAY;
}

cDisparoNave::~cDisparoNave() {
}

void cDisparoNave::caja(cRect &rect) const {
	if (_tamano || _tipo==DISPARO_NAVE_NORMAL) {
		int numSeq = 2 + (2 * _tamano);
		rect.w = tiroNave[numSeq][2];
		rect.h = tiroNave[numSeq][3];
		rect.x = _x;
		rect.y = _y;
	} else {
		rect.w = tiroCirMov[_seq][2];
		rect.h = tiroCirMov[_seq][3];
		rect.x = _x;
		rect.y = _y - (tiroCirMov[_seq][3]>>1);
	}
}



void cDisparoNave::colision(cRect &rect, int &colMask) const {
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

void cDisparoNave::explota() {
	_state = DISPARO_EXPLO;
	_seq = 0;
	_delay = DISPARO_EXPLO_DELAY;
}

void cDisparoNave::muerete() {
	if (!_tamano && _tipo==DISPARO_NAVE_NORMAL) {
		_state = DISPARO_EXPLO;
		_seq = 0;
		_delay = DISPARO_EXPLO_DELAY;
	}
}

void cDisparoNave::logica() {
	if (_state == DISPARO_VIVE) {
		cNivel* nivel = ((cNivel*)_sis->nivel());
		
		// avanza disparo
		bool circular = !_tamano && _tipo==DISPARO_NAVE_CIRCULAR;
		if (circular) _x += tiroCircAva[_seq];
		else _x += _pixelsAvanza;


		// si queda fuera del escenario, matarlo de inmediato
		if (_x > GAME_WIDTH + nivel->getPosicion()) {
			// matar el tiro cuando queda fuera de la pantalla
			_muerto = true;
			return;
		}

		// los tiros tienen una hit box menor sólo para las colisiones con el escenario
		cRect rect;
		caja(rect);
		int h=min(rect.h, 4);
		rect.y=rect.y+(rect.h>>1)-(h>>1);
		rect.h=h;
		
		int colMask;
  		nivel->colision(rect, colMask);
		if (colMask) {
			// aunque pueden atravesar varios enemigos de una, 
			// los disparos grandes mueren contra el escenario
			explota();
			return;
		}

		if (_delay) --_delay;
		else {
			if (_tamano || _tipo==DISPARO_NAVE_NORMAL) {
				_seq = !_seq;
				_delay = DISPARO_NAVE_DELAY;
			} else {
				++_seq;
				if (_seq==24) _seq=16;
				_delay = DISPARO_NAVE_CIR_DELAY;
			}
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

	// el disparo circular tiene un tratamiento especial
	bool circular = !_tamano && _tipo==DISPARO_NAVE_CIRCULAR;
	
	float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
	int xPixTiro, yPixTiro, yPixOffsetTiro, wPixTiro, hPixTiro;
	
	if (_state == DISPARO_VIVE) {
		int texId = circular ? TEX_NAVE2 : TEX_NAVE1;
		int tex = _sis->idTextura(texId);
		int wTex, hTex;
		_sis->tamanoTextura(texId, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		// estos parametros dependen del tipo de tiro
		int seqTiro = circular ? _seq : 2 + (2 * _tamano) + _seq;
		int* tiro = circular ? tiroCirMov[seqTiro] : tiroNave[seqTiro];
		int mid = circular ? tiroCircMid[_seq] : tiroNaveMid[_tamano];

		xTexTiro = tiro[0]/float(wTex);
		yTexTiro = tiro[1]/float(hTex);
		wTexTiro = tiro[2]/float(wTex);
		hTexTiro = tiro[3]/float(hTex);
		wPixTiro = tiro[2];
		hPixTiro = tiro[3];
		xPixTiro = _x;
		yPixOffsetTiro = mid - tiro[1];
		yPixTiro = GAME_HEIGHT - (_y - yPixOffsetTiro + hPixTiro);
	} else if (_state == DISPARO_EXPLO) {
		
		int tex = _sis->idTextura(TEX_NAVE1);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_NAVE1, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexTiro = tiroNave[_seq][0]/(float)wTex;
		yTexTiro = tiroNave[_seq][1]/(float)hTex;
		wTexTiro = tiroNave[_seq][2]/(float)wTex;
		hTexTiro = tiroNave[_seq][3]/(float)hTex;
		wPixTiro = tiroNave[_seq][2];
		hPixTiro = tiroNave[_seq][3];

		if (circular) xPixTiro = _x + tiroCirMov[_seq][2]*4 - tiroNave[_seq][2];
		else xPixTiro = _x + tiroNave[2 + (2 * _tamano)][2] - tiroNave[_seq][2];
		
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


