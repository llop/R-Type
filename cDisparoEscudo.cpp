
#include "cDisparo.h"
#include "cNivel.h"
#include <gl/glut.h>


#include "cDisparo.h"

int tiroEscudoDie[2][4] = {
	{ 303, 85, 14, 12 },
	{ 288, 87, 11, 9 }
};

int tiroEscudoFrente[4] = { 283, 59, 16, 4 };
int tiroEscudoArriba[4] = { 255, 436, 4, 15 };
int tiroEscudoAbajo[4] = { 238, 435, 4, 15 };

int tiroEscudo2ArribaMov[2][4] = { 
	{ 89, 313, 32, 4 },
	{ 123, 313, 32, 4 }
};
int tiroEscudo2AbajoMov[2][4] = { 
	{ 89, 330, 32, 4 },
	{ 123, 330, 32, 4 }
};


cDisparoEscudo::cDisparoEscudo(cSistema* sis, int x, int y, int tipo) : cDisparo(sis, x, y) {
	_sis->cargaTextura(TEX_NAVE1, "img\\r-typesheet1.png");
	_sis->cargaTextura(TEX_NAVE2, "img\\r-typesheet2.png");

	_tipo = tipo;
	_dano = DISPARO_ESCUDO_DANO;

	_malo = false;

	_state = DISPARO_VIVE;
	_seq = 0;
	_delay = DISPARO_NAVE_DELAY;

}

cDisparoEscudo::~cDisparoEscudo() {
}

void cDisparoEscudo::caja(cRect &rect) const {
	int* tiro = tiroEscudoFrente;
	if (_tipo == DISPARO_ESCUDO1_ARRIBA) tiro = tiroEscudoArriba;
	if (_tipo == DISPARO_ESCUDO1_ABAJO) tiro = tiroEscudoAbajo;
	if (_tipo == DISPARO_ESCUDO2_ARRIBA) tiro = tiroEscudo2ArribaMov[_seq];
	if (_tipo == DISPARO_ESCUDO2_ABAJO) tiro = tiroEscudo2AbajoMov[_seq];
	rect.w = tiro[2];
	rect.h = tiro[3];
	rect.x = _x - (rect.w>>1);
	rect.y = _y - (rect.h>>1);
}



void cDisparoEscudo::colision(cRect &rect, int &colMask) const {
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

void cDisparoEscudo::muerete() {
	_state = DISPARO_EXPLO;
	_seq = 0;
	_delay = DISPARO_EXPLO_DELAY;
}

void cDisparoEscudo::logica() {
	if (_state == DISPARO_VIVE) {
		cNivel* nivel = (cNivel*)_sis->nivel();

		// avanzar
		if (_tipo==DISPARO_ESCUDO1_ARRIBA) {
			_y -= DISPARO_ESCUDO_AVANCE;
		} else if (_tipo==DISPARO_ESCUDO1_ABAJO) {
			_y += DISPARO_ESCUDO_AVANCE;
		} else {
			_x += DISPARO_ESCUDO_AVANCE;
		}

		cRect rect;
		caja(rect);
		if (nivel->fueraLimites(rect)) {
			// matar el tiro cuando queda fuera de la pantalla
			_muerto = true;
			return;
		}

		// si choca contra el escenario, muerte
		int colMask;
  		nivel->colision(rect, colMask);
		if (colMask) {
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

void cDisparoEscudo::pinta() const {
	if (_state == DISPARO_MUERE) return;

	float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
	int xPixTiro, yPixTiro, wPixTiro, hPixTiro;
	
	// pillar las coordenadas adecuadas
	int* tiro = tiroEscudoFrente;
	if (_tipo == DISPARO_ESCUDO1_ARRIBA) tiro = tiroEscudoArriba;
	if (_tipo == DISPARO_ESCUDO1_ABAJO) tiro = tiroEscudoAbajo;
	if (_tipo == DISPARO_ESCUDO2_ARRIBA) tiro = tiroEscudo2ArribaMov[_seq];
	if (_tipo == DISPARO_ESCUDO2_ABAJO) tiro = tiroEscudo2AbajoMov[_seq];
		
	if (_state == DISPARO_VIVE) {
		int tex = _sis->idTextura(TEX_NAVE2);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_NAVE2, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		// calcular
		xTexTiro = tiro[0]/(float)wTex;
		yTexTiro = tiro[1]/(float)hTex;
		wTexTiro = tiro[2]/(float)wTex;
		hTexTiro = tiro[3]/(float)hTex;
		wPixTiro = tiro[2];
		hPixTiro = tiro[3];
		xPixTiro = _x - (wPixTiro>>1);
		yPixTiro = GAME_HEIGHT - (_y - (hPixTiro>>1) + hPixTiro);
	} else if (_state == DISPARO_EXPLO) {
		int tex = _sis->idTextura(TEX_NAVE1);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_NAVE1, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexTiro = tiroEscudoDie[_seq][0]/(float)wTex;
		yTexTiro = tiroEscudoDie[_seq][1]/(float)hTex;
		wTexTiro = tiroEscudoDie[_seq][2]/(float)wTex;
		hTexTiro = tiroEscudoDie[_seq][3]/(float)hTex;
		wPixTiro = tiroEscudoDie[_seq][2];
		hPixTiro = tiroEscudoDie[_seq][3];
		xPixTiro = _x + (tiro[2]>>1) - wPixTiro;
		yPixTiro = GAME_HEIGHT - (_y - (hPixTiro>>1) + hPixTiro);
	}

	glTexCoord2f(xTexTiro, yTexTiro + hTexTiro);			glVertex2i(xPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro + hTexTiro);	glVertex2i(xPixTiro + wPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro);			glVertex2i(xPixTiro + wPixTiro, yPixTiro + hPixTiro);
	glTexCoord2f(xTexTiro, yTexTiro);						glVertex2i(xPixTiro, yPixTiro + hPixTiro);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


