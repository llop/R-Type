
#include "cNaveEspacial.h"
#include "cNivel.h"
#include <gl/glut.h>



// coords textura nave en movimiento
int mov[5][4] = {  
	{ 101, 3, 32, 14 },
	{ 134, 3, 32, 13 },
	{ 167, 3, 32, 12 },
	{ 200, 3, 32, 13 },
	{ 233, 2, 32, 14 }
};
int movMid = 10;

// coords textura nave explotando
int die[8][4] = {
	{ 1, 343, 32, 28 },
	{ 35, 343, 32, 28 },
	{ 73, 348, 21, 18 },
	{ 106, 347, 22, 20 },
	{ 138, 346, 24, 22 },
	{ 170, 345, 26, 24 },
	{ 202, 344, 28, 26 },
	{ 233, 343, 32, 28 }
};
int dieMid = 356;

int dis[8][4] = {
	{ 2, 51, 31, 32 },
	{ 35, 52, 28, 28 },
	{ 68, 54, 29, 28 },
	{ 103, 55, 25, 25 },
	{ 135, 56, 29, 22 },
	{ 168, 58, 26, 18 },
	{ 200, 59, 23, 15 },
	{ 233, 60, 20, 12 }
};
int disMid = 66;

int flash[2][4] = {
	{ 215, 85, 14, 12 },
	{ 233, 87, 11, 9 }
};
int flashMid = 91;




cNaveEspacial::cNaveEspacial(cSistema* sis) : cSprite(sis) {
	_sis->cargaTextura(TEX_NAVE, "img\\r-typesheet1.png");

	_x=100;
	_y=100;

	_vidas = NAVE_VIDAS_INICIO;

	reset();
}

cNaveEspacial::~cNaveEspacial() {

}




void cNaveEspacial::arriba() {
	if (_state == NAVE_VIVE) {
		
		// cambiar coordenadas
		_y -= NAVE_PIXELS_MUEVE;
		
		if (_seq == NAVE_SUBE) {
			if (_delay) {
				--_delay;
			} else {
				_seq = NAVE_SUBE_MAS;
			}
		} else if (_seq == NAVE_LADO) {
			_seq = NAVE_SUBE;
			_delay = NAVE_MUEVE_DELAY;
		} else if (_seq <= NAVE_BAJA) {
			_seq = NAVE_LADO;
		}
	}
}

void cNaveEspacial::abajo() {
	if (_state == NAVE_VIVE) {

		// cambiar coordenadas
		_y += NAVE_PIXELS_MUEVE;

		if (_seq == NAVE_BAJA) {
			if (_delay) {
				--_delay;
			} else {
				_seq = NAVE_BAJA_MAS;
			}
		} else if (_seq == NAVE_LADO) {
			_seq = NAVE_BAJA;
			_delay = NAVE_MUEVE_DELAY;
		} else if (_seq >= NAVE_SUBE) {
			_seq = NAVE_LADO;
		}
	}
}

void cNaveEspacial::quieto() {
	if (_state == NAVE_VIVE) {
		_seq = NAVE_LADO;
	}
}

void cNaveEspacial::adelante() {
	_x += NAVE_PIXELS_MUEVE;
}

void cNaveEspacial::atras() {
	_x -= NAVE_PIXELS_MUEVE;
}

void cNaveEspacial::dispara() {
	if (_tiro_pulsado) {
		++_carga_tiro;
	} else {
		_tiro_pulsado = true;
		_carga_tiro = 0;
	}
}

void cNaveEspacial::no_dispara() {
	// disparar cuando el jugador suelta la tecla
	if (_tiro_pulsado) {
		_tiro_pulsado = false;
		// no permitir disparar muy rapido
		int intervalo = _tiempo_vida - _ultimo_tiro;
		if (intervalo < NAVE_TIRO_DELAY) return;
		_ultimo_tiro = _tiempo_vida;

		// el tiempo de carga determina lo tocho que saldra el proyectil
		int xTiro = _x + (mov[_seq][2]>>1);
		int yTiro = _y;
		int tamano = min(_carga_tiro / NAVE_TIRO_DELAY - 2, 5);
		cDisparoNave* tiro = new cDisparoNave(_sis, xTiro, yTiro, max(tamano, 0));
		// meter el nuevo disparo en el nivel
		((cNivel*)_sis->getNivel())->pushDisparo(tiro);
	}
	_carga_tiro = 0;
}

void cNaveEspacial::tira_magia() {

}


void cNaveEspacial::reset() {
	_tiempo_vida = 0;
	_ultimo_tiro = -NAVE_TIRO_DELAY;
	_vida = NAVE_VIDA_INICIAL;
	_magias = NAVE_MAGIAS_INICIAL;
	_tiro_pulsado = false;

	_state = NAVE_VIVE;
	_seq = NAVE_LADO;
	_delay = 0;

}

void cNaveEspacial::procesaTeclas(unsigned char *keys) {
	if (_state == NAVE_VIVE) {
		// movimiento
		bool mueveArriba = keys['w']||keys['W'];
		bool mueveAbajo = keys['s']||keys['S'];
		if (mueveArriba) arriba();
		if (mueveAbajo) abajo();
		if (keys['a']||keys['A']) atras();
		if (keys['d']||keys['D']) adelante();
		if (!mueveArriba && !mueveAbajo) quieto();
		// disparo
		if (keys[' ']) dispara();
		else no_dispara();
		// magias
		if (keys['m']||keys['M']) tira_magia();
	}
}


void cNaveEspacial::muerete() {
	--_vidas;
	_state = NAVE_EXPLO;
	_seq = 0;
	_delay = NAVE_EXPLO_DELAY;
}

void cNaveEspacial::getCaja(cRect &rect) const {
	rect.w = mov[_seq][2] - 4;
	rect.h = mov[_seq][3] - 4;
	rect.x = _x - (rect.w>>1);
	rect.y = _y - (rect.h>>1);
}

void cNaveEspacial::logica() {
	if (_state == NAVE_VIVE) {
		// subir el tiempo que lleva con vida
		++_tiempo_vida;
		// corregir la posicion de la nave para que no se salga de la pantalla
		const cNivel* nivel = (const cNivel*)_sis->getNivel();
		int xPixNave, yPixNave, yPixOffset, wPixNave, hPixNave;
		wPixNave = mov[_seq][2];
		hPixNave = mov[_seq][3];
		xPixNave = _x - (wPixNave>>1);
		yPixOffset = movMid - mov[_seq][1];
		yPixNave = GAME_HEIGHT - 32 - (_y - yPixOffset + hPixNave);

		int posNivel = nivel->getPosicion();
		int left = xPixNave - posNivel;
		if (left < 0) _x -= left;

		int right = (posNivel + GAME_WIDTH) - (xPixNave + wPixNave);
		if (right < 0) _x += right;

		int top = GAME_HEIGHT - 32 - (yPixNave + hPixNave);
		if (top < 0) _y -= top;

		int bottom = yPixNave;
		if (bottom < 0) _y += bottom;

		// ha chocado con el escenario?
		cRect rect;
		getCaja(rect);
		int colMask;
		nivel->colision(rect, colMask);
		if (colMask) muerete();

		// ha chocado con algun disparo malo?
		list<cDisparo*> disparos = nivel->getDisparos();
		for (list<cDisparo*>::iterator it=disparos.begin(); it!=disparos.end(); ++it) {
			cDisparo* disparo = *it;
			if (disparo->malo()) {
				disparo->colision(rect, colMask);
				if (colMask) {
					// hace pupa!!!
					// Y si hacemos que la nave muera de un tiro?
					int dano = disparo->dano();
					_vida -= dano;
					// el disparo muere
					disparo->muerete();
				}
			}
		}

		// ha chocado con algun enemigo?
		list<cEnemigo*> enemigos = nivel->getEnemigos();
		for (list<cEnemigo*>::iterator it = enemigos.begin(); it != enemigos.end(); ++it) {
			cEnemigo* enemigo = *it;
			enemigo->colision(rect, colMask);
			if (colMask) {
				// la nave muere
				_vida = 0;
				// y el enemigo tambien
				enemigo->muerete();
			}
		}

		if (_vida <= 0) {
			muerete();
		}

	} else if (_state == NAVE_EXPLO) {
		if (_delay) {
			--_delay;
		} else {
			++_seq;
			_delay = NAVE_EXPLO_DELAY;
		}
		if (_seq == NAVE_EXPLO_FRAMES) {
			if (_vidas) reset();
			else {
				_state = NAVE_MUERE;
				_muerto = true;
			}
		}
	}
}


void cNaveEspacial::pinta() const {
	if (_state == NAVE_MUERE) return;

	int tex = _sis->getIdTextura(TEX_NAVE);
	int wTex, hTex;
	_sis->getTamanoTextura(TEX_NAVE, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	
	float xTexNave, yTexNave, wTexNave, hTexNave;
	int xPixNave, yPixNave, yPixOffset, wPixNave, hPixNave;

	if (_state == NAVE_VIVE) {
		xTexNave = mov[_seq][0]/(float)wTex;
		yTexNave = mov[_seq][1]/(float)hTex;
		wTexNave = mov[_seq][2]/(float)wTex;
		hTexNave = mov[_seq][3]/(float)hTex;
		wPixNave = mov[_seq][2];
		hPixNave = mov[_seq][3];
		xPixNave = _x - (wPixNave>>1);
		yPixOffset = movMid - mov[_seq][1];
		yPixNave = GAME_HEIGHT - (_y - yPixOffset + hPixNave);
	} else if (_state == NAVE_EXPLO) {
		xTexNave = die[_seq][0]/(float)wTex;
		yTexNave = die[_seq][1]/(float)hTex;
		wTexNave = die[_seq][2]/(float)wTex;
		hTexNave = die[_seq][3]/(float)hTex;
		wPixNave = die[_seq][2];
		hPixNave = die[_seq][3];
		xPixNave = _x - (wPixNave>>1);
		yPixOffset = dieMid - die[_seq][1];
		yPixNave = GAME_HEIGHT - (_y - yPixOffset + hPixNave);
	}

	glTexCoord2f(xTexNave, yTexNave + hTexNave);			glVertex2i(xPixNave, yPixNave);
	glTexCoord2f(xTexNave + wTexNave, yTexNave + hTexNave);	glVertex2i(xPixNave + wPixNave, yPixNave);
	glTexCoord2f(xTexNave + wTexNave, yTexNave);			glVertex2i(xPixNave + wPixNave, yPixNave + hPixNave);
	glTexCoord2f(xTexNave, yTexNave);						glVertex2i(xPixNave, yPixNave + hPixNave);

	if (_state == NAVE_VIVE) {
		if (_carga_tiro >= NAVE_TIRO_FACTOR * NAVE_TIRO_DELAY) {
			// pintar la carga del disparo
			float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
			int xPixTiro, yPixTiro, yPixOffsetTiro, wPixTiro, hPixTiro;
		
			int xTiro = _x + (wPixNave>>1);
			int yTiro = _y;
			int seqTiro = (_carga_tiro / NAVE_TIRO_DELAY) % 8;
			xTexTiro = dis[seqTiro][0]/(float)wTex;
			yTexTiro = dis[seqTiro][1]/(float)hTex;
			wTexTiro = dis[seqTiro][2]/(float)wTex;
			hTexTiro = dis[seqTiro][3]/(float)hTex;
			wPixTiro = dis[seqTiro][2];
			hPixTiro = dis[seqTiro][3];
			xPixTiro = xTiro;
			yPixOffsetTiro = disMid - dis[seqTiro][1];
			yPixTiro = GAME_HEIGHT - (yTiro - yPixOffsetTiro + hPixTiro);

			glTexCoord2f(xTexTiro, yTexTiro + hTexTiro);			glVertex2i(xPixTiro, yPixTiro);
			glTexCoord2f(xTexTiro + wTexTiro, yTexTiro + hTexTiro);	glVertex2i(xPixTiro + wPixTiro, yPixTiro);
			glTexCoord2f(xTexTiro + wTexTiro, yTexTiro);			glVertex2i(xPixTiro + wPixTiro, yPixTiro + hPixTiro);
			glTexCoord2f(xTexTiro, yTexTiro);						glVertex2i(xPixTiro, yPixTiro + hPixTiro);
		}

		int intervalo = _tiempo_vida - _ultimo_tiro;
		if (intervalo < 2 * NAVE_FLASH_DELAY) {
			// pintar el destello del proyectil cuando sale
			float xTexFlash, yTexFlash, wTexFlash, hTexFlash;
			int xPixFlash, yPixFlash, yPixOffsetFlash, wPixFlash, hPixFlash;
		
			int xFlash = _x + (wPixNave>>1);
			int yFlash = _y;
			int seqFlash = intervalo / NAVE_FLASH_DELAY;
			xTexFlash = flash[seqFlash][0]/(float)wTex;
			yTexFlash = flash[seqFlash][1]/(float)hTex;
			wTexFlash = flash[seqFlash][2]/(float)wTex;
			hTexFlash = flash[seqFlash][3]/(float)hTex;
			wPixFlash = flash[seqFlash][2];
			hPixFlash = flash[seqFlash][3];
			xPixFlash = xFlash;
			yPixOffsetFlash = flashMid - flash[seqFlash][1];
			yPixFlash = GAME_HEIGHT - (yFlash - yPixOffsetFlash + hPixFlash);

			glTexCoord2f(xTexFlash, yTexFlash + hTexFlash);				glVertex2i(xPixFlash, yPixFlash);
			glTexCoord2f(xTexFlash + wTexFlash, yTexFlash + hTexFlash);	glVertex2i(xPixFlash + wPixFlash, yPixFlash);
			glTexCoord2f(xTexFlash + wTexFlash, yTexFlash);				glVertex2i(xPixFlash + wPixFlash, yPixFlash + hPixFlash);
			glTexCoord2f(xTexFlash, yTexFlash);							glVertex2i(xPixFlash, yPixFlash + hPixFlash);
		}
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}