#include "cEscudo.h"
#include "cNivel.h"


int escudoMov[4][4] = {
	{ 164, 379, 31, 24 },
	{ 198, 381, 31, 20 },
	{ 233, 379, 31, 24 },
	{ 268, 377, 31, 28 }
};
int escudoMid = 390;

int escudoSecMov[12][4] = {
	{ 1, 2, 16, 12 },
	{ 18, 2, 16, 12 },
	{ 35, 2, 16, 12 },
	{ 52, 2, 16, 14 },
	{ 69, 4, 16, 12 },
	{ 86, 4, 16, 12 },
	{ 103, 4, 16, 12 },
	{ 120, 4, 16, 12 },
	{ 137, 4, 16, 12 },
	{ 154, 2, 16, 14 },
	{ 171, 2, 16, 12 },
	{ 188, 2, 16, 12 }
};
int escudoSecMid = 8;


cEscudo::cEscudo(cSistema* sis, int x, int y, int tipo) : cSprite(sis, x, y) {
	_sis->cargaTextura(TEX_NAVE2, "img\\r-typesheet2.png");
	_sis->cargaTextura(TEX_ESCUDO, "img\\r-typesheet3.png");

	_state = ESCUDO_VUELTA;
	_seq = 0;
	_delay = ESCUDO_MUEVE_DELAY;

	_dano = ESCUDO_DANO;
	_tipo = tipo;

	_numFrames = ESCUDO_NUM_FRAMES;
	if (_tipo != ESCUDO_FRENTE) _numFrames = ESCUDO_SEC_NUM_FRAMES;
	
	_xF = float(_x);
	_yF = float(_y);
	_xAnt = _xF;
	_yAnt = _yF;
}

cEscudo::~cEscudo() {

}

void cEscudo::offset(int x, int y) {
	_xF += float(x);
	_yF += float(y);
	_x = int(_xF);
	_y = int(_yF);
}

void cEscudo::dispara() {
	cNivel* nivel = (cNivel*)_sis->nivel();
	int xTiro = _x;
	int yTiro = _y;
	
	// meter el nuevo disparo en el nivel
	int tipoDisparo = DISPARO_ESCUDO1_FRENTE;
	if (_tipo == ESCUDO_ARRIBA) tipoDisparo = DISPARO_ESCUDO2_ARRIBA;
	if (_tipo == ESCUDO_ABAJO) tipoDisparo = DISPARO_ESCUDO2_ABAJO;
	cDisparoEscudo* tiro = new cDisparoEscudo(_sis, xTiro, yTiro, tipoDisparo);
	nivel->pushDisparo(tiro);

	// el escudo desanclado dispara arriba y abajo
	if (_tipo == ESCUDO_FRENTE && _state != ESCUDO_ANCLADO) {
		xTiro = _x + (escudoMov[_seq][2]>>1);
		yTiro = _y;
		cDisparoEscudo* tiroArriba = new cDisparoEscudo(_sis, xTiro, yTiro, DISPARO_ESCUDO1_ARRIBA);
		nivel->pushDisparo(tiroArriba);
		cDisparoEscudo* tiroAbajo = new cDisparoEscudo(_sis, xTiro, yTiro, DISPARO_ESCUDO1_ABAJO);
		nivel->pushDisparo(tiroAbajo);
	}
}

void cEscudo::lanza() {
	if (_state == ESCUDO_ANCLADO) _state = ESCUDO_TIRO;
}

void cEscudo::choca() {
	if (_state == ESCUDO_TIRO) _state = ESCUDO_VUELTA;
}

void cEscudo::muerete() {
	_muerto = true;
}

bool cEscudo::anclado() const {
	return _state == ESCUDO_ANCLADO;
}
int cEscudo::dano() const {
	return _dano;
}

void cEscudo::caja(cRect &rect) const {
	if (_tipo == ESCUDO_FRENTE) {
		rect.w = escudoMov[_seq][2];
		rect.h = escudoMov[_seq][3];
		rect.x = _x;
		rect.y = _y - (escudoMid - escudoMov[_seq][1]);
	} else {
		rect.w = escudoSecMov[_seq][2];
		rect.h = escudoSecMov[_seq][3];
		rect.x = _x - (rect.w>>1);
		rect.y = _y - (escudoSecMid - escudoSecMov[_seq][1]);
	}
}

void cEscudo::colision(cRect &rect, int &colMask) const {
	cRect myRect;
	caja(myRect);
	colMask = 0;
	if (myRect.x < rect.x+rect.w && myRect.x+myRect.w > rect.x &&
		myRect.y < rect.y+rect.h && myRect.y+myRect.h > rect.y) {
			colMask = 1;
	}
}

void cEscudo::logica() {
	// avance
	if (_tipo == ESCUDO_FRENTE) {
		if (_state == ESCUDO_VUELTA) {
			cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
			int xNave, yNave;
			nave->getPosicion(xNave, yNave);
			xNave -= 4;
			float xEscudo = _xF - (escudoMov[_seq][2]>>1);
			float yEscudo = _yF;

			float xVec = xNave - xEscudo;
			float yVec = yNave - yEscudo;
			float dist = sqrt(xVec*xVec + yVec*yVec);

			if (dist <= ESCUDO_DIST_ANCLA) {
				_state = ESCUDO_ANCLADO;
				_xF = float(xNave + (escudoMov[_seq][2]>>1));
				_yF = float(yNave);
			} else {
				xVec /= dist;
				yVec /= dist;
				xVec *= ESCUDO_VUELTA_SPEED;
				yVec *= ESCUDO_VUELTA_SPEED;
				_xF += xVec;
				_yF += yVec;
			}
		} else if (_state == ESCUDO_ANCLADO) {
			cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
			int xNave, yNave;
			nave->getPosicion(xNave, yNave);
			xNave -= 4;
			_xF = float(xNave + (escudoMov[_seq][2]>>1));
			_yF = float(yNave);
		} else if (_state == ESCUDO_TIRO) {
			_xF += ESCUDO_TIRO_SPEED;
			// ha chocado con el escenario?
			cRect rect;
			caja(rect);
			int colisionMask, x, y, objeto;
			cNivel* nivel = (cNivel*)_sis->nivel();
			nivel->colisionNivel(rect, colisionMask, x, y, objeto);
			// si ha chocado, que se vuelva a la nave
			if (colisionMask) _state = ESCUDO_VUELTA;
		}
	} else if (_tipo == ESCUDO_ARRIBA) {
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
		int xNave, yNave;
		nave->getPosicion(xNave, yNave);
		_xF = float(xNave - NAVE_ESCUDO_SEC_X_OFFSET);
		_yF = float(yNave - NAVE_ESCUDO_SEC_Y_OFFSET);
	} else if (_tipo == ESCUDO_ABAJO) {
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
		int xNave, yNave;
		nave->getPosicion(xNave, yNave);
		_xF = float(xNave - NAVE_ESCUDO_SEC_X_OFFSET);
		_yF = float(yNave + NAVE_ESCUDO_SEC_Y_OFFSET);
	}
	_x = int(_xF);
	_y = int(_yF);

	cNivel* nivel = (cNivel*)_sis->nivel();
	if (nivel != NULL) {
		// placa algun disparo malo?
		cRect rect;
		caja(rect);
		list<cDisparo*> disparos = nivel->disparos();
		for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
			cDisparo* disparo = *it;
			if (disparo->malo() && disparo->dano()<ESCUDO_DANO) {
				int colMask;
  				disparo->colision(rect, colMask);
				if (colMask) disparo->muerete();
			}
		}
	}

	// animacion
	if (_delay) --_delay;
	else {
		_seq = (_seq + 1) % _numFrames;
		_delay = ESCUDO_MUEVE_DELAY;
	}

}
void cEscudo::pinta() const {
	float xTexEsc, yTexEsc, wTexEsc, hTexEsc;
	int xPixEsc, yPixEsc, wPixEsc, hPixEsc;
	
	if (_tipo == ESCUDO_FRENTE) {
		int tex = _sis->idTextura(TEX_NAVE2);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_NAVE2, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexEsc = escudoMov[_seq][0]/(float)wTex;
		yTexEsc = escudoMov[_seq][1]/(float)hTex;
		wTexEsc = escudoMov[_seq][2]/(float)wTex;
		hTexEsc = escudoMov[_seq][3]/(float)hTex;
		wPixEsc = escudoMov[_seq][2];
		hPixEsc = escudoMov[_seq][3];
		xPixEsc = _x;
		yPixEsc = GAME_HEIGHT - (_y - (escudoMid - escudoMov[_seq][1]) + hPixEsc);
	} else {
		int tex = _sis->idTextura(TEX_ESCUDO);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_ESCUDO, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexEsc = escudoSecMov[_seq][0]/(float)wTex;
		yTexEsc = escudoSecMov[_seq][1]/(float)hTex;
		wTexEsc = escudoSecMov[_seq][2]/(float)wTex;
		hTexEsc = escudoSecMov[_seq][3]/(float)hTex;
		wPixEsc = escudoSecMov[_seq][2];
		hPixEsc = escudoSecMov[_seq][3];
		xPixEsc = _x - (wPixEsc>>1);
		yPixEsc = GAME_HEIGHT - (_y - (escudoSecMid - escudoSecMov[_seq][1]) + hPixEsc);
	}

	glTexCoord2f(xTexEsc, yTexEsc + hTexEsc);			glVertex2i(xPixEsc, yPixEsc);
	glTexCoord2f(xTexEsc + wTexEsc, yTexEsc + hTexEsc);	glVertex2i(xPixEsc + wPixEsc, yPixEsc);
	glTexCoord2f(xTexEsc + wTexEsc, yTexEsc);			glVertex2i(xPixEsc + wPixEsc, yPixEsc + hPixEsc);
	glTexCoord2f(xTexEsc, yTexEsc);						glVertex2i(xPixEsc, yPixEsc + hPixEsc);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}