
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
	_sis->cargaTextura(TEX_NAVE1, "img\\r-typesheet1.png");

	_x=100;
	_y=100;

	_vidas = NAVE_VIDAS_INICIO;
	_puntos = 0;

	reset();

}

cNaveEspacial::~cNaveEspacial() {

}


void cNaveEspacial::sumaPuntos(long long puntos) {
	_puntos+=puntos;
}
long long cNaveEspacial::puntos() const {
	return _puntos;
}
int cNaveEspacial::vidas() const {
	return _vidas;
}
int cNaveEspacial::cargaDisparo() const {
	return _cargaTiro;
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
	if (_tiroPulsado) {
		++_cargaTiro;
	} else {
		_tiroPulsado = true;
		_cargaTiro = 0;
	}
}

void cNaveEspacial::no_dispara() {
	// disparar cuando el jugador suelta la tecla
	if (_tiroPulsado) {
		_tiroPulsado = false;

		// los escudos tambien le meten duro!
		for (unsigned int i=0; i<_escudos.size(); ++i) _escudos[i]->dispara();

		// no permitir disparar muy rapido
		long long intervalo = _tiempoVida - _ultimoTiro;
		if (intervalo < _tiroDelay) return;
		_ultimoTiro = _tiempoVida;

		// el tiempo de carga determina lo tocho que saldra el proyectil
		int xTiro = _x + (mov[_seq][2]>>1);
		int yTiro = _y;

		// meter el nuevo disparo en el nivel
		int tamano = max(0, _cargaTiro/NAVE_TIRO_DELAY);
		tamano = min(NAVE_MAX_CARGA_TIRO, tamano/NAVE_FACTOR_CARGA_TIRO);
		cDisparoNave* tiro = new cDisparoNave(_sis, xTiro, yTiro, _tipoTiro, tamano);
		((cNivel*)_sis->nivel())->pushDisparo(tiro);
	}
	_cargaTiro = 0;
}

void cNaveEspacial::tira_magia() {

}

void cNaveEspacial::lanzaEscudo() {
	if (_nivelEscudos >= NAVE_ESCUDO1) {
		_escudos[0]->lanza();
	}
}


void cNaveEspacial::reset() {
	_tiempoVida = 0;
	_ultimoTiro = -NAVE_TIRO_DELAY;
	_vida = NAVE_VIDA_INICIAL;
	_magias = NAVE_MAGIAS_INICIAL;
	_tiroPulsado = false;

	_state = NAVE_VIVE;
	_seq = NAVE_LADO;
	_delay = 0;

	_nivelEscudos = NAVE_SIN_ESCUDO;
	for (unsigned int i=0; i<_escudos.size(); ++i) _escudos[i]->muerete();
	_escudos.clear();

	_tipoTiro = DISPARO_NAVE_NORMAL;
	_tiroDelay = NAVE_TIRO_DELAY;

	if (_sis->nivel() != NULL) {
		int x, y;
		((cNivel*)_sis->nivel())->posicionRespawn(x, y);
		renace(x, y);
	}

	_lastX = _x;
	_lastY = _y;
}

void cNaveEspacial::renace(int x, int y) {
	_tiempoVida = 0;
	_x = x;
	_y = y;
}

void cNaveEspacial::anadeEscudo() {
	if (_nivelEscudos == NAVE_SIN_ESCUDO) {
		cNivel* nivel = (cNivel*)_sis->nivel();
		cRect rectNivel;
		nivel->caja(rectNivel);
		int xEscudo = rectNivel.x-16;
		int yEscudo = _y;
		cEscudo* escudo = new cEscudo(_sis, xEscudo, yEscudo, ESCUDO_FRENTE);
		nivel->pushEscudo(escudo);
		_nivelEscudos = NAVE_ESCUDO1;
		_escudos.push_back(escudo);
	} else if (_nivelEscudos == NAVE_ESCUDO1) {
		int xEscudo = _x - NAVE_ESCUDO_SEC_X_OFFSET;
		int yEscudo = _y - NAVE_ESCUDO_SEC_Y_OFFSET;
		cEscudo* escudo = new cEscudo(_sis, xEscudo, yEscudo, ESCUDO_ARRIBA);
		((cNivel*)_sis->nivel())->pushEscudo(escudo);
		_nivelEscudos = NAVE_ESCUDO2;
		_escudos.push_back(escudo);
		
	} else if (_nivelEscudos == NAVE_ESCUDO2) {
		int xEscudo = _x - NAVE_ESCUDO_SEC_X_OFFSET;
		int yEscudo = _y + NAVE_ESCUDO_SEC_Y_OFFSET;
		cEscudo* escudo = new cEscudo(_sis, xEscudo, yEscudo, ESCUDO_ABAJO);
		((cNivel*)_sis->nivel())->pushEscudo(escudo);
		_nivelEscudos = NAVE_ESCUDO3;
		_escudos.push_back(escudo);
	}
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
		// lanzar el escudo
		if (keys['n']||keys['N']) lanzaEscudo();
		// magias
		if (keys['m']||keys['M']) tira_magia();

		// debug
		if (keys['o']||keys['O']) {
			anadeEscudo();
			anadeEscudo();
			anadeEscudo();
		}
	
	}
}


void cNaveEspacial::muerete() {
	--_vidas;
	_state = NAVE_EXPLO;
	_seq = 0;
	_delay = NAVE_EXPLO_DELAY;

	for (unsigned int i=0; i<_escudos.size(); ++i) _escudos[i]->muerete();
	_escudos.clear();
}

void cNaveEspacial::caja(cRect &rect) const {
	rect.w = mov[_seq][2] - 4;
	rect.h = mov[_seq][3] - 4;
	rect.x = _x - (rect.w>>1);
	rect.y = _y - (rect.h>>1);
}

void cNaveEspacial::logica() {
	if (_state == NAVE_VIVE) {
		// subir el tiempo que lleva con vida
		++_tiempoVida;

		// sacar la hit box
		cRect rect;
		caja(rect);

		// buscar colisiones con el escenario
		cNivel* nivel = (cNivel*)_sis->nivel();
		int colisionMask, x, y, objeto;
		nivel->colisionNivel(rect, colisionMask, x, y, objeto);

		// tratamiento especial para las colisiones cuando la nave sea invencible
		bool invencible = _tiempoVida<NAVE_TIEMPO_INVENCIBLE;
		if (invencible) {
			if (colisionMask && objeto&COLISION_TILE) {
				// si es invencible y ha chocado con un tile, corregirle la posicion
				if (colisionMask&COLISION_IZQ && colisionMask&COLISION_DER && colisionMask&COLISION_ARRIBA && colisionMask&COLISION_ABAJO) {
					_x = _lastX;
					_y = _lastY;
				} else {
					if (colisionMask&COLISION_ARRIBA && !(colisionMask&COLISION_ABAJO)) _y += y - rect.y;
					else if (colisionMask&COLISION_ABAJO && !(colisionMask&COLISION_ARRIBA)) _y += y - (rect.y+rect.h);
					if (colisionMask&COLISION_IZQ && !(colisionMask&COLISION_DER)) _x += x - rect.x;
					else if (colisionMask&COLISION_DER && !(colisionMask&COLISION_IZQ)) _x += x - (rect.x+rect.w);
				}

				// ok, ahora es posible que se salga de los limites de la pantalla
				// si es as�, empujar la nave para dentro
				caja(rect);
				nivel->colisionNivel(rect, colisionMask, x, y, objeto);
			}
			if (colisionMask && objeto&COLISION_PANTALLA) {
				if (colisionMask & COLISION_ARRIBA) _y += y - rect.y;
				else if (colisionMask & COLISION_ABAJO) _y += y - (rect.y+rect.h);
				if (colisionMask & COLISION_IZQ) _x += x - rect.x;
				else if (colisionMask & COLISION_DER) _x += x - (rect.x+rect.w);
			}
		} else {
			// se sale de la pantalla?
			if (colisionMask && objeto&COLISION_PANTALLA) {
				if (colisionMask & COLISION_ARRIBA) _y += y - rect.y;
				else if (colisionMask & COLISION_ABAJO) _y += y - (rect.y+rect.h);
				if (colisionMask & COLISION_IZQ) _x += x - rect.x;
				else if (colisionMask & COLISION_DER) _x += x - (rect.x+rect.w);
				// puede que al empujar la nave, esta choque contra un tile
				caja(rect);
				nivel->colisionNivel(rect, colisionMask, x, y, objeto);
			}
			// choca contra un tile? que reviente!
			if (colisionMask && objeto&COLISION_TILE) {
				_vida = 0;
			}
		}

		// ha chocado con algun disparo malo?
		list<cDisparo*> disparos = nivel->disparos();
		for (list<cDisparo*>::iterator it=disparos.begin(); it!=disparos.end(); ++it) {
			cDisparo* disparo = *it;
			if (disparo->malo()) {
				int colMask;
				disparo->colision(rect, colMask);
				if (colMask) {
					// hace pupa!!!
					if (!invencible) _vida -= disparo->dano();
					// el disparo muere
					disparo->muerete();
				}
			}
		}

		// ha chocado con algun enemigo?
		list<cEnemigo*> enemigos = nivel->enemigos();
		for (list<cEnemigo*>::iterator it = enemigos.begin(); it != enemigos.end(); ++it) {
			cEnemigo* enemigo = *it;
			int colMask;
			enemigo->colision(rect, colMask);
			if (colMask) {
				// el enemigo tambien
				enemigo->restaVida(_vida);
				if (!invencible) _vida = 0;
				else if (enemigo->jefe()) {
					_x = _lastX;
					_y = _lastY;
				}
			}
		}

		if (_vida <= 0) {
			muerete();
			return;
		}

		// ha pillado algun item?
		list<cItem*> items = nivel->items();
		for (list<cItem*>::iterator it = items.begin(); it != items.end(); ++it) {
			cItem* item = *it;
			cRect myRect = rect;
			myRect.x -= 2;
			myRect.y -= 2;
			myRect.w += 4;
			myRect.h += 4;
			int colMask;
			item->colision(myRect, colMask);
			if (colMask) {
				int tipo = item->tipo();
				if (tipo == ITEM_ESCUDO) anadeEscudo();
				else if (tipo == ITEM_DISPARO_RB) {
					_tipoTiro = DISPARO_NAVE_CIRCULAR;
					_tiroDelay = NAVE_TIRO_CIR_DELAY;
				}
				item->muerete();
			}
		}

		_lastX = _x;
		_lastY = _y;

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

	// el parpadeo indica que la nave es invencible
	bool invencible = _state==NAVE_VIVE && _tiempoVida<NAVE_TIEMPO_INVENCIBLE;
	if (invencible) {
		long long t = _tiempoVida/NAVE_TIEMPO_PARPADEO;
		if (t%2) return;
	}

	int tex = _sis->idTextura(TEX_NAVE1);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_NAVE1, wTex, hTex);

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
		if (_cargaTiro >= NAVE_TIRO_FACTOR * NAVE_TIRO_DELAY) {
			// pintar la carga del disparo
			float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
			int xPixTiro, yPixTiro, yPixOffsetTiro, wPixTiro, hPixTiro;
		
			int xTiro = _x + (wPixNave>>1);
			if (_nivelEscudos >= NAVE_ESCUDO1 && _escudos[0]->anclado()) {
				cRect rectEscudo;
				_escudos[0]->caja(rectEscudo);
				xTiro += rectEscudo.w - 4;
			}
			int yTiro = _y;
			int seqTiro = (_cargaTiro / NAVE_TIRO_DELAY) % 8;
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

		int intervalo = int(_tiempoVida - _ultimoTiro);
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