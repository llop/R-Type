#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>


int jefe4Mov[5][4] = {
	{ 387, 1, 64, 80 },
	{ 320, 1, 64, 80 },
	{ 253, 1, 64, 80 },
	{ 186, 1, 64, 80 },
	{ 119, 1, 64, 80 }
};
int jefe4Escudo[9][4] = {
	{ 511, 83, 55, 48 },
	{ 446, 83, 55, 48 },
	{ 380, 83, 54, 48 },
	{ 315, 83, 55, 48 },
	{ 250, 83, 55, 48 },
	{ 186, 83, 53, 48 },
	{ 126, 83, 47, 48 },
	{ 64, 83, 44, 48 },
	{ 1, 83, 42, 48 }
};
int jefe4EscudoMid[9] = { 522, 458, 392, 326, 261, 196, 131, 66, 1 };
int jefe4CajasAbre[5][2][4] = {
	{
		{ 250-jefe4Escudo[4][0], 83-jefe4Escudo[4][1], 55, 18 },
		{ 250-jefe4Escudo[4][0], 113-jefe4Escudo[4][1], 55, 18 }
	},
	{
		{ 186-jefe4Escudo[5][0], 83-jefe4Escudo[5][1], 53, 14 },
		{ 186-jefe4Escudo[5][0], 116-jefe4Escudo[5][1], 53, 15}
	},
	{
		{ 126-jefe4Escudo[6][0], 83-jefe4Escudo[6][1], 47, 11 },
		{ 126-jefe4Escudo[6][0], 120-jefe4Escudo[6][1], 47, 11 }
	},
	{
		{ 64-jefe4Escudo[7][0], 83-jefe4Escudo[7][1], 44, 7 },
		{ 64-jefe4Escudo[7][0], 124-jefe4Escudo[7][1], 44, 7 }
	}
	// la ultima es completamente abierta
	//, {
	//	{ ... }, { ... }
	//}
};


int jefe4Explo[9][4] = {
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
int jefe4ExploMid[2] = { 65, 131 };

int critterSize[2] = { 128, 80 };
int barreraSize[2] = { 48, 48 };
int bestiaSize[2] = { 64, 80 };
int offsetsCritter[3][2] = {
	{ 0, 16 },		// barrera
	{ 64, 0 },		// critter
	{ 81, 36 }		// boca

};


cJefe4::cJefe4(cSistema* sis) : cEnemigo(sis) {
	_sis->cargaTextura(TEX_JEFE4, "img\\r-typesheet41.png");
	_sis->cargaTextura(TEX_EXPLO2, "img\\r-typesheet44.png");

	_sis->cargaSonido(SOUND_EXPLO1, "wavs\\rtype-083.wav", false, 50, 20);

	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = JEFE4_MUEVE_DELAY;

	_vidaCritter.resize(3);
	_vidaCritter[0] = _sis->dificultad() == DIFICULTAD_NORMAL ?
		JEFE4_VIDA_INICIAL_CRITTER1 : JEFE4_VIDA_INICIAL_CRITTER1_HARD;
	_vidaCritter[1] = _vidaCritter[2] = _sis->dificultad() == DIFICULTAD_NORMAL ?
		JEFE4_VIDA_INICIAL_CRITTER2 : JEFE4_VIDA_INICIAL_CRITTER2_HARD;

	_ultimoAtaque.resize(3);
	_ultimoAtaque[0] = _sis->dificultad() == DIFICULTAD_NORMAL ? 
		-JEFE4_INTERVALO_ATAQUE1 : -JEFE4_INTERVALO_ATAQUE1_HARD;
	_ultimoAtaque[1] = _ultimoAtaque[2] = _sis->dificultad() == DIFICULTAD_NORMAL ?
		-JEFE4_INTERVALO_ATAQUE2 : -JEFE4_INTERVALO_ATAQUE2_HARD;


	_tiempoVida = 0;

	_ultimoImpacto.resize(3, -JEFE4_FLASH_IMPACTO);

	_xCritter.resize(3);
	_xCritter[0] = 90 * TILE_WIDTH;
	_xCritter[1] = 88 * TILE_WIDTH;
	_xCritter[2] = 88 * TILE_WIDTH;

	_yCritter.resize(3);
	_yCritter[0] = 13 * TILE_HEIGHT + (TILE_HEIGHT >> 1);
	_yCritter[1] = 5 * TILE_HEIGHT + (TILE_HEIGHT >> 1);
	_yCritter[2] = 22 * TILE_HEIGHT + (TILE_HEIGHT >> 1);

	_estadoCritter.resize(3, JEFE4_BARRERA_CERRADA);
	_subEstadoCritter.resize(3, JEFE4_BOCA_CERRADA);
	_delayCritter.resize(3, JEFE4_MUEVE_DELAY);
	_delayBarrera.resize(3, JEFE4_MUEVE_DELAY);

	_seqCritter.resize(3, 0);
	_seqExploCritter.resize(3, 0);
	_seqBarrera.resize(3, 0);
	_pulsaBarrera.resize(3, true);

	_cuentaTiros.resize(3, 0);

	_subState = JEFE4_IDLE;

	_esJefe = true;
}
cJefe4::~cJefe4() {

}

void cJefe4::muerete() {
	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_delay = JEFE4_MUERE_DELAY;
	_seqExplo = 0;
	_tiempoMuerto = 0;
}
void cJefe4::restaVida(int vida) {
	if (_state==ENEMIGO_VIVE && _subState!=JEFE4_IDLE) {
		for (int i = 0; i < 3; ++i) {
			if (_estadoCritter[i] == JEFE4_BARRERA_ABIERTA) {
				_vidaCritter[i] -= vida;
				_ultimoImpacto[i] = _tiempoVida;
			}
		}
	}
}
void cJefe4::caja(cRect &rect) const {
	// no hace falta
}
void cJefe4::offset(int x, int y) {
	// el nivel no lo menea
}

void cJefe4::colision(cRect &rect, int &colMask) const {
	colMask = 0;
	if (_state == ENEMIGO_VIVE) {
		for (int i = 0; i < 3; ++i) {
			if (_vidaCritter[i]>0) {
				int left = _xCritter[i] - (critterSize[0] >> 1);
				int top = _yCritter[i] - (critterSize[1] >> 1);
				// si esta vivo, veamos si choca
				// choca con la barrera?
				int seq = _seqBarrera[i];
				if (_estadoCritter[i] == JEFE4_BARRERA_ABIERTA && seq>=4) {
					int seqCaja = seq - 4;
					for (int j = 0; j < 2; ++j) {
						int wPixSolido = jefe4CajasAbre[seqCaja][j][2];
						int hPixSolido = jefe4CajasAbre[seqCaja][j][3];
						int xPixSolido = (left + offsetsCritter[0][0] + 2) - 
							(jefe4EscudoMid[seq] - jefe4Escudo[seq][0]) + 
							jefe4CajasAbre[seqCaja][j][0];
						int yPixSolido = top + offsetsCritter[0][1] + jefe4CajasAbre[seqCaja][j][1];
						if (xPixSolido < rect.x + rect.w && xPixSolido + wPixSolido > rect.x &&
							yPixSolido < rect.y + rect.h && yPixSolido + hPixSolido > rect.y) {
							colMask = 1;
							return;
						}
					}
				} else {
					int wPixSolido = barreraSize[0];
					int hPixSolido = barreraSize[1];
					int xPixSolido = (left + offsetsCritter[0][0] + 2) - (jefe4EscudoMid[seq] - jefe4Escudo[seq][0]);
					int yPixSolido = top + offsetsCritter[0][1];
					if (xPixSolido < rect.x + rect.w && xPixSolido + wPixSolido > rect.x &&
						yPixSolido < rect.y + rect.h && yPixSolido + hPixSolido > rect.y) {
						colMask = 1;
						return;
					}
				}

				// o con la bestia?
				//int wPixSolido = bestiaSize[0];
				//int hPixSolido = bestiaSize[1];
				//int xPixSolido = left + offsetsCritter[1][0];
				//int yPixSolido = top + offsetsCritter[1][1];
				//if (xPixSolido < rect.x + rect.w && xPixSolido + wPixSolido > rect.x &&
				//	yPixSolido < rect.y + rect.h && yPixSolido + hPixSolido > rect.y) {
				//	colMask = 1;
				//	return;
				//}
			}
		}
	}
}

void cJefe4::logica() {
	// avanzar animacion explosiones
	for (list<cExplo>::iterator it = _exploCuerpo.begin(); it != _exploCuerpo.end();) {
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
	
	if (_state == ENEMIGO_VIVE) {
		++_tiempoVida;

		cNivel* nivel = (cNivel*)_sis->nivel();
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();

		// generar malos al azar
		int modulo = _sis->dificultad() == DIFICULTAD_NORMAL ? NIVEL4_FREC_GRIPAU : NIVEL4_FREC_GRIPAU_HARD;
		modulo = int(modulo / nivel->factorDificultad());
		if (!(rand() % modulo)) {
			// generar un gripau al azar
			int x = nivel->getPosicion() + (rand() % GAME_WIDTH);
			int y = rand() % 2 ? 0 : GAME_HEIGHT;
			cEnemigo5* enemigo = new cEnemigo5(_sis, x, y);
			nivel->pushEnemigo(enemigo);
		}

		int numMuertos = 0;
		for (int i = 0; i < 3; ++i) {
			if (_vidaCritter[i] > 0) {
				int left = _xCritter[i] - (critterSize[0] >> 1);
				int top = _yCritter[i] - (critterSize[1] >> 1);

				// bestia
				cRect rect;
				rect.x = left + offsetsCritter[1][0];
				rect.y = top + offsetsCritter[1][1];
				rect.w = bestiaSize[0];
				rect.h = bestiaSize[1];

				list<cDisparo*> disparos = nivel->disparos();
				for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
					cDisparo* disparo = *it;
					if (!disparo->malo() && disparo->vive()) {
						int colMask;
						disparo->colision(rect, colMask);
						if (colMask) {
							disparo->explota();
							// aplicar efecto del disparo
							_vidaCritter[i] -= disparo->dano();
							_ultimoImpacto[i] = _tiempoVida;
						} else {
							cRect rectDisp;
							disparo->caja(rectDisp);
							colision(rectDisp, colMask);
							if (colMask) disparo->explota();
						}
					}
				}

				vector<cEscudo*> escudos = nave->escudos();
				for (unsigned int i = 0; i < escudos.size(); ++i) {
					cEscudo* escudo = escudos[i];
					int colMask;
					escudo->colision(rect, colMask);
					if (colMask) {
						escudo->choca();
						_vidaCritter[i] -= escudo->dano();
						_ultimoImpacto[i] = _tiempoVida;
					} else {
						cRect rectDisp;
						escudo->caja(rectDisp);
						colision(rectDisp, colMask);
						if (colMask) escudo->choca();
					}
				}

				if (_vidaCritter[i] <= 0) {
  					int puntos = i ? JEFE4_PUNTOS_CRITTER2 : JEFE4_PUNTOS_CRITTER1;
					nave->sumaPuntos(puntos);

					_estadoCritter[i] = JEFE4_BARRERA_ABIERTA;
					_subEstadoCritter[i] = JEFE4_BOCA_ABIERTA;

					++numMuertos;

					// explotarlo
					cExplo explo;
					explo.seq = 0;
					explo.delay = JEFE1_EXPLO_DELAY;
					explo.x = _xCritter[i] + 20;
					explo.y = _yCritter[i];
					_exploCuerpo.push_back(explo);
				}

			} else {
				++numMuertos;

				// generar una explosion por ahi
				if (_exploCuerpo.size() < JEFE4_MAX_NUM_EXPLO && !(rand() % 32)) {
					cExplo explo;
					explo.seq = 0;
					explo.delay = JEFE1_EXPLO_DELAY;
					explo.x = (_xCritter[i]-(critterSize[0]>>1)) + (rand()% critterSize[0]);
					explo.y = (_yCritter[i]-(critterSize[1]>>1)) + (rand()% critterSize[1]);
					_exploCuerpo.push_back(explo);

					_sis->playSonido(SOUND_EXPLO1);
				}
			}
		}

		// todos los critters han palmado?
		if (numMuertos == 3) {
			nivel->tiraMagia();
			muerete();
			return;
		}

		// actualizar estado
		if (_tiempoVida == 60) {
			_subState = JEFE4_ATACA;
			for (int i = 0; i < 3; ++i) _ultimoAtaque[i] = _tiempoVida;
		}
		if (_subState == JEFE4_ATACA) {
			for (int i = 0; i < 3; ++i) {
				if (_vidaCritter[i]>0) {
					if (_estadoCritter[i] == JEFE4_BARRERA_ABIERTA) {
						long long intervalo = _tiempoVida - _ultimoAtaque[i];
						long long interCierra = _sis->dificultad() == DIFICULTAD_NORMAL ?
							(i ? JEFE4_INTERVALO_CIERRA2 : JEFE4_INTERVALO_CIERRA1) :
							(i ? JEFE4_INTERVALO_CIERRA2_HARD : JEFE4_INTERVALO_CIERRA1_HARD);
						if (intervalo == interCierra) {
							// cierra la barrera
							_estadoCritter[i] = JEFE4_BARRERA_CERRADA;
							//_seqBarrera[i] = 0;
							_pulsaBarrera[i] = true;
							_delayBarrera[i] = JEFE4_MUEVE_DELAY;
							_ultimoAtaque[i] = _tiempoVida;

							// setea el critter
							_subEstadoCritter[i] = JEFE4_BOCA_CERRADA;
							//_seqCritter[i] = 0;
							_delayCritter[i] = JEFE4_MUEVE_DELAY;
							_cuentaTiros[i] = _sis->dificultad() == DIFICULTAD_NORMAL ?
								(i ? JEFE4_TIROS2 : JEFE4_TIROS1) :
								(i ? JEFE4_TIROS2_HARD : JEFE4_TIROS1_HARD);
						} else {

							if (_subEstadoCritter[i]==JEFE4_BOCA_CERRADA) {
								int modulo = _sis->dificultad() == DIFICULTAD_NORMAL ?
									(i ? JEFE4_INTERVALO_VOMITO2 : JEFE4_INTERVALO_VOMITO1) :
									(i ? JEFE4_INTERVALO_VOMITO2_HARD : JEFE4_INTERVALO_VOMITO1_HARD);
								if (!(intervalo%modulo)) {
									_subEstadoCritter[i] = JEFE4_BOCA_ABIERTA;
									//_seqCritter[i] = 0;
									_delayCritter[i] = JEFE4_MUEVE_DELAY;
									_cuentaTiros[i] = 0;
								}
							} else if (_subEstadoCritter[i]==JEFE4_BOCA_ABIERTA) {
								if (_seqCritter[i] == 4) {
									int tiro = _sis->dificultad() == DIFICULTAD_NORMAL ?
										(i ? JEFE4_TIROS2 : JEFE4_TIROS1) :
										(i ? JEFE4_TIROS2_HARD : JEFE4_TIROS1_HARD);
									if (_cuentaTiros[i]<tiro) {
										if (!(_tiempoVida % 2)) {
											++_cuentaTiros[i];
											int xTiro = _xCritter[i] - (critterSize[0] >> 1) + offsetsCritter[2][0];
											int yTiro = _yCritter[i] - (critterSize[1] >> 1) + offsetsCritter[2][1];
											int xNave, yNave;
											_sis->naveEspacial()->getPosicion(xNave, yNave);
											float xVec = float(xNave - xTiro);
											float yVec = float(yNave - yTiro);
											cDisparoJefe1* tiro = new cDisparoJefe1(_sis, xTiro, yTiro, xVec, yVec);
											nivel->pushDisparo(tiro);
										}
									} else {
										_subEstadoCritter[i] = JEFE4_BOCA_CERRADA;
										_delayCritter[i] = JEFE4_MUEVE_DELAY;
									}
								}
							}
						}
					} else if (_estadoCritter[i] == JEFE4_BARRERA_CERRADA) {
						long long intervalo = _tiempoVida - _ultimoAtaque[i];
						long long interAtaque = _sis->dificultad() == DIFICULTAD_NORMAL ?
							(i ? JEFE4_INTERVALO_ATAQUE2 : JEFE4_INTERVALO_ATAQUE1) :
							(i ? JEFE4_INTERVALO_ATAQUE2_HARD : JEFE4_INTERVALO_ATAQUE1_HARD);
						if (intervalo == interAtaque) {
							// abre la barrera
							_estadoCritter[i] = JEFE4_BARRERA_ABIERTA;
							_seqBarrera[i] = 4;
							_delayBarrera[i] = JEFE4_MUEVE_DELAY;
							_ultimoAtaque[i] = _tiempoVida;
						}
					}
				}
			}
		}
	} else if (_state == ENEMIGO_EXPLO) {

		++_tiempoMuerto;
		if (_tiempoMuerto == JEFE4_TIEMPO_MUERTO) ((cNivel*)_sis->nivel())->termina();
		
		// genera una explosion random
		if (_exploCuerpo.size() < JEFE4_MAX_NUM_EXPLO && !(rand()%4)) {
			cExplo explo;
			explo.seq = 0;
			explo.delay = JEFE1_EXPLO_DELAY;
			explo.x = (_xCritter[0]-(critterSize[0]>>1)-32) + (rand()%(critterSize[0]+64));
			explo.y = 32 + (rand()%(GAME_HEIGHT-64));
			_exploCuerpo.push_back(explo);

			_sis->playSonido(SOUND_EXPLO1);
		}

	}

	// animacion
	for (int i = 0; i < 3; ++i) {
		if (_vidaCritter[i]>0) {
			// barrera
			if (_estadoCritter[i] == JEFE4_BARRERA_ABIERTA) {
				if (_delayBarrera[i]) --_delayBarrera[i];
				else {
					_seqBarrera[i] = min(8, _seqBarrera[i] + 1);
					_delayBarrera[i] = JEFE4_MUEVE_DELAY;
				}
			}
			else if (_estadoCritter[i] == JEFE4_BARRERA_CERRADA) {
				if (_delayBarrera[i]) --_delayBarrera[i];
				else {
					if (_pulsaBarrera[i]) {
						++_seqBarrera[i];
						if (_seqBarrera[i] >= 4) {
							_seqBarrera[i] -= 2;
							_pulsaBarrera[i] = false;
						}
					}
					else {
						--_seqBarrera[i];
						if (_seqBarrera[i]<0) {
							_seqBarrera[i] = 0;
							_pulsaBarrera[i] = true;
						}
					}
					_delayBarrera[i] = JEFE4_MUEVE_DELAY;
				}
			}

			// critter
			if (_subEstadoCritter[i] == JEFE4_BOCA_CERRADA) {
				if (_delayCritter[i]) --_delayCritter[i];
				else {
					_seqCritter[i] = max(0, _seqCritter[i] - 1);
					_delayCritter[i] = JEFE2_MUEVE_DELAY;
				}
			}
			else if (_subEstadoCritter[i] == JEFE4_BOCA_ABIERTA) {
				if (_delayCritter[i]) --_delayCritter[i];
				else {
					_seqCritter[i] = min(4, _seqCritter[i] + 1);
					_delayCritter[i] = JEFE2_MUEVE_DELAY;
				}
			}

		} else {
			// muerto: abre todo
			if (_delayBarrera[i]) --_delayBarrera[i];
			else {
				if (_seqBarrera[i] < 4) {
					--_seqBarrera[i];
					if (_seqBarrera[i] < 0) {
						_seqBarrera[i] = 4;
						_delayBarrera[i] = JEFE4_MUEVE_DELAY;
					}
				}
				else {
					_seqBarrera[i] = min(8, _seqBarrera[i] + 1);
					_delayBarrera[i] = JEFE4_MUEVE_DELAY;
				}
			}
			// critter
			if (_delayCritter[i]) --_delayCritter[i];
			else {
				_seqCritter[i] = min(4, _seqCritter[i] + 1);
				_delayCritter[i] = JEFE4_MUEVE_DELAY;
			}
		}
	}
}

void cJefe4::pintaVivo() const {
	// textura jefe
	int tex = _sis->idTextura(TEX_JEFE4);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_JEFE4, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	for (int i = 0; i < 3; ++i) {
		int left = _xCritter[i] - (critterSize[0] >> 1);
		int top = _yCritter[i] - (critterSize[1] >> 1);
		int seq = _seqBarrera[i];

		int wPixEne = jefe4Escudo[seq][2];
		int hPixEne = jefe4Escudo[seq][3];
		int xPixEne = (left + offsetsCritter[0][0] + 2) - (jefe4EscudoMid[seq] - jefe4Escudo[seq][0]);
		int yPixEne = top + offsetsCritter[0][1];
		yPixEne = GAME_HEIGHT - (yPixEne + hPixEne);

		// barrera
		float xTexEne = jefe4Escudo[seq][0] / (float)wTex;
		float yTexEne = jefe4Escudo[seq][1] / (float)hTex;
		float wTexEne = wPixEne / (float)wTex;
		float hTexEne = hPixEne / (float)hTex;

		glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
		glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
		glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
		glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);

		
		// la bestia
		seq = _seqCritter[i];

		wPixEne = jefe4Mov[seq][2];
		hPixEne = jefe4Mov[seq][3];
		xPixEne = left + offsetsCritter[1][0];
		yPixEne = top + offsetsCritter[1][1];
		yPixEne = GAME_HEIGHT - (yPixEne + hPixEne);

		xTexEne = jefe4Mov[seq][0] / (float)wTex;
		yTexEne = jefe4Mov[seq][1] / (float)hTex;
		wTexEne = wPixEne / (float)wTex;
		hTexEne = hPixEne / (float)hTex;

		long long tiempoFlash = _tiempoVida - _ultimoImpacto[i];
		bool hazFlash = tiempoFlash < JEFE1_FLASH_IMPACTO && _state == ENEMIGO_VIVE;
		if (hazFlash) {
			float r = tiempoFlash % 3 == 0;
			float g = tiempoFlash % 3 == 1;
			float b = tiempoFlash % 3 == 2;
			glColor3f(r, g, b);
		}
		glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
		glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
		glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
		glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);
		if (hazFlash) glColor3f(1, 1, 1);
	}	

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void cJefe4::pintaExplo() const {

	// explosiones
	int texExplo = _sis->idTextura(TEX_EXPLO2);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_EXPLO2, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExplo);
	glBegin(GL_QUADS);

	for (list<cExplo>::const_iterator it = _exploCuerpo.begin(); it != _exploCuerpo.end(); ++it) {
		const cExplo &explo = *it;
		int wPixCola = jefe4Explo[explo.seq][2];
		int hPixCola = jefe4Explo[explo.seq][3];
		float xTexCola = jefe4Explo[explo.seq][0] / (float)wTex;
		float yTexCola = jefe4Explo[explo.seq][1] / (float)hTex;
		float wTexCola = wPixCola / (float)wTex;
		float hTexCola = hPixCola / (float)hTex;
		int xPixCola = int(explo.x - (wPixCola >> 1));
		int yPixOffset = jefe4ExploMid[explo.seq / 5] - jefe4Explo[explo.seq][1];
		int yPixCola = int(GAME_HEIGHT - (explo.y - yPixOffset + hPixCola));

		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);

	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void cJefe4::pinta() const {
	pintaVivo();
	pintaExplo();
}