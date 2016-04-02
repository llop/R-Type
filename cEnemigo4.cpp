#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>


int e4MoveIzq[4][4] = {
	{ 1, 4, 32, 29 },
	{ 3, 40, 28, 26 },
	{ 34, 4, 32, 29 },
	{ 3, 40, 28, 26 }
};
int e4MoveDer[4][4] = {
	{ 167, 4, 32, 29 },
	{ 169, 40, 28, 26 },
	{ 134, 4, 32, 29 },
	{ 169, 40, 28, 26 }
};
int e4MovMid[4] = { 19, 52, 19, 52 };
int e4CaeIzq[4][4] = {
	{ 69, 1, 28, 32 },
	{ 34, 34, 32, 32 },
	{ 69, 1, 28, 32 },
	{ 67, 34, 32, 32 }
};
int e4CaeDer[4][4] = {
	{ 103, 1, 28, 32 },
	{ 134, 34, 32, 32 },
	{ 103, 1, 28, 32 },
	{ 1101, 34, 32, 32 }
};

int ene4Explo[9][4] = {
	{ 1, 38, 64, 56 },
	{ 66, 38, 64, 56 },
	{ 141, 47, 42, 38 },
	{ 203, 46, 51, 42 },
	{ 265, 40, 55, 52 },
	{ 1, 99, 64, 64 },
	{ 66, 99, 64, 64 },
	{ 131, 99, 64, 64 },
	{ 196, 99, 64, 64 }
};
int ene4ExploMid[2] = { 65, 131 };


cEnemigo4::cEnemigo4(cSistema* sis, int x, int y, int direccion) : cEnemigo(sis, x, y) {
	_sis->cargaTextura(TEX_ENE4, "img\\r-typesheet7.png");
	_sis->cargaTextura(TEX_EXPLO2, "img\\r-typesheet44.png");

	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = ENEMIGO4_MUEVE_DELAY;

	_subState = ENEMIGO4_ANDA;
	_direccion = direccion;

	_vida = _sis->dificultad()==DIFICULTAD_NORMAL ? ENEMIGO4_VIDA_INICIAL : ENEMIGO4_VIDA_INICIAL_HARD;
	_vida = int(_vida*((cNivel*)_sis->nivel())->factorDificultad());
	_puntos = ENEMIGO4_PUNTOS;

	_esJefe = false;
}

cEnemigo4::~cEnemigo4() {

}

void cEnemigo4::muerete() {
	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_seq = 0;
	_delay = ENEMIGO4_MUERE_DELAY;
}

void cEnemigo4::offset(int x, int y) {
	// el nivel no debe arrastrarlo con el scroll
}


void cEnemigo4::caja(cRect &rect) const {
	if (_subState == ENEMIGO4_ANDA) {
		int* mov = _direccion == ENEMIGO4_ANDA_IZQ ? e4MoveIzq[_seq] : e4MoveDer[_seq];
		rect.w = mov[2] - 2;
		rect.h = mov[3] - 2;
		rect.x = _x - (rect.w >> 1);
		rect.y = _y - (e4MovMid[_seq]-mov[1]);
	} else if (_subState == ENEMIGO4_CAE) {
		int* mov = _direccion == ENEMIGO4_ANDA_IZQ ? e4CaeIzq[_seq] : e4CaeDer[_seq];
		rect.w = mov[2] - 2;
		rect.h = mov[3] - 2;
		rect.x = _x - (rect.w >> 1);
		rect.y = _y - (rect.h >> 1);
	}
}

void cEnemigo4::colision(cRect &rect, int &colMask) const {
	cRect myRect;
	caja(myRect);
	colMask = 0;
	if (_state == ENEMIGO_VIVE) {
		if (myRect.x < rect.x + rect.w && myRect.x + myRect.w > rect.x &&
			myRect.y < rect.y + rect.h && myRect.y + myRect.h > rect.y) {
			colMask = 1;
		}
	}
}

void cEnemigo4::logica() {
	if (_state == ENEMIGO_VIVE) {

		// actualizar posicion
		if (_subState == ENEMIGO4_ANDA) {
			_x += _direccion == ENEMIGO4_ANDA_IZQ ? -ENEMIGO4_ANDA_INC : ENEMIGO4_ANDA_INC;
		} else {
			_y += ENEMIGO4_CAIDA_INC;
		}

		// estos los necesitaremos por ahi
		cNivel* nivel = (cNivel*)_sis->nivel();
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();

		// matar el enemigo cuando queda fuera de la pantalla
		cRect rect;
		caja(rect);
		if (nivel->fueraLimites(rect)) {
			_muerto = true;
			return;
		}

		// se lo habra cargado la nave?
		bool naveLoMata = false;

		// se come algun disparo bueno?
		list<cDisparo*> disparos = nivel->disparos();
		for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
			cDisparo* disparo = *it;
			if (!disparo->malo() && disparo->vive()) {
				int colMask;
				disparo->colision(rect, colMask);
				if (colMask) {
					disparo->muerete();
					// aplicar efecto del disparo
					_vida -= disparo->dano();
					if (_vida <= 0) naveLoMata = true;
				}
			}
		}

		// se come algun escudo?
		vector<cEscudo*> escudos = nave->escudos();
		for (unsigned int i = 0; i < escudos.size(); ++i) {
			cEscudo* escudo = escudos[i];
			int colMask;
			escudo->colision(rect, colMask);
			if (colMask) {
				_vida -= escudo->dano();
				if (_vida <= 0) naveLoMata = true;
			}
		}

		// si ha sido la nave, habra que darle los puntos
		if (naveLoMata) {
			nave->sumaPuntos(_puntos);
		}

		// si acaba de palmar, nos detenemos aquí
		if (_vida <= 0) {
			muerete();
			return;
		}

		// actualizar estado si es necesario
		if (_subState == ENEMIGO4_ANDA) {
			// si el malo anda,
			// ver si choca la caja con el suelo si la hacemos un pixel más alta
			++rect.h;
			int colisionMask, x, y, objeto;
			nivel->colisionNivel(rect, colisionMask, x, y, objeto);
			if (objeto&COLISION_TILE && colisionMask&COLISION_ABAJO) {
				// choca por abajo: que siga caminando
				// y cambiar de direccion si choca por los lados
				_y -= (rect.y + rect.h) - y;
				caja(rect);
				nivel->colisionNivel(rect, colisionMask, x, y, objeto);
				if (objeto&COLISION_TILE) {
					if (_direccion == ENEMIGO4_ANDA_IZQ) {
						if (colisionMask & COLISION_IZQ) {
							_direccion = ENEMIGO4_ANDA_DER;
						}
					} else if (_direccion == ENEMIGO4_ANDA_DER) {
						if (colisionMask & COLISION_DER) {
							_direccion = ENEMIGO4_ANDA_IZQ;
						}
					}
				}
			} else {
 				_subState = ENEMIGO4_CAE;
				_seq = 0;
				_delay = ENEMIGO4_MUEVE_DELAY;
			}
		} else if (_subState == ENEMIGO4_CAE) {
			int colisionMask, x, y, objeto;
			nivel->colisionNivel(rect, colisionMask, x, y, objeto);
			if (objeto&COLISION_TILE && colisionMask&COLISION_ABAJO) {
				// choca por abajo: pasa a caminar
				_y -= (rect.y + rect.h) - y - 3;
				_subState = ENEMIGO4_ANDA;
				_seq = 0;
				_delay = ENEMIGO4_MUEVE_DELAY;
			}
		}

		// animacion
		if (_delay) --_delay;
		else {
			_seq = (_seq + 1) % ENEMIGO4_MOV_FRAMES;
			_delay = ENEMIGO4_MUEVE_DELAY;
		}

		//IA DISPAROS
		int modulo = _sis->dificultad() == DIFICULTAD_NORMAL ? ENEMIGO4_DISPARO : ENEMIGO4_DISPARO_HARD;
		modulo = int(modulo / nivel->factorDificultad());
		if (!(rand() % modulo)) {
			// meter el nuevo disparo en el nivel
			int nX, nY;
			nave->getPosicion(nX, nY);
			float vectX = float(nX - _x);
			float vectY = float(nY - _y);
			float len = sqrt(vectX*vectX + vectY*vectY);
			nivel->pushDisparo(new cDisparoEnemigo(_sis, _x, _y, vectX/len, vectY/len));
		}
	}
	else if (_state == ENEMIGO_EXPLO) {
		// avanzar en la animacion
		if (_delay) {
			--_delay;
		}
		else {
			++_seq;
			_delay = ENEMIGO4_MUERE_DELAY;
		}
		if (_seq == ENEMIGO4_EXPLO_FRAMES) {
			_state = ENEMIGO_MUERE;
			_muerto = true;
		}
	}
}

void cEnemigo4::pinta() const {
	if (_state == ENEMIGO_MUERE) return;

	float xTexEne, yTexEne, wTexEne, hTexEne;
	int xPixEne, yPixEne, yPixOffset, wPixEne, hPixEne;

	if (_state == ENEMIGO_VIVE) {
		int tex = _sis->idTextura(TEX_ENE4);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_ENE4, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		int* mov = _direccion == ENEMIGO4_ANDA_IZQ ?
			(_subState == ENEMIGO4_ANDA ? e4MoveIzq[_seq] : e4CaeIzq[_seq]) :
			(_subState == ENEMIGO4_ANDA ? e4MoveDer[_seq] : e4CaeDer[_seq]);
		wPixEne = mov[2];
		hPixEne = mov[3];
		xTexEne = mov[0] / (float)wTex;
		yTexEne = mov[1] / (float)hTex;
		wTexEne = wPixEne / (float)wTex;
		hTexEne = hPixEne / (float)hTex;
		xPixEne = _x - (wPixEne >> 1);
		yPixOffset = _subState == ENEMIGO4_ANDA ? e4MovMid[_seq]-mov[1] : hPixEne>>1;
		yPixEne = GAME_HEIGHT - (_y - yPixOffset + hPixEne);
	}
	else if (_state == ENEMIGO_EXPLO) {
		int tex = _sis->idTextura(TEX_EXPLO2);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_EXPLO2, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		wPixEne = ene4Explo[_seq][2];
		hPixEne = ene4Explo[_seq][3];
		xTexEne = ene4Explo[_seq][0] / (float)wTex;
		yTexEne = ene4Explo[_seq][1] / (float)hTex;
		wTexEne = wPixEne / (float)wTex;
		hTexEne = hPixEne / (float)hTex;
		xPixEne = _x - (wPixEne >> 1);
		yPixOffset = ene4ExploMid[_seq / 5] - ene4Explo[_seq][1];
		yPixEne = GAME_HEIGHT - (_y - yPixOffset + hPixEne);
	}

	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
