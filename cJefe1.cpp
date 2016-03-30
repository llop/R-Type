#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>



int jefe1Mov[24][4] = {
	{ 27, 217, 155, 204 },
	{ 189, 217, 155, 204 },
	{ 351, 217, 155, 204 },
	{ 27, 430, 155, 204 },
	{ 189, 430, 155, 204 },
	{ 351, 430, 155, 204 },
	{ 27, 644, 155, 204 },
	{ 189, 644, 155, 204 },
	{ 351, 644, 155, 204 },
	{ 27, 861, 155, 204 },
	{ 189, 861, 155, 204 },
	{ 351, 861, 155, 204 },
	{ 27, 1073, 155, 204 },
	{ 189, 1073, 155, 204 },
	{ 351, 1073, 155, 204 },
	{ 27, 1285, 155, 204 },
	{ 189, 1285, 155, 204 },
	{ 351, 1285, 155, 204 },
	{ 27, 1498, 155, 204 },
	{ 189, 1498, 155, 204 },
	{ 351, 1498, 155, 204 },
	{ 27, 1711, 155, 204 },
	{ 189, 1711, 155, 204 },
	{ 351, 1711, 155, 204 }
};
int jefe1Cola[4][4] = {
	{ 652, 2000, 16, 16 },
	{ 654, 2017, 14, 13 },
	{ 656, 2032, 12, 11 },
	{ 616, 2045, 16, 16 }
};

int jefe1Solido[31][4] = {
	{ 61, 0, 12, 4 },
	{ 52, 4, 32, 4 },
	{ 48, 8, 38, 4 },
	{ 42, 12, 49, 4 },
	{ 31, 16, 66, 4 },
	{ 25, 20, 78, 4 },
	{ 21, 24, 86, 4 },
	{ 18, 28, 92, 4 },
	{ 15, 32, 96, 4 },
	{ 12, 36, 99, 4 },
	{ 8, 40, 100, 4 },
	{ 4, 44, 102, 4 },
	{ 1, 48, 97, 8 },
	{ 2, 56, 54, 13 },
	{ 29, 69, 25, 16 },
	{ 65, 56, 48, 4 },
	{ 69, 60, 45, 4 },
	{ 74, 64, 50, 8 },
	{ 80, 72, 56, 8 },
	{ 83, 80, 64, 8 },
	{ 83, 88, 67, 14 },
	{ 79, 102, 74, 24 },
	{ 89, 126, 59, 20 },
	{ 92, 146, 52, 10 },
	{ 95, 156, 42, 8 },
	{ 97, 164, 36, 4 },
	{ 95, 168, 36, 4 },
	{ 91, 172, 32, 8 },
	{ 70, 180, 48, 8 },
	{ 61, 188, 53, 8 },
	{ 74, 196, 26, 5 }
};

int jefe1HitBox[4] = { 72, 94, 32, 32 };

int jefe1ColaExplo[6][4] = {
	{ 130, 2, 30, 28 },
	{ 162, 1, 31, 32 },
	{ 194, 1, 32, 32 },
	{ 228, 2, 31, 30 },
	{ 261, 2, 31, 30 },
	{ 295, 1, 31, 31 }
};
int jefe1ColaExploMid = 16;

int jefe1Explo[9][4] = {
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
int jefe1ExploMid[2] = { 65, 131 };

cJefe1::cJefe1(cSistema* sis) : cEnemigo(sis) {
	_sis->cargaTextura(TEX_JEFE1, "img\\r-typesheet30.png");
	_sis->cargaTextura(TEX_EXPLO2, "img\\r-typesheet44.png");
	
	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = JEFE1_MUEVE_DELAY;
	
	_vida = JEFE1_VIDA_INICIAL;
	_puntos = JEFE1_PUNTOS;

	_tiempoVida = 0;
	_ultimoImpacto = -JEFE1_FLASH_IMPACTO;

	_x = 5084;
	_y = 96;

	_subState = JEFE1_ALIEN_DENTRO;

	_anguloCrece = true;
	_anguloCola = acos(-1.0f);

	_sube = false;

	_esJefe = true;
}

cJefe1::~cJefe1() {
	
}

void cJefe1::muerete() {
	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_seq = 21;
	_delay = JEFE1_MUERE_DELAY;
	_bolaExplo = 0;
	_seqExplo = 0;
	_delayExploCola = JEFE1_COLA_EXPLO_DELAY;
	_tiempoMuerto = 0;
}

void cJefe1::offset(int x, int y) {
	// el nivel no lo mueve
}

void cJefe1::caja(cRect &rect) const {
	// estes no es necesario para un jefe
}

void cJefe1::restaVida(int vida) {
	// impactos fisicos de la nave no le quitan vida
};

void cJefe1::colision(cRect &rect, int &colMask) const {
	colMask=0;
	if (_state == ENEMIGO_VIVE) {
		
		// colisiona con la cola?
		float rad = 0;
		float xCola = float(_x + 63);
		float yCola = float(_y + 187);
		for (int i=0; i<8; ++i) {
			if (i) rad += jefe1Cola[0][2]*0.75f;
			float x = xCola + rad*cos(_anguloCola);
			float y = yCola + rad*sin(_anguloCola);

			int wPixCola = jefe1Cola[0][2];
			int hPixCola = jefe1Cola[0][3]; 
			int xPixCola = int(x - (wPixCola>>1));
			int yPixCola = int(y - (hPixCola>>1));

			if (xPixCola < rect.x+rect.w && xPixCola+wPixCola > rect.x &&
				yPixCola < rect.y+rect.h && yPixCola+hPixCola > rect.y) {
					colMask = 1;
					return;
			}
		}
		// 6 medianas
		for (int i=0; i<6; ++i) {
			rad += jefe1Cola[1][2]*0.75f;
			float x = xCola + rad*cos(_anguloCola);
			float y = yCola + rad*sin(_anguloCola);

			int wPixCola = jefe1Cola[1][2];
			int hPixCola = jefe1Cola[1][3]; 
			int xPixCola = int(x - (wPixCola>>1));
			int yPixCola = int(y - (hPixCola>>1) + hPixCola);

			if (xPixCola < rect.x+rect.w && xPixCola+wPixCola > rect.x &&
				yPixCola < rect.y+rect.h && yPixCola+hPixCola > rect.y) {
					colMask = 1;
					return;
			}
		}
		// 6 pequeñas
		for (int i=0; i<6; ++i) {
			rad += jefe1Cola[2][2]*0.75f;
			float x = xCola + rad*cos(_anguloCola);
			float y = yCola + rad*sin(_anguloCola);

			int wPixCola = jefe1Cola[2][2];
			int hPixCola = jefe1Cola[2][3]; 
			int xPixCola = int(x - (wPixCola>>1));
			int yPixCola = int(y - (hPixCola>>1));

			if (xPixCola < rect.x+rect.w && xPixCola+wPixCola > rect.x &&
				yPixCola < rect.y+rect.h && yPixCola+hPixCola > rect.y) {
					colMask = 1;
					return;
			}
		}
		// la punta
		rad += jefe1Cola[3][2]*0.75f;
		float x = xCola + rad*cos(_anguloCola);
		float y = yCola + rad*sin(_anguloCola);

		int wPixCola = jefe1Cola[3][2];
		int hPixCola = jefe1Cola[3][3]; 
		int xPixCola = int(x - (wPixCola>>1));
		int yPixCola = int(y - (hPixCola>>1));
		
		if (xPixCola < rect.x+rect.w && xPixCola+wPixCola > rect.x &&
			yPixCola < rect.y+rect.h && yPixCola+hPixCola > rect.y) {
				colMask = 1;
				return;
		}

		// el cuerpo
		for (int i=0; i<31; ++i) {

			int wPixCola = jefe1Solido[i][2];
			int hPixCola = jefe1Solido[i][3]; 
			int xPixCola = _x + jefe1Solido[i][0];
			int yPixCola = _y + jefe1Solido[i][1];

			if (xPixCola < rect.x+rect.w && xPixCola+wPixCola > rect.x &&
				yPixCola < rect.y+rect.h && yPixCola+hPixCola > rect.y) {
					colMask = 1;
					return;
			}
		}
	}
}

void cJefe1::logica() {
	if (_state == ENEMIGO_VIVE) {

		++_tiempoVida;

		// mover el jefe
		if (_sube) _y -= JEFE1_INC_MOV;
		else _y += JEFE1_INC_MOV;
		if (_y < TILE_HEIGHT*5) {
			_sube = false;
			_y += 2*JEFE1_INC_MOV;
		} else if (_y + jefe1Mov[0][3] > GAME_HEIGHT-HUD_HEIGHT-4*TILE_HEIGHT) {
			_sube = true;
			_y -= 2*JEFE1_INC_MOV;
		}

		
		cNivel* nivel = (cNivel*)_sis->nivel();
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();


		// chequear impactos
		cRect rect;
		rect.x = _x + jefe1HitBox[0];
		rect.y = _y + jefe1HitBox[1];
		rect.w = jefe1HitBox[2];
		rect.h = jefe1HitBox[3];
		list<cDisparo*> disparos = nivel->disparos();
		for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
			cDisparo* disparo = *it;
			if (!disparo->malo() && disparo->vive()) {
				int colMask;
  				disparo->colision(rect, colMask);
				if (colMask && _subState>=JEFE1_ALIEN_SALIENDO) {
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

		list<cEscudo*> escudos = nivel->escudos();
		for (list<cEscudo*>::iterator it = escudos.begin(); it != escudos.end(); ++it) {
			cEscudo* escudo = *it;
			int colMask;
			escudo->colision(rect, colMask);
			if (colMask && _subState>=JEFE1_ALIEN_SALIENDO) {
				escudo->choca();
				long long tiempoFlash = _tiempoVida - _ultimoImpacto;
				if (tiempoFlash >= JEFE1_FLASH_IMPACTO) {
					_vida -= escudo->dano();
					_ultimoImpacto = _tiempoVida;
				}
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
		if (_tiempoVida == 816) {
			_subState = JEFE1_ALIEN_SALIENDO;
			_seq = 3;
		} else if (_subState==JEFE1_ALIEN_SALIENDO && _seq>=JEFE1_NUM_FRAMES-3) {
			_subState = JEFE1_ALIEN_FUERA;
			_seq = 15;
			_delay = JEFE1_MUEVE_DELAY;
		} else if (_subState == JEFE1_ALIEN_FUERA) {
			long long intervalo = _tiempoVida - _ultimoTiro;
			if (intervalo >= JEFE1_TIRO_DELAY) {
				_ultimoTiro = _tiempoVida;
				_subState = JEFE1_ALIEN_VOMITA;
				_seq = 18;
				_delay = JEFE1_MUEVE_DELAY;

				// vomita!
				int xTiro = _x + 81;
				int yTiro = _y + 114;
				int xNave, yNave;
				_sis->naveEspacial()->getPosicion(xNave, yNave);
				float xVec = float(xNave - xTiro);
				float yVec = float(yNave - yTiro);
				cDisparoJefe1* tiro = new cDisparoJefe1(_sis, xTiro, yTiro, xVec, yVec);
				nivel->pushDisparo(tiro);
			}
		} else if (_subState == JEFE1_ALIEN_VOMITA) {
			long long intervalo = _tiempoVida - _ultimoTiro;
			if (intervalo >= 90) {
				_ultimoTiro = _tiempoVida;
				_subState = JEFE1_ALIEN_FUERA;
				_seq = 15;
				_delay = JEFE1_MUEVE_DELAY;
			} else {
				if (!(intervalo%4)) {
					// vomita!
					int xTiro = _x + 81;
					int yTiro = _y + 114;
					int xNave, yNave;
					_sis->naveEspacial()->getPosicion(xNave, yNave);
					float xVec = float(xNave - xTiro);
					float yVec = float(yNave - yTiro);
					cDisparoJefe1* tiro = new cDisparoJefe1(_sis, xTiro, yTiro, xVec, yVec);
					nivel->pushDisparo(tiro);
				}
			}
		}

		// mover la cola
		if (_anguloCrece) _anguloCola += JEFE1_INC_ANG_COLA;
		else _anguloCola -= JEFE1_INC_ANG_COLA;

		if (_anguloCola > 1.3*acos(-1.0f)) {
			_anguloCrece = false;
			_anguloCola -= 2*JEFE1_INC_ANG_COLA;
		} else if (_anguloCola < acos(-1.0f)) {
			_anguloCrece = true;
			_anguloCola += 2*JEFE1_INC_ANG_COLA;
		}

		// animacion
		if (_subState == JEFE1_ALIEN_DENTRO) {
			if (_delay) --_delay;
			else {
				_seq = (_seq + 1) % 3;
				_delay = JEFE1_MUEVE_DELAY;
			}
		} else if (_subState == JEFE1_ALIEN_SALIENDO) {
			if (_delay) --_delay;
			else {
				_seq = _seq + 3;
				if (!_seq) _seq = 3;
				_delay = JEFE1_MUEVE_DELAY;
			}
		} else if (_subState == JEFE1_ALIEN_FUERA) {
			if (_delay) --_delay;
			else {
				++_seq;
				if (_seq == 18) _seq = 15;
				_delay = JEFE1_MUEVE_DELAY;
			}
		} else if (_subState == JEFE1_ALIEN_VOMITA) {
			if (_delay) --_delay;
			else {
				_seq = (_seq + 3) % JEFE1_NUM_FRAMES;
				if (!_seq) _seq = 18;
				_delay = JEFE1_MUEVE_DELAY;
			}
		}
	} else if (_state == ENEMIGO_EXPLO) {

		++_tiempoMuerto;
		if (_tiempoMuerto == JEFE2_TIEMPO_MUERTO) {
			((cNivel*)_sis->nivel())->termina();
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
			int boxIndex = rand() % 31;
			explo.x = _x + jefe1Solido[boxIndex][0] + (rand() % jefe1Solido[boxIndex][2]);
			explo.y = _y + jefe1Solido[boxIndex][1] + (rand() % jefe1Solido[boxIndex][3]);
			_exploCuerpo.push_back(explo);
		}

		// animacion explosion cola
  		if (_delayExploCola) --_delayExploCola;
		else {
			_seqExplo = (_seqExplo + 1) % JEFE1_COLA_EXPLO_NUM_FRAMES;
   			if (!_seqExplo) ++_bolaExplo;
			_delayExploCola = JEFE1_COLA_EXPLO_DELAY;
		}
	}
}


void cJefe1::pintaVivo() const {
	// textura jefe
	int tex = _sis->idTextura(TEX_JEFE1);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_JEFE1, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	// para este, sus coordenadas (_x, _y) corresponden con su esquina superior izquierda
	float xTexEne = jefe1Mov[_seq][0] / (float)wTex;
	float yTexEne = jefe1Mov[_seq][1] / (float)hTex;
	float wTexEne = jefe1Mov[_seq][2] / (float)wTex;
	float hTexEne = jefe1Mov[_seq][3] / (float)hTex;
	int wPixEne = jefe1Mov[_seq][2];
	int hPixEne = jefe1Mov[_seq][3]; 
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
	
	// vamos a pintar la cola
	//X=R*Cos(theta)
	//Y=R*Sin(theta) 
	// 8 bols grandes
	float rad = 0;
	float xCola = float(_x + 63);
	float yCola = float(_y + 187);
	for (int i=0; i<8; ++i) {
		if (i) rad += jefe1Cola[0][2]*0.75f;
		float x = xCola + rad*cos(_anguloCola);
		float y = yCola + rad*sin(_anguloCola);

		float xTexCola = jefe1Cola[0][0] / (float)wTex;
		float yTexCola = jefe1Cola[0][1] / (float)hTex;
		float wTexCola = jefe1Cola[0][2] / (float)wTex;
		float hTexCola = jefe1Cola[0][3] / (float)hTex;
		int wPixCola = jefe1Cola[0][2];
		int hPixCola = jefe1Cola[0][3]; 
		int xPixCola = int(x - (wPixCola>>1));
		int yPixCola = int(GAME_HEIGHT - (y - (hPixCola>>1) + hPixCola));

		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);
	}
	// 6 medianas
	for (int i=0; i<6; ++i) {
		rad += jefe1Cola[1][2]*0.75f;
		float x = xCola + rad*cos(_anguloCola);
		float y = yCola + rad*sin(_anguloCola);

		float xTexCola = jefe1Cola[1][0] / (float)wTex;
		float yTexCola = jefe1Cola[1][1] / (float)hTex;
		float wTexCola = jefe1Cola[1][2] / (float)wTex;
		float hTexCola = jefe1Cola[1][3] / (float)hTex;
		int wPixCola = jefe1Cola[1][2];
		int hPixCola = jefe1Cola[1][3]; 
		int xPixCola = int(x - (wPixCola>>1));
		int yPixCola = int(GAME_HEIGHT - (y - (hPixCola>>1) + hPixCola));

		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);
	}
	// 6 pequeñas
	for (int i=0; i<6; ++i) {
		rad += jefe1Cola[2][2]*0.75f;
		float x = xCola + rad*cos(_anguloCola);
		float y = yCola + rad*sin(_anguloCola);

		float xTexCola = jefe1Cola[2][0] / (float)wTex;
		float yTexCola = jefe1Cola[2][1] / (float)hTex;
		float wTexCola = jefe1Cola[2][2] / (float)wTex;
		float hTexCola = jefe1Cola[2][3] / (float)hTex;
		int wPixCola = jefe1Cola[2][2];
		int hPixCola = jefe1Cola[2][3]; 
		int xPixCola = int(x - (wPixCola>>1));
		int yPixCola = int(GAME_HEIGHT - (y - (hPixCola>>1) + hPixCola));

		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);
	}
	// la punta
	rad += jefe1Cola[3][2]*0.75f;
	float x = xCola + rad*cos(_anguloCola);
	float y = yCola + rad*sin(_anguloCola);

	float xTexCola = jefe1Cola[3][0] / (float)wTex;
	float yTexCola = jefe1Cola[3][1] / (float)hTex;
	float wTexCola = jefe1Cola[3][2] / (float)wTex;
	float hTexCola = jefe1Cola[3][3] / (float)hTex;
	int wPixCola = jefe1Cola[3][2];
	int hPixCola = jefe1Cola[3][3]; 
	int xPixCola = int(x - (wPixCola>>1));
	int yPixCola = int(GAME_HEIGHT - (y - (hPixCola>>1) + hPixCola));

	glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
	glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
	glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
	glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);

	if (tiempoFlash < JEFE1_FLASH_IMPACTO) glColor3f(1, 1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cJefe1::pintaExplo() const {
	// textura jefe
	int tex = _sis->idTextura(TEX_JEFE1);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_JEFE1, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	// para este, sus coordenadas (_x, _y) corresponden con su esquina superior izquierda
	float xTexEne = jefe1Mov[_seq][0] / (float)wTex;
	float yTexEne = jefe1Mov[_seq][1] / (float)hTex;
	float wTexEne = jefe1Mov[_seq][2] / (float)wTex;
	float hTexEne = jefe1Mov[_seq][3] / (float)hTex;
	int wPixEne = jefe1Mov[_seq][2];
	int hPixEne = jefe1Mov[_seq][3]; 
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
		float xTexCola = jefe1Explo[explo.seq][0] / (float)wTex;
		float yTexCola = jefe1Explo[explo.seq][1] / (float)hTex;
		float wTexCola = jefe1Explo[explo.seq][2] / (float)wTex;
		float hTexCola = jefe1Explo[explo.seq][3] / (float)hTex;
		int wPixCola = jefe1Explo[explo.seq][2];
		int hPixCola = jefe1Explo[explo.seq][3]; 
		int xPixCola = int(explo.x - (wPixCola>>1));
		int yPixOffset =  jefe1ExploMid[explo.seq / 5] - jefe1Explo[explo.seq][1];
		int yPixCola = int(GAME_HEIGHT - (explo.y - yPixOffset + hPixCola));
				
		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);

	}

	bool bolaExploPintada = false;
	int accBolaExplo = 0;


	// 8 bols grandes
	float rad = 0;
	float xCola = float(_x + 63);
	float yCola = float(_y + 187);
	for (int i=0; i<8; ++i, ++accBolaExplo) {
		if (i) rad += jefe1Cola[0][2]*0.75f;

		if (accBolaExplo < _bolaExplo) continue;

		float x = xCola + rad*cos(_anguloCola);
		float y = yCola + rad*sin(_anguloCola);

		float xTexCola, yTexCola, wTexCola, hTexCola;
		int wPixCola, hPixCola, xPixCola, yPixCola;
		if (bolaExploPintada) {
			xTexCola = jefe1Cola[0][0] / (float)wTex;
			yTexCola = jefe1Cola[0][1] / (float)hTex;
			wTexCola = jefe1Cola[0][2] / (float)wTex;
			hTexCola = jefe1Cola[0][3] / (float)hTex;
			wPixCola = jefe1Cola[0][2];
			hPixCola = jefe1Cola[0][3]; 
			xPixCola = int(x - (wPixCola>>1));
			yPixCola = int(GAME_HEIGHT - (y - (hPixCola>>1) + hPixCola));
		} else {
			xTexCola = jefe1ColaExplo[_seqExplo][0] / (float)wTex;
			yTexCola = jefe1ColaExplo[_seqExplo][1] / (float)hTex;
			wTexCola = jefe1ColaExplo[_seqExplo][2] / (float)wTex;
			hTexCola = jefe1ColaExplo[_seqExplo][3] / (float)hTex;
			wPixCola = jefe1ColaExplo[_seqExplo][2];
			hPixCola = jefe1ColaExplo[_seqExplo][3]; 
			xPixCola = int(x - (wPixCola>>1));
			int yPixOffset =  jefe1ColaExploMid - jefe1ColaExplo[_seqExplo][1];
			yPixCola = int(GAME_HEIGHT - (y - yPixOffset + hPixCola));
		}
		
		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);

		if (accBolaExplo == _bolaExplo) {
			bolaExploPintada = true;
			glEnd();
			glDisable(GL_TEXTURE_2D);
			_sis->tamanoTextura(TEX_JEFE1, wTex, hTex);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex);
			glBegin(GL_QUADS);
		}
	}
	// 6 medianas
	for (int i=0; i<6; ++i, ++accBolaExplo) {
		rad += jefe1Cola[1][2]*0.75f;

		if (accBolaExplo < _bolaExplo) continue;

		float x = xCola + rad*cos(_anguloCola);
		float y = yCola + rad*sin(_anguloCola);

		float xTexCola, yTexCola, wTexCola, hTexCola;
		int wPixCola, hPixCola, xPixCola, yPixCola;
		if (bolaExploPintada) {
			xTexCola = jefe1Cola[1][0] / float(wTex);
			yTexCola = jefe1Cola[1][1] / float(hTex);
			wTexCola = jefe1Cola[1][2] / float(wTex);
			hTexCola = jefe1Cola[1][3] / float(hTex);
			wPixCola = jefe1Cola[1][2];
			hPixCola = jefe1Cola[1][3]; 
			xPixCola = int(x - (wPixCola>>1));
			yPixCola = int(GAME_HEIGHT - (y - (hPixCola>>1) + hPixCola));
		} else {
			xTexCola = jefe1ColaExplo[_seqExplo][0] / (float)wTex;
			yTexCola = jefe1ColaExplo[_seqExplo][1] / (float)hTex;
			wTexCola = jefe1ColaExplo[_seqExplo][2] / (float)wTex;
			hTexCola = jefe1ColaExplo[_seqExplo][3] / (float)hTex;
			wPixCola = jefe1ColaExplo[_seqExplo][2];
			hPixCola = jefe1ColaExplo[_seqExplo][3]; 
			xPixCola = int(x - (wPixCola>>1));
			int yPixOffset =  jefe1ColaExploMid - jefe1ColaExplo[_seqExplo][1];
			yPixCola = int(GAME_HEIGHT - (y - yPixOffset + hPixCola));
		}

		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);

		if (accBolaExplo == _bolaExplo) {
			bolaExploPintada = true;
			glEnd();
			glDisable(GL_TEXTURE_2D);
			_sis->tamanoTextura(TEX_JEFE1, wTex, hTex);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex);
			glBegin(GL_QUADS);
		}
	}
	// 6 pequeñas
	for (int i=0; i<6; ++i, ++accBolaExplo) {
		rad += jefe1Cola[2][2]*0.75f;

		if (accBolaExplo < _bolaExplo) continue;

		float x = xCola + rad*cos(_anguloCola);
		float y = yCola + rad*sin(_anguloCola);

		float xTexCola, yTexCola, wTexCola, hTexCola;
		int wPixCola, hPixCola, xPixCola, yPixCola;
		if (bolaExploPintada) {
			xTexCola = jefe1Cola[2][0] / float(wTex);
			yTexCola = jefe1Cola[2][1] / float(hTex);
			wTexCola = jefe1Cola[2][2] / float(wTex);
			hTexCola = jefe1Cola[2][3] / float(hTex);
			wPixCola = jefe1Cola[2][2];
			hPixCola = jefe1Cola[2][3]; 
			xPixCola = int(x - (wPixCola>>1));
			yPixCola = int(GAME_HEIGHT - (y - (hPixCola>>1) + hPixCola));
		} else {
			xTexCola = jefe1ColaExplo[_seqExplo][0] / (float)wTex;
			yTexCola = jefe1ColaExplo[_seqExplo][1] / (float)hTex;
			wTexCola = jefe1ColaExplo[_seqExplo][2] / (float)wTex;
			hTexCola = jefe1ColaExplo[_seqExplo][3] / (float)hTex;
			wPixCola = jefe1ColaExplo[_seqExplo][2];
			hPixCola = jefe1ColaExplo[_seqExplo][3]; 
			xPixCola = int(x - (wPixCola>>1));
			int yPixOffset =  jefe1ColaExploMid - jefe1ColaExplo[_seqExplo][1];
			yPixCola = int(GAME_HEIGHT - (y - yPixOffset + hPixCola));
		}

		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);
		
		if (accBolaExplo == _bolaExplo) {
			bolaExploPintada = true;
			glEnd();
			glDisable(GL_TEXTURE_2D);
			_sis->tamanoTextura(TEX_JEFE1, wTex, hTex);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex);
			glBegin(GL_QUADS);
		}
	}
	
	if (_bolaExplo > accBolaExplo) {
		glEnd();
		glDisable(GL_TEXTURE_2D);
		return;
	}

	// la punta
	rad += jefe1Cola[3][2]*0.75f;
	float x = xCola + rad*cos(_anguloCola);
	float y = yCola + rad*sin(_anguloCola);

	float xTexCola, yTexCola, wTexCola, hTexCola;
	int wPixCola, hPixCola, xPixCola, yPixCola;
	if (bolaExploPintada) {
		xTexCola = jefe1Cola[3][0] / (float)wTex;
		yTexCola = jefe1Cola[3][1] / (float)hTex;
		wTexCola = jefe1Cola[3][2] / (float)wTex;
		hTexCola = jefe1Cola[3][3] / (float)hTex;
		wPixCola = jefe1Cola[3][2];
		hPixCola = jefe1Cola[3][3]; 
		xPixCola = int(x - (wPixCola>>1));
		yPixCola = int(GAME_HEIGHT - (y - (hPixCola>>1) + hPixCola));
	} else {
		xTexCola = jefe1ColaExplo[_seqExplo][0] / (float)wTex;
		yTexCola = jefe1ColaExplo[_seqExplo][1] / (float)hTex;
		wTexCola = jefe1ColaExplo[_seqExplo][2] / (float)wTex;
		hTexCola = jefe1ColaExplo[_seqExplo][3] / (float)hTex;
		wPixCola = jefe1ColaExplo[_seqExplo][2];
		hPixCola = jefe1ColaExplo[_seqExplo][3]; 
		xPixCola = int(x - (wPixCola>>1));
		int yPixOffset =  jefe1ColaExploMid - jefe1ColaExplo[_seqExplo][1];
		yPixCola = int(GAME_HEIGHT - (y - yPixOffset + hPixCola));
	}

	glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
	glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
	glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
	glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cJefe1::pinta() const{
	if (_state == ENEMIGO_MUERE) return;
	if (_state == ENEMIGO_VIVE) pintaVivo();
	else if (_state == ENEMIGO_EXPLO) pintaExplo();
}
