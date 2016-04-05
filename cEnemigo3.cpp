#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>


int ene3Cap[16][4] = {
	{ 273, 20, 32, 28 },
	{ 239, 37, 32, 28 },
	{ 205, 37, 32, 29 },
	{ 171, 37, 32, 28 },
	{ 137, 36, 32, 30 },
	{ 103, 37, 32, 28 },
	{ 67, 37, 32, 29 },
	{ 35, 37, 32, 28 },
	{ 1, 20, 32, 28 },
	{ 35, 3, 32, 28 },
	{ 69, 2, 32, 29 },
	{ 103, 3, 32, 28 },
	{ 137, 2, 32, 30 },
	{ 171, 3, 32, 28 },
	{ 205, 2, 32, 29 },
	{ 239, 3, 32, 28 }
};

int ene3Coll[16][4] = {
	{ 274, 88, 30, 28 },
	{ 239, 102, 30, 29 },
	{ 205, 102, 32, 28 },
	{ 171, 103, 31, 27 },
	{ 137, 104, 32, 26 },
	{ 104, 103, 31, 27 },
	{ 69, 102, 32, 28 },
	{ 37, 102, 30, 29 },
	{ 2, 88, 30, 28 },
	{ 37, 71, 30, 29 },
	{ 69, 72, 32, 28 },
	{ 104, 72, 31, 27 },
	{ 137, 72, 32, 26 },
	{ 171, 72, 31, 27 },
	{ 205, 72, 32, 28 },
	{ 239, 71, 30, 29 }
};

int ene3Cos[2][4] = {
	{ 137, 137, 32, 28 },	// sano
	{ 137, 170, 32, 29 }	// chungo
};

int ene3BolaExplo[6][4] = {
	{ 130, 2, 30, 28 },
	{ 162, 1, 31, 32 },
	{ 194, 1, 32, 32 },
	{ 228, 2, 31, 30 },
	{ 261, 2, 31, 30 },
	{ 295, 1, 31, 31 }
};
int ene3BolaExploMid = 16;


int ene3Explo[9][4] = {
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
int ene3ExploMid[2] = { 65, 131 };


cEnemigo3::cEnemigo3(cSistema* sis, int x, int y, int numPiezas, float radio, float angle, float incAngle, bool antihorario) 
		: cEnemigo(sis, x, y) {
	_sis->cargaTextura(TEX_ENE3, "img\\r-typesheet31.png");
	_sis->cargaTextura(TEX_EXPLO2, "img\\r-typesheet44.png");

	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = ENEMIGO3_MUERE_DELAY;

	_numPiezas = numPiezas;
	vector<int> base(3);
	base[0] = _sis->dificultad()==DIFICULTAD_NORMAL ?
		ENEMIGO3_VIDA_INICIAL : ENEMIGO3_VIDA_INICIAL_HARD;	// la vida
	base[0] = int(base[0]*((cNivel*)_sis->nivel())->factorDificultad());
 	base[1] = 0;						// la seq cuando peta
	base[2] = ENEMIGO3_MUERE_DELAY;		// delay cuando peta
	_piezas.resize(_numPiezas, base);

	_antihorario = antihorario;
	_angle = angle;
	_incAngle = incAngle;
	
	_radio = radio;
	// perimetro = 2 PI radio
	//     36         ?
	// ----------- = ----
	//  perimetro     PI
	_diffAngle = (_PI*36.0f)/(_2PI*_radio);

	_puntos = ENEMIGO3_PUNTOS;

	_esJefe = true;
}

cEnemigo3::~cEnemigo3() {

}

void cEnemigo3::explota() {
	_state = ENEMIGO_EXPLO;
	for (int i=0; i<_numPiezas; ++i) {
		_piezas[i][0] = 0;
	}
}
void cEnemigo3::muerete() {
	// palma de golpe
	_state = ENEMIGO_MUERE;
  	_muerto = true;
}

void cEnemigo3::offset(int x, int y) {
	// el scroll no le afecta
}

void cEnemigo3::caja(cRect &rect) const {
	// que su caja sea la caja contenedora de su trayecto
	float radio = _radio + (ene3Cap[0][2]>>1);
	rect.w = rect.h = int(2 * radio);
	rect.x = _x - (rect.w>>1);
	rect.y = _y - (rect.h>>1);
}


void cEnemigo3::restaVida(int vida) {
	// magias afectan a las pelotillas individualmente
	for (int i = 0; i < _numPiezas; ++i) {
		if (_piezas[i][0] > 0) {
			_piezas[i][0] -= vida;
			if (_piezas[i][0] <= 0) {
				((cNaveEspacial*)_sis->naveEspacial())->sumaPuntos(_puntos);

				_sis->playSonido(SOUND_EXPLO2);
			}
		}
	}
}


// pre: inc <= 2 PI
float _E3_2PI = float(2 * acos(-1.0f));
inline float corrAng(float ang, float inc) {
	ang += inc;
	while (ang<0) ang += _E3_2PI;
	while (ang>= _E3_2PI) ang -= _E3_2PI;
	return ang;
}

void cEnemigo3::colision(cRect &rect,int &colMask) const {
	colMask = 0;
	if (_state != ENEMIGO_VIVE) return;

	float angle = _angle;
	// las pelotillas
	if (_antihorario) angle = corrAng(angle, -2*_diffAngle);
	else angle = corrAng(angle, 2*_diffAngle);
	for (int i=0; i<_numPiezas; ++i) {
		float x = _x + _radio*cos(angle);
		float y = _y + _radio*sin(angle);
		int* bola = _piezas[i][0]>0 ? ene3Cos[0] : ene3Cos[1];
		int wPixBola = bola[2];
		int hPixBola = bola[3]; 
		int xPixBola = int(x - (wPixBola>>1));
		int yPixBola = int(y - (hPixBola>>1));

		if (xPixBola<rect.x+rect.w && xPixBola+wPixBola>rect.x &&
			yPixBola<rect.y+rect.h && yPixBola+hPixBola>rect.y) {
				colMask = 1;
				return;
		}

		if (_antihorario) angle = corrAng(angle, -_diffAngle);
		else angle = corrAng(angle, _diffAngle);
	}
	// la cabeza o el cuello
	float seqFac = 16/_2PI;
	for (int i=0; i<4; ++i) {
		int* bola = NULL;
		if (i<2) {
			if (_antihorario) angle = corrAng(_angle, -i*_diffAngle);
			else angle = corrAng(_angle, i*_diffAngle);
			if (i%2) {
				// ? / 16 = ang / 2 PI
				int seqColl = int(seqFac*angle);
				seqColl = (seqColl+4) % 16;
				bola = ene3Coll[seqColl];
			} else {
				int seqCap = int(seqFac*angle);
				seqCap = (seqCap+4) % 16;
				bola = ene3Cap[seqCap];
			}
		} else {
			if (_antihorario) angle = corrAng(_angle, -(i+_numPiezas)*_diffAngle);
			else angle = corrAng(_angle, (i+_numPiezas)*_diffAngle);
			if (i%2) {
				int seqCap = int(seqFac*angle);
				seqCap = (seqCap+4) % 16;
				bola = ene3Cap[seqCap];
			} else {
				int seqColl = int(seqFac*angle);
				seqColl = (seqColl+4) % 16;
				bola = ene3Coll[seqColl];
			}
		}
		float x = _x + _radio*cos(angle);
		float y = _y + _radio*sin(angle);
		int wPixBola = bola[2];
		int hPixBola = bola[3]; 
		int xPixBola = int(x - (wPixBola>>1));
		int yPixBola = int(y - (hPixBola>>1));
					
		if (xPixBola<rect.x+rect.w && xPixBola+wPixBola>rect.x &&
			yPixBola<rect.y+rect.h && yPixBola+hPixBola>rect.y) {
				colMask = 1;
				return;
		}
	}
}

void cEnemigo3::logica() { // cambiar logica quan es vegi
	if (_state == ENEMIGO_VIVE) {
		// actualizar posicion
		if (_antihorario) _angle -= _incAngle;
		else _angle += _incAngle;

		// estos los necesitaremos por ahi
		cNivel* nivel = (cNivel*)_sis->nivel();
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();

		// matar el enemigo cuando queda fuera de la pantalla
		cRect rect;
		float radio = _radio + (ene3Cap[0][2]>>1);
		rect.w = rect.h = int(2 * radio);
		rect.x = _x - (rect.w>>1);
		rect.y = _y - (rect.h>>1);
		if (nivel->fueraLimites(rect)) {
			_muerto = true;
			return;
		}
		
		float angle = _angle;
		// las pelotillas
		if (_antihorario) angle = corrAng(angle, -2*_diffAngle);
		else angle = corrAng(angle, 2*_diffAngle);
		for (int i=0; i<_numPiezas; ++i) {
			float x = _x + _radio*cos(angle);
			float y = _y + _radio*sin(angle);
			int* bola = _piezas[i][0]>0 ? ene3Cos[0] : ene3Cos[1];
			int wPixBola = bola[2];
			int hPixBola = bola[3]; 
			int xPixBola = int(x - (wPixBola>>1));
			int yPixBola = int(y - (hPixBola>>1));

			// se come algun disparo bueno?
			list<cDisparo*> disparos = nivel->disparos();
			for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
				cDisparo* disparo = *it;
				if (!disparo->malo() && disparo->vive()) {
					cRect rectDisp;
					disparo->caja(rectDisp);
					if (xPixBola<rectDisp.x+rectDisp.w && xPixBola+wPixBola>rectDisp.x &&
						yPixBola<rectDisp.y+rectDisp.h && yPixBola+hPixBola>rectDisp.y) {
							if (_piezas[i][0] > 0) {
								_piezas[i][0] -= disparo->dano();
								if (_piezas[i][0] <= 0) {
									nave->sumaPuntos(_puntos);

									_sis->playSonido(SOUND_EXPLO2);
								}
							}
							disparo->explota();
					}
				}
			}

			// se come algun escudo?
			vector<cEscudo*> escudos = nave->escudos();
			for (unsigned int j = 0; j < escudos.size(); ++j) {
				cEscudo* escudo = escudos[j];
				cRect rectEsc;
				escudo->caja(rectEsc);
				if (xPixBola<rectEsc.x+rectEsc.w && xPixBola+wPixBola>rectEsc.x &&
					yPixBola<rectEsc.y+rectEsc.h && yPixBola+hPixBola>rectEsc.y) {
						if (_piezas[i][0] > 0) {
							_piezas[i][0] -= escudo->dano();
							if (_piezas[i][0] <= 0) {
								nave->sumaPuntos(_puntos);

								_sis->playSonido(SOUND_EXPLO2);
							}
						}
						escudo->choca();
				}
			}

			// intentar disparar
			int modulo = _sis->dificultad()==DIFICULTAD_NORMAL ? 
				ENEMIGO3_DISPARO_PIEZA : ENEMIGO3_DISPARO_PIEZA_HARD;
			modulo = int(modulo/nivel->factorDificultad());
			if (_piezas[i][0]>0 && !(rand()%modulo)) {
				float incAngle = (_2PI)/8;
				float angTiro = 0.0f;
				for (int i=0; i<8; ++i) {
					float xVecTiro = cos(angTiro);
					float yVecTiro = sin(angTiro);
					cDisparoEnemigo3* tiro = new cDisparoEnemigo3(_sis, int(x), int(y), xVecTiro, yVecTiro);
					nivel->pushDisparo(tiro);
					angTiro += incAngle;
				}
			}

			if (_antihorario) angle = corrAng(angle, -_diffAngle);
			else angle = corrAng(angle, _diffAngle);
		}
		// la cabeza o el cuello
		float seqFac = 16 / _2PI;
		for (int i=0; i<4; ++i) {
			int* bola = NULL;
			if (i<2) {
				if (_antihorario) angle = corrAng(_angle, -i*_diffAngle);
				else angle = corrAng(_angle, i*_diffAngle);
				if (i%2) {
					int seqColl = int(seqFac*angle);
					seqColl = (seqColl+4) % 16;
					bola = ene3Coll[seqColl];
				} else {
					int seqCap = int(seqFac*angle);
					seqCap = (seqCap+4) % 16;
					bola = ene3Cap[seqCap];
				}
			} else {
				if (_antihorario) angle = corrAng(_angle, -(i+_numPiezas)*_diffAngle);
				else angle = corrAng(_angle, (i+_numPiezas)*_diffAngle);
				if (i%2) {
					int seqCap = int(seqFac*angle);
					seqCap = (seqCap+4) % 16;
					bola = ene3Cap[seqCap];
				} else {
					int seqColl = int(seqFac*angle);
					seqColl = (seqColl+4) % 16;
					bola = ene3Coll[seqColl];
				}
			}
			float x = _x + _radio*cos(angle);
			float y = _y + _radio*sin(angle);
			int wPixBola = bola[2];
			int hPixBola = bola[3]; 
			int xPixBola = int(x - (wPixBola>>1));
			int yPixBola = int(y - (hPixBola>>1));
					
			// se come algun disparo bueno?
			list<cDisparo*> disparos = nivel->disparos();
			for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
				cDisparo* disparo = *it;
				if (!disparo->malo() && disparo->vive()) {
					cRect rectDisp;
					disparo->caja(rectDisp);
					if (xPixBola<rectDisp.x+rectDisp.w && xPixBola+wPixBola>rectDisp.x &&
						yPixBola<rectDisp.y+rectDisp.h && yPixBola+hPixBola>rectDisp.y) {
							disparo->explota();
					}
				}
			}

			// se come algun escudo?
			vector<cEscudo*> escudos = nave->escudos();
			for (unsigned int j = 0; j < escudos.size(); ++j) {
				cEscudo* escudo = escudos[j];
				cRect rectEsc;
				escudo->caja(rectEsc);
				if (xPixBola<rectEsc.x+rectEsc.w && xPixBola+wPixBola>rectEsc.x &&
					yPixBola<rectEsc.y+rectEsc.h && yPixBola+hPixBola>rectEsc.y) {
						escudo->choca();
				}
			}
		}
		
		// animacion explosion
		for (int i=0; i<_numPiezas; ++i) {
			if (_piezas[i][0]<=0 && _piezas[i][1]<ENEMIGO3_EXPLO_FRAMES) {
				if (_piezas[i][2]) --_piezas[i][2];
				else {
					++_piezas[i][1];
					_piezas[i][2] = ENEMIGO3_MUERE_DELAY;
				}
			}
		}
	} else if (_state == ENEMIGO_EXPLO) {
		// animacion explosion
		for (int i=0; i<_numPiezas; ++i) {
			if (_piezas[i][0]<=0 && _piezas[i][1]<ENEMIGO3_EXPLO_FRAMES) {
				if (_piezas[i][2]) --_piezas[i][2];
				else {
					++_piezas[i][1];
					_piezas[i][2] = ENEMIGO3_MUERE_DELAY;
				}
			}
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
		if (_exploCuerpo.size() < ENEMIGO3_MAX_NUM_EXPLO && !(rand() % 4)) {
			cExplo explo;
  			explo.seq = 0;
			explo.delay = JEFE1_EXPLO_DELAY;

			int i = rand() % (_numPiezas + 4);
			if (i < _numPiezas) {
				// bolas
				float angle = corrAng(_angle, -(i+2)*_diffAngle);
				float x = _x + _radio*cos(angle);
				float y = _y + _radio*sin(angle);
				int* bola = ene3Cos[1];
				int wPixBola = bola[2];
				int hPixBola = bola[3]; 
				int xPixBola = int(x - (wPixBola>>1));
				int yPixBola = int(y - (hPixBola>>1));
				explo.x = xPixBola + (rand() % wPixBola);
				explo.y = yPixBola + (rand() % hPixBola);
			} else {
				// cabeza o cuello
				i -= _numPiezas;
				int* bola = NULL;
				float angle = _angle;
				if (i<2) {
					if (_antihorario) angle = corrAng(_angle, -i*_diffAngle);
					else angle = corrAng(_angle, i*_diffAngle);
					if (i%2) {
						int seqColl = int((16.0f*angle)/(_2PI));
						seqColl = (seqColl+4) % 16;
						bola = ene3Coll[seqColl];
					} else {
						int seqCap = int((16.0f*angle)/(_2PI));
						seqCap = (seqCap+4) % 16;
						bola = ene3Cap[seqCap];
					}
				} else {
					if (_antihorario) angle = corrAng(_angle, -(i+_numPiezas)*_diffAngle);
					else angle = corrAng(_angle, (i+_numPiezas)*_diffAngle);
					if (i%2) {
						int seqCap = int((16.0f*angle)/(_2PI));
						seqCap = (seqCap+4) % 16;
						bola = ene3Cap[seqCap];
					} else {
						int seqColl = int((16.0f*angle)/(_2PI));
						seqColl = (seqColl+4) % 16;
						bola = ene3Coll[seqColl];
					}
				}
				float x = _x + _radio*cos(angle);
				float y = _y + _radio*sin(angle);
				int wPixBola = bola[2];
				int hPixBola = bola[3]; 
				int xPixBola = int(x - (wPixBola>>1));
				int yPixBola = int(y - (hPixBola>>1));
				explo.x = xPixBola + (rand() % wPixBola);
				explo.y = yPixBola + (rand() % hPixBola);
			}
			_exploCuerpo.push_back(explo);
		}
	}
}

void cEnemigo3::pintaVivo() const {
	int tex = _sis->idTextura(TEX_ENE3);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_ENE3, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	float angle = _angle;
	// las pelotillas
	if (_antihorario) angle = corrAng(angle, -2*_diffAngle);
	else angle = corrAng(angle, 2*_diffAngle);
	for (int i=0; i<_numPiezas; ++i) {
		float x = _x + _radio*cos(angle);
		float y = _y + _radio*sin(angle);
		int* bola = _piezas[i][0]>0 ? ene3Cos[0] : ene3Cos[1];
		int wPixBola = bola[2];
		int hPixBola = bola[3]; 
		int xPixBola = int(x - (wPixBola>>1));
		int yPixBola = int(GAME_HEIGHT - (y + (hPixBola>>1)));
		float xTexBola = bola[0] / (float)wTex;
		float yTexBola = bola[1] / (float)hTex;
		float wTexBola = bola[2] / (float)wTex;
		float hTexBola = bola[3] / (float)hTex;

		glTexCoord2f(xTexBola, yTexBola + hTexBola);			glVertex2i(xPixBola, yPixBola);
		glTexCoord2f(xTexBola + wTexBola, yTexBola + hTexBola);	glVertex2i(xPixBola + wPixBola, yPixBola);
		glTexCoord2f(xTexBola + wTexBola, yTexBola);			glVertex2i(xPixBola + wPixBola, yPixBola + hPixBola);
		glTexCoord2f(xTexBola, yTexBola);						glVertex2i(xPixBola, yPixBola + hPixBola);

		if (_antihorario) angle = corrAng(angle, -_diffAngle);
		else angle = corrAng(angle, _diffAngle);
	}
	// la cabeza o el cuello
	float seqFac = 16 / _2PI;
	for (int i=0; i<4; ++i) {
		int* bola = NULL;
		if (i<2) {
			if (_antihorario) angle = corrAng(_angle, -i*_diffAngle);
			else angle = corrAng(_angle, i*_diffAngle);
			if (i%2) {
				int seqColl = int(seqFac*angle);
				seqColl = (seqColl+4) % 16;
				bola = ene3Coll[seqColl];
			} else {
				int seqCap = int(seqFac*angle);
				seqCap = (seqCap+4) % 16;
				bola = ene3Cap[seqCap];
			}
		} else {
			if (_antihorario) angle = corrAng(_angle, -(i+_numPiezas)*_diffAngle);
			else angle = corrAng(_angle, (i+_numPiezas)*_diffAngle);
			if (i%2) {
				int seqCap = int(seqFac*angle);
				seqCap = (seqCap+4) % 16;
				bola = ene3Cap[seqCap];
			} else {
				int seqColl = int(seqFac*angle);
				seqColl = (seqColl+4) % 16;
				bola = ene3Coll[seqColl];
			}
		}
		float x = _x + _radio*cos(angle);
		float y = _y + _radio*sin(angle);
		int wPixBola = bola[2];
		int hPixBola = bola[3]; 
		int xPixBola = int(x - (wPixBola>>1));
		int yPixBola = int(GAME_HEIGHT - (y + (hPixBola>>1)));
		float xTexBola = bola[0] / (float)wTex;
		float yTexBola = bola[1] / (float)hTex;
		float wTexBola = bola[2] / (float)wTex;
		float hTexBola = bola[3] / (float)hTex;

		glTexCoord2f(xTexBola, yTexBola + hTexBola);			glVertex2i(xPixBola, yPixBola);
		glTexCoord2f(xTexBola + wTexBola, yTexBola + hTexBola);	glVertex2i(xPixBola + wPixBola, yPixBola);
		glTexCoord2f(xTexBola + wTexBola, yTexBola);			glVertex2i(xPixBola + wPixBola, yPixBola + hPixBola);
		glTexCoord2f(xTexBola, yTexBola);						glVertex2i(xPixBola, yPixBola + hPixBola);
		
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);

	// ahora las explosiones
	int texExplo = _sis->idTextura(TEX_EXPLO2);
	_sis->tamanoTextura(TEX_EXPLO2, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExplo);
	glBegin(GL_QUADS);

	angle = _angle;
	if (_antihorario) angle = corrAng(angle, -2*_diffAngle);
	else angle = corrAng(angle, 2*_diffAngle);
	for (int i=0; i<_numPiezas; ++i) {
		float x = _x + _radio*cos(angle);
		float y = _y + _radio*sin(angle);
		if (_piezas[i][0]<=0 && _piezas[i][1]<ENEMIGO3_EXPLO_FRAMES) {
			int seq = _piezas[i][1];
			int wPixBola = ene3BolaExplo[seq][2];
			int hPixBola = ene3BolaExplo[seq][3]; 
			int xPixBola = int(x - (wPixBola>>1));
			int yPixOffset =  ene3BolaExploMid - ene3BolaExplo[seq][1];
			int yPixBola = int(GAME_HEIGHT - (y - yPixOffset + hPixBola));
			float xTexBola = ene3BolaExplo[seq][0] / (float)wTex;
			float yTexBola = ene3BolaExplo[seq][1] / (float)hTex;
			float wTexBola = ene3BolaExplo[seq][2] / (float)wTex;
			float hTexBola = ene3BolaExplo[seq][3] / (float)hTex;

			glTexCoord2f(xTexBola, yTexBola + hTexBola);			glVertex2i(xPixBola, yPixBola);
			glTexCoord2f(xTexBola + wTexBola, yTexBola + hTexBola);	glVertex2i(xPixBola + wPixBola, yPixBola);
			glTexCoord2f(xTexBola + wTexBola, yTexBola);			glVertex2i(xPixBola + wPixBola, yPixBola + hPixBola);
			glTexCoord2f(xTexBola, yTexBola);						glVertex2i(xPixBola, yPixBola + hPixBola);
		}
		if (_antihorario) angle = corrAng(angle, -_diffAngle);
		else angle = corrAng(angle, _diffAngle);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cEnemigo3::pintaExplo() const {
	int tex = _sis->idTextura(TEX_ENE3);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_ENE3, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	float angle = _angle;
	// las pelotillas
	if (_antihorario) angle = corrAng(angle, -2*_diffAngle);
	else angle = corrAng(angle, 2*_diffAngle);
	for (int i=0; i<_numPiezas; ++i) {
		float x = _x + _radio*cos(angle);
		float y = _y + _radio*sin(angle);
		int* bola = _piezas[i][0]>0 ? ene3Cos[0] : ene3Cos[1];
		int wPixBola = bola[2];
		int hPixBola = bola[3]; 
		int xPixBola = int(x - (wPixBola>>1));
		int yPixBola = int(GAME_HEIGHT - (y + (hPixBola>>1)));
		float xTexBola = bola[0] / (float)wTex;
		float yTexBola = bola[1] / (float)hTex;
		float wTexBola = bola[2] / (float)wTex;
		float hTexBola = bola[3] / (float)hTex;

		glTexCoord2f(xTexBola, yTexBola + hTexBola);			glVertex2i(xPixBola, yPixBola);
		glTexCoord2f(xTexBola + wTexBola, yTexBola + hTexBola);	glVertex2i(xPixBola + wPixBola, yPixBola);
		glTexCoord2f(xTexBola + wTexBola, yTexBola);			glVertex2i(xPixBola + wPixBola, yPixBola + hPixBola);
		glTexCoord2f(xTexBola, yTexBola);						glVertex2i(xPixBola, yPixBola + hPixBola);

		if (_antihorario) angle = corrAng(angle, -_diffAngle);
		else angle = corrAng(angle, _diffAngle);
	}
	// la cabeza o el cuello
	float seqFac = 16 / _2PI;
	for (int i=0; i<4; ++i) {
		int* bola = NULL;
		if (i<2) {
			if (_antihorario) angle = corrAng(_angle, -i*_diffAngle);
			else angle = corrAng(_angle, i*_diffAngle);
			if (i%2) {
				int seqColl = int(seqFac*angle);
				seqColl = (seqColl+4) % 16;
				bola = ene3Coll[seqColl];
			} else {
				int seqCap = int(seqFac*angle);
				seqCap = (seqCap+4) % 16;
				bola = ene3Cap[seqCap];
			}
		} else {
			if (_antihorario) angle = corrAng(_angle, -(i+_numPiezas)*_diffAngle);
			else angle = corrAng(_angle, (i+_numPiezas)*_diffAngle);
			if (i%2) {
				int seqCap = int(seqFac*angle);
				seqCap = (seqCap+4) % 16;
				bola = ene3Cap[seqCap];
			} else {
				int seqColl = int(seqFac*angle);
				seqColl = (seqColl+4) % 16;
				bola = ene3Coll[seqColl];
			}
		}
		float x = _x + _radio*cos(angle);
		float y = _y + _radio*sin(angle);
		int wPixBola = bola[2];
		int hPixBola = bola[3]; 
		int xPixBola = int(x - (wPixBola>>1));
		int yPixBola = int(GAME_HEIGHT - (y + (hPixBola>>1)));
		float xTexBola = bola[0] / (float)wTex;
		float yTexBola = bola[1] / (float)hTex;
		float wTexBola = bola[2] / (float)wTex;
		float hTexBola = bola[3] / (float)hTex;

		glTexCoord2f(xTexBola, yTexBola + hTexBola);			glVertex2i(xPixBola, yPixBola);
		glTexCoord2f(xTexBola + wTexBola, yTexBola + hTexBola);	glVertex2i(xPixBola + wPixBola, yPixBola);
		glTexCoord2f(xTexBola + wTexBola, yTexBola);			glVertex2i(xPixBola + wPixBola, yPixBola + hPixBola);
		glTexCoord2f(xTexBola, yTexBola);						glVertex2i(xPixBola, yPixBola + hPixBola);
		
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);

	// ahora las explosiones
	int texExplo = _sis->idTextura(TEX_EXPLO2);
	_sis->tamanoTextura(TEX_EXPLO2, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExplo);
	glBegin(GL_QUADS);

	angle = _angle;
	if (_antihorario) angle = corrAng(angle, -2*_diffAngle);
	else angle = corrAng(angle, 2*_diffAngle);
	for (int i=0; i<_numPiezas; ++i) {
		float x = _x + _radio*cos(angle);
		float y = _y + _radio*sin(angle);
		if (_piezas[i][0]<=0 && _piezas[i][1]<ENEMIGO3_EXPLO_FRAMES) {
			int seq = _piezas[i][1];
			int wPixBola = ene3BolaExplo[seq][2];
			int hPixBola = ene3BolaExplo[seq][3]; 
			int xPixBola = int(x - (wPixBola>>1));
			int yPixOffset =  ene3BolaExploMid - ene3BolaExplo[seq][1];
			int yPixBola = int(GAME_HEIGHT - (y - yPixOffset + hPixBola));
			float xTexBola = ene3BolaExplo[seq][0] / (float)wTex;
			float yTexBola = ene3BolaExplo[seq][1] / (float)hTex;
			float wTexBola = ene3BolaExplo[seq][2] / (float)wTex;
			float hTexBola = ene3BolaExplo[seq][3] / (float)hTex;

			glTexCoord2f(xTexBola, yTexBola + hTexBola);			glVertex2i(xPixBola, yPixBola);
			glTexCoord2f(xTexBola + wTexBola, yTexBola + hTexBola);	glVertex2i(xPixBola + wPixBola, yPixBola);
			glTexCoord2f(xTexBola + wTexBola, yTexBola);			glVertex2i(xPixBola + wPixBola, yPixBola + hPixBola);
			glTexCoord2f(xTexBola, yTexBola);						glVertex2i(xPixBola, yPixBola + hPixBola);
		}
		if (_antihorario) angle = corrAng(angle, -_diffAngle);
		else angle = corrAng(angle, _diffAngle);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// ahora las explosiones grandes
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExplo);
	glBegin(GL_QUADS);

	for (list<cExplo>::const_iterator it=_exploCuerpo.begin(); it!=_exploCuerpo.end(); ++it) {
		const cExplo &explo = *it;
		float xTexCola = ene3Explo[explo.seq][0] / (float)wTex;
		float yTexCola = ene3Explo[explo.seq][1] / (float)hTex;
		float wTexCola = ene3Explo[explo.seq][2] / (float)wTex;
		float hTexCola = ene3Explo[explo.seq][3] / (float)hTex;
		int wPixCola = ene3Explo[explo.seq][2];
		int hPixCola = ene3Explo[explo.seq][3]; 
		int xPixCola = int(explo.x - (wPixCola>>1));
		int yPixOffset =  ene3ExploMid[explo.seq / 5] - ene3Explo[explo.seq][1];
		int yPixCola = int(GAME_HEIGHT - (explo.y - yPixOffset + hPixCola));
				
		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}



void cEnemigo3::pinta() const {
	if (_state == ENEMIGO_MUERE) return;
	if (_state == ENEMIGO_VIVE) pintaVivo();
	else if (_state == ENEMIGO_EXPLO) pintaExplo();
}

