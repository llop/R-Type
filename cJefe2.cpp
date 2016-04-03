#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>



int jefe2Mov[8][4] = {
	{ 262, 2, 256, 140 },
	{ 2, 2, 256, 140 },
	{ 262, 145, 256, 140 },
	{ 2, 145, 256, 140 },
	{ 262, 288, 256, 140 },
	{ 2, 288, 256, 140 },
	{ 262, 431, 256, 140 },
	{ 2, 431, 256, 140 }
};

int jefe2Explo[9][4] = {
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
int jefe2ExploMid[2] = { 65, 131 };

int jefe2Solido[33][4] = {
	{ 57, 3, 26, 4 },
	{ 49, 7, 34, 4 },
	{ 46, 11, 39, 4 },
	{ 48, 15, 40, 4 },
	{ 50, 19, 40, 6 },
	{ 157, 26, 47, 4 },
	{ 162, 22, 40, 4 },
	{ 165, 17, 41, 5 },
	{ 169, 12, 40, 5 },
	{ 171, 8, 32, 4 },
	{ 172, 4, 20, 4 },
	{ 174, 1, 10, 3 },
	{ 55, 38, 146, 6 },
	{ 56, 44, 146, 28 },
	{ 56, 72, 143, 8 },
	{ 60, 80, 136, 8 },
	{ 58, 88, 144, 5 },
	{ 52, 93, 157, 4 },
	{ 3, 97, 253, 4 },
	{ 217, 93, 37, 4 },
	{ 220, 89, 29, 4 },
	{ 224, 84, 19, 5 },
	{ 1, 101, 250, 8 },
	{ 5, 109, 242, 6 },
	{ 13, 115, 230, 6 },
	{ 17, 121, 221, 6 },
	{ 23, 127, 213, 4 },
	{ 25, 131, 206, 9 },
	{ 51, 25, 48, 5 },
	{ 52, 30, 150, 8 },
	{ 18, 80, 11, 3 },
	{ 15, 89, 17, 4 },
	{ 8, 93, 28, 4 }
};

int jefe2HitBox[8][4] = {
	{ 371-jefe2Mov[0][0], 22-jefe2Mov[0][1], 40, 10 },
	{ 120-jefe2Mov[1][0], 23-jefe2Mov[1][1], 20, 9 },
	{ 376-jefe2Mov[2][0], 165-jefe2Mov[2][1], 30, 10 },
	{ 115-jefe2Mov[3][0], 163-jefe2Mov[3][1], 30, 12 },
	{ 375-jefe2Mov[4][0], 305-jefe2Mov[4][1], 28, 13 },
	{ 116-jefe2Mov[5][0], 305-jefe2Mov[5][1], 30, 13 },
	{ 375-jefe2Mov[6][0], 447-jefe2Mov[6][1], 32, 14 },
	{ 114-jefe2Mov[7][0], 445-jefe2Mov[7][1], 32, 16 }
};


cJefe2::cJefe2(cSistema* sis) : cEnemigo(sis) {
	_sis->cargaTextura(TEX_JEFE2, "img\\r-typesheet32.png");
	_sis->cargaTextura(TEX_EXPLO2, "img\\r-typesheet44.png");
	
	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = JEFE2_MUEVE_DELAY;
	
	_vida = _sis->dificultad()==DIFICULTAD_NORMAL ? JEFE2_VIDA_INICIAL : JEFE2_VIDA_INICIAL_HARD;
	_puntos = JEFE2_PUNTOS;

	_tiempoVida = 0;
	_ultimoImpacto = -JEFE2_FLASH_IMPACTO;

	// 4608 - 640 + 320 - 128 = 4160
	_x = 4160;
	_y = GAME_HEIGHT-HUD_HPIX-jefe2Mov[0][3];

	_subState = JEFE2_IDLE;
	
	_esJefe = true;

	_gusanos.resize(2, NULL);
	_ultimaSalidaGusano.resize(2, -1000);
	_tiempoGusano.resize(2, 0);
}

cJefe2::~cJefe2() {
	
}



void cJefe2::muerete() {
	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_delay = JEFE2_MUEVE_DELAY;
	_seqExplo = 0;
	_tiempoMuerto = 0;

	if (_gusanos[0] != NULL) _gusanos[0]->explota();
	if (_gusanos[1] != NULL) _gusanos[1]->explota();
}

void cJefe2::offset(int x, int y) {
	// el nivel no lo mueve
}

void cJefe2::caja(cRect &rect) const {
	// estes no es necesario para un jefe
}


void cJefe2::colision(cRect &rect, int &colMask) const {
	colMask=0;
	if (_state == ENEMIGO_VIVE) {
		// parte solida
		for (int i=0; i<33; ++i) {
			int wPixSolido = jefe2Solido[i][2];
			int hPixSolido = jefe2Solido[i][3]; 
			int xPixSolido = _x + jefe2Solido[i][0];
			int yPixSolido = _y + jefe2Solido[i][1];
			if (xPixSolido < rect.x+rect.w && xPixSolido+wPixSolido > rect.x &&
				yPixSolido < rect.y+rect.h && yPixSolido+hPixSolido > rect.y) {
					colMask = 1;
					return;
			}
		}
		// la hit box
		int wPixSolido = jefe2HitBox[_seq][2];
		int hPixSolido = jefe2HitBox[_seq][3]; 
		int xPixSolido = _x + jefe2HitBox[_seq][0];
		int yPixSolido = _y + jefe2HitBox[_seq][1];
		if (xPixSolido < rect.x+rect.w && xPixSolido+wPixSolido > rect.x &&
			yPixSolido < rect.y+rect.h && yPixSolido+hPixSolido > rect.y) {
				colMask = 1;
				return;
		}
	}
}


void cJefe2::restaVida(int vida) {
	if (_subState == JEFE2_VULVA_ABIERTA) {
		// cerrar la vulva y hacer dano
		_subState = JEFE2_VULVA_CERRADA;
		_delay = JEFE2_MUEVE_DELAY;
		cEnemigo::restaVida(vida);
		_ultimoImpacto = _tiempoVida;
	}
}

void cJefe2::logica() {
	if (_state == ENEMIGO_VIVE) {

		++_tiempoVida;

		cNivel* nivel = (cNivel*)_sis->nivel();
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();

		// chequear impactos
		cRect rect;
		rect.x = _x + jefe2HitBox[_seq][0];
		rect.y = _y + jefe2HitBox[_seq][1];
		rect.w = jefe2HitBox[_seq][2];
		rect.h = jefe2HitBox[_seq][3];
		list<cDisparo*> disparos = nivel->disparos();
		for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
			cDisparo* disparo = *it;
			if (!disparo->malo() && disparo->vive()) {
				int colMask;
  				disparo->colision(rect, colMask);
				if (colMask && _subState==JEFE2_VULVA_ABIERTA) {
					// cerrar la vulva
					_subState = JEFE2_VULVA_CERRADA;
					_delay = JEFE2_MUEVE_DELAY;

					disparo->explota();
					// aplicar efecto del disparo
					_vida -= disparo->dano();
					_ultimoImpacto = _tiempoVida;
				} else {
					cRect rectDisp;
					disparo->caja(rectDisp);
  					colision(rectDisp, colMask);
					if (colMask) {
						disparo->explota();
					}
				}
			}
		}

		vector<cEscudo*> escudos = nave->escudos();
		for (unsigned int i = 0; i < escudos.size(); ++i) {
			cEscudo* escudo = escudos[i];
			int colMask;
			escudo->colision(rect, colMask);
			if (colMask && _subState==JEFE2_VULVA_ABIERTA) {
				// cerrar la vulva
				_subState = JEFE2_VULVA_CERRADA;
				_delay = JEFE2_MUEVE_DELAY;

				escudo->choca();
				_vida -= escudo->dano();
				_ultimoImpacto = _tiempoVida;
			} else {
				cRect rectDisp;
				escudo->caja(rectDisp);
  				colision(rectDisp, colMask);
				if (colMask) escudo->choca();
			}
		}

		if (_vida <= 0) {
			nave->sumaPuntos(_puntos);
			muerete();
			return;
		}

		// actualizar estado
		if (_tiempoVida == 640) {
			_subState = JEFE2_VULVA_CERRADA;
			_seq = 0;
			_ultimaSalida = _tiempoVida;
		} else if (_subState==JEFE2_VULVA_CERRADA) {
			long long intervalo = _tiempoVida - _ultimaSalida;
			if (intervalo==JEFE2_INTERVALO_ENTRADA) {
				_subState=JEFE2_VULVA_ABIERTA;
				_seq = 1;
				_delay = JEFE2_MUEVE_DELAY;
				_ultimaSalida = _tiempoVida;
			}
		} else if (_subState==JEFE2_VULVA_ABIERTA) {
			long long intervalo = _tiempoVida - _ultimaSalida;
			if (intervalo==JEFE2_INTERVALO_SALIDA) {
				_subState=JEFE2_VULVA_CERRADA;
				_delay = JEFE2_MUEVE_DELAY;
				_ultimaSalida = _tiempoVida;
			}
		}

		// tirar o matar los gusanos
		if (_subState > JEFE2_IDLE) {
			if (_gusanos[0] == NULL) {
				long long intervaloGusano1 = _tiempoVida - _ultimaSalidaGusano[0];
				if (intervaloGusano1 >= JEFE2_INTERVALO_GUSANO1) {
					_ultimaSalidaGusano[0] = _tiempoVida;
					_tiempoGusano[0] = JEFE2_TIEMPO_GUSANO1;
					cEnemigo3* enemigo3 = new cEnemigo3(_sis, 4288, _y+4, 6, 62.0f, 0.2f, 0.03333f, false);
					_gusanos[0] = enemigo3;
					nivel->pushEnemigo(enemigo3);
				}
			} else {
				if (_tiempoGusano[0]) --_tiempoGusano[0];
				else if (_gusanos[0] != NULL) {
					_gusanos[0]->muerete();
					_gusanos[0] = NULL;
				}
			}
			// gusano 2
			if (_gusanos[1] == NULL) {
				long long intervaloGusano2 = _tiempoVida - _ultimaSalidaGusano[1];
				if (intervaloGusano2 >= JEFE2_INTERVALO_GUSANO2) {
					_ultimaSalidaGusano[1] = _tiempoVida;
					_tiempoGusano[1] = JEFE2_TIEMPO_GUSANO2;
					cEnemigo3* enemigo3 = new cEnemigo3(_sis, 4288-2, _y+2, 8, 144.0f, 2.3f, 0.03333f, true);
					_gusanos[1] = enemigo3;
					nivel->pushEnemigo(enemigo3);
				}
			} else {
				if (_tiempoGusano[1]) --_tiempoGusano[1];
				else if (_gusanos[1] != NULL) {
					_gusanos[1]->muerete();
					_gusanos[1] = NULL;
				}
			}
		}

		// animacion
		if (_subState <= JEFE2_VULVA_CERRADA) {
			if (_delay) --_delay;
			else {
				_seq = max(0, _seq-1);
				_delay = JEFE2_MUEVE_DELAY;
			}
		} else if (_subState == JEFE2_VULVA_ABIERTA) {
			if (_delay) --_delay;
			else {
				_seq = min(7, _seq+1);
				_delay = JEFE2_MUEVE_DELAY;
			}
		}
	} else if (_state == ENEMIGO_EXPLO) {

		++_tiempoMuerto;
		if (_tiempoMuerto == JEFE2_TIEMPO_MUERTO) {
			((cNivel*)_sis->nivel())->termina();
		}

		if (_delay) --_delay;
		else {
			_seq = min(7, _seq+1);
			_delay = JEFE2_MUEVE_DELAY;
		}
		// animacion explosiones cuerpo
		for (list<cExplo>::iterator it=_exploCuerpo.begin(); it!=_exploCuerpo.end();) {
			cExplo &explo = *it;
			bool borra = false;
			if (explo.delay) --explo.delay;
			else {
  				++explo.seq;
				explo.delay = JEFE1_EXPLO_DELAY;
				borra = explo.seq == JEFE1_EXPLO_NUM_FRAMES;
			}
			if (borra) it = _exploCuerpo.erase(it);
			else ++it;
		}
		// genera una explosion random
		if (_exploCuerpo.size() < JEFE1_MAX_NUM_EXPLO && !(rand() % 4)) {
			cExplo explo;
  			explo.seq = 0;
			explo.delay = JEFE1_EXPLO_DELAY;
			int boxIndex = rand() % 34;
			if (boxIndex<33) {
				explo.x = _x + jefe2Solido[boxIndex][0] + (rand() % jefe2Solido[boxIndex][2]);
				explo.y = _y + jefe2Solido[boxIndex][1] + (rand() % jefe2Solido[boxIndex][3]);
			} else {
				explo.x = _x + jefe2HitBox[_seq][0] + (rand() % jefe2HitBox[_seq][2]);
				explo.y = _y + jefe2HitBox[_seq][1] + (rand() % jefe2HitBox[_seq][3]);
			}
			_exploCuerpo.push_back(explo);
		}

	}
}


void cJefe2::pintaVivo() const {
	// textura jefe
	int tex = _sis->idTextura(TEX_JEFE2);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_JEFE2, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	// para este, sus coordenadas (_x, _y) corresponden con su esquina superior izquierda
	float xTexEne = jefe2Mov[_seq][0] / (float)wTex;
	float yTexEne = jefe2Mov[_seq][1] / (float)hTex;
	float wTexEne = jefe2Mov[_seq][2] / (float)wTex;
	float hTexEne = jefe2Mov[_seq][3] / (float)hTex;
	int wPixEne = jefe2Mov[_seq][2];
	int hPixEne = jefe2Mov[_seq][3]; 
	int xPixEne = _x;
	int yPixEne = GAME_HEIGHT - (_y + hPixEne);

	long long tiempoFlash = _tiempoVida - _ultimoImpacto;
	if (tiempoFlash < JEFE1_FLASH_IMPACTO) {
		float r = tiempoFlash%3 == 0;
		float g = tiempoFlash%3 == 1;
		float b = tiempoFlash%3 == 2;
		glColor3f(r, g, b);
	}
	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);
	if (tiempoFlash < JEFE1_FLASH_IMPACTO) glColor3f(1, 1, 1);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cJefe2::pintaExplo() const {
	// textura jefe
	int tex = _sis->idTextura(TEX_JEFE2);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_JEFE2, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	// para este, sus coordenadas (_x, _y) corresponden con su esquina superior izquierda
	float xTexEne = jefe2Mov[_seq][0] / (float)wTex;
	float yTexEne = jefe2Mov[_seq][1] / (float)hTex;
	float wTexEne = jefe2Mov[_seq][2] / (float)wTex;
	float hTexEne = jefe2Mov[_seq][3] / (float)hTex;
	int wPixEne = jefe2Mov[_seq][2];
	int hPixEne = jefe2Mov[_seq][3]; 
	int xPixEne = _x;
	int yPixEne = GAME_HEIGHT - (_y + hPixEne);

	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	// explosiones
	int texExplo = _sis->idTextura(TEX_EXPLO2);
	_sis->tamanoTextura(TEX_EXPLO2, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExplo);
	glBegin(GL_QUADS);

	for (list<cExplo>::const_iterator it=_exploCuerpo.begin(); it!=_exploCuerpo.end(); ++it) {
		const cExplo &explo = *it;
		float xTexCola = jefe2Explo[explo.seq][0] / (float)wTex;
		float yTexCola = jefe2Explo[explo.seq][1] / (float)hTex;
		float wTexCola = jefe2Explo[explo.seq][2] / (float)wTex;
		float hTexCola = jefe2Explo[explo.seq][3] / (float)hTex;
		int wPixCola = jefe2Explo[explo.seq][2];
		int hPixCola = jefe2Explo[explo.seq][3]; 
		int xPixCola = int(explo.x - (wPixCola>>1));
		int yPixOffset =  jefe2ExploMid[explo.seq / 5] - jefe2Explo[explo.seq][1];
		int yPixCola = int(GAME_HEIGHT - (explo.y - yPixOffset + hPixCola));
				
		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);

	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cJefe2::pinta() const{
	if (_state == ENEMIGO_MUERE) return;
	if (_state == ENEMIGO_VIVE) pintaVivo();
	else if (_state == ENEMIGO_EXPLO) pintaExplo();
}



