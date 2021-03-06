#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>

int jefe3Mov[1][4] = {
	{ 34, 2, 64, 64 }
};

int jefe3MovMid = 34;

int jefe3Explo[9][4] = {
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

int jefe3ExploMid[2] = { 65, 131 };

cJefe3::cJefe3(cSistema* sis) : cEnemigo(sis) {
	_sis->cargaTextura(TEX_JEFE3, "img\\r-typesheet39.png");
	_sis->cargaTextura(TEX_EXPLO2, "img\\r-typesheet44.png");

	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = JEFE3_MUEVE_DELAY;

	_vida = _sis->dificultad() == DIFICULTAD_NORMAL ? JEFE3_VIDA_INICIAL : JEFE3_VIDA_INICIAL_HARD;
	_puntos = JEFE3_PUNTOS;

	_tiempoVida = 0;
	_ultimoImpacto = -JEFE3_FLASH_IMPACTO;

	_x = 5075;
	_y = 480;

	_subState = JEFE3_MOVE_UP;

	_esJefe = true;
	creaMinis();
}

cJefe3::~cJefe3() {

}

void cJefe3::creaMinis() {
	int i, j, n, x;
	n = _sis->dificultad() == DIFICULTAD_NORMAL ? JEFE3_NUM_MINIS : JEFE3_NUM_MINIS_HARD;

	for (i = 0; i < 5; i++) { //x 5000->5150
		for (j = 0; j < 5; j++) { // y 150->250
			cMiniJefe3* e1 = new cMiniJefe3(_sis, (5000+30*i), (430+20*j));
			_minis.insert(_minis.end(), e1);
			((cNivel*)_sis->nivel())->pushEnemigo(e1);
		}
	}
	for (x = 0; x < n; x++) {
		int xRand, yRand;
		xRand = rand() % 150;
		yRand = rand() % 100;
		cMiniJefe3* e2 = new cMiniJefe3(_sis, (5000 + xRand), (430 + yRand));
		_minis.insert(_minis.end(), e2);
		((cNivel*)_sis->nivel())->pushEnemigo(e2);
	}
}

void cJefe3::muerete() {
	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_delay = JEFE3_MUERE_DELAY;
	_seqExplo = 0;
	_tiempoMuerto = 0;
}

void cJefe3::offset(int x, int y) {
	// el nivel no lo mueve
}

void cJefe3::caja(cRect &rect) const {
	// estes no es necesario para un jefe
}

void cJefe3::restaVida(int vida) {
	if (_minis.size() <= 0) {
		// las magias le afectan solo cuando no quedan minis
		cEnemigo::restaVida(vida);
		_ultimoImpacto = _tiempoVida;
	}
};

void cJefe3::colision(cRect &rect, int &colMask) const {
	colMask = 0;
	if (_state == ENEMIGO_VIVE) {
		int wPixSolido = jefe3Mov[_seq][2];
		int hPixSolido = jefe3Mov[_seq][3];
		int xPixSolido = _x - (wPixSolido >> 1);
		int yPixSolido = _y - (hPixSolido >> 1);
		if (xPixSolido < rect.x + rect.w && xPixSolido + wPixSolido > rect.x &&
			yPixSolido < rect.y + rect.h && yPixSolido + hPixSolido > rect.y) {
			colMask = 1;
			return;
		}
	}
}

void cJefe3::logica() {
	if (_state == ENEMIGO_VIVE) {

		++_tiempoVida;
		eliminaMinisLista();

		//mover el jefe
		int x = 0;
		int y = 0;
		if (_subState == JEFE3_MOVE_UP) {
			if (_minis.size() > 0) _y -= JEFE3_INC_MOV;
			else _y -= 2 * JEFE3_INC_MOV;
			y = -JEFE3_INC_MOV;
			if (_y <= 150) _subState = JEFE3_MOVE_RIGHT; //limite arriba
		}
		else if (_subState == JEFE3_MOVE_RIGHT) {
			if (_minis.size() > 0) _x += JEFE3_INC_MOV;
			else _x += 2 * JEFE3_INC_MOV;
			x = JEFE3_INC_MOV;
			if (_x >= 5150) _subState = JEFE3_MOVE_DOWN; //limite derecha
		}
		else if (_subState == JEFE3_MOVE_DOWN) {
			if (_minis.size() > 0) _y += JEFE3_INC_MOV;
			else _y += 2 * JEFE3_INC_MOV;
			y = JEFE3_INC_MOV;
			if (_y >= 300) _subState = JEFE3_MOVE_LEFT; //limite abajo
		}
		else if (_subState == JEFE3_MOVE_LEFT) {
			if (_minis.size() > 0) _x -= JEFE3_INC_MOV;
			else _x -= 2 * JEFE3_INC_MOV;
			x = -JEFE3_INC_MOV;
			if (_x <= 5000) _subState = JEFE3_MOVE_UP; //limite izquierda
		}

		for (list<cMiniJefe3*>::iterator it = _minis.begin(); it != _minis.end();) {
			cMiniJefe3* single = *it;
			single->moveIt(x, y);
			++it;
		}

		cNivel* nivel = (cNivel*)_sis->nivel();
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
		
		if (_minis.size() <= 0) {
			// chequear impactos solo si esta solo
			cRect rect;
			rect.w = jefe3Mov[_seq][2];
			rect.h = jefe3Mov[_seq][3];
			rect.x = _x - (rect.w >> 1);
			rect.y = _y - (rect.h >> 1);
			list<cDisparo*> disparos = nivel->disparos();
			for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
				cDisparo* disparo = *it;
				if (!disparo->malo() && disparo->vive()) {
					int colMask;
					disparo->colision(rect, colMask);
					if (colMask) {
						_delay = JEFE3_MUEVE_DELAY;
						disparo->explota();
						// aplicar efecto del disparo
						if (_minis.size() <= 0)	_vida -= disparo->dano();
						_ultimoImpacto = _tiempoVida;
					}
					else {
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
				if (colMask) {
					escudo->choca();
					long long tiempoFlash = _tiempoVida - _ultimoImpacto;
					if (tiempoFlash >= JEFE3_FLASH_IMPACTO) {
						_vida -= escudo->dano();
						_ultimoImpacto = _tiempoVida;
					}
				}
				else {
					cRect rectDisp;
					escudo->caja(rectDisp);
					colision(rectDisp, colMask);
					if (colMask) escudo->choca();
				}
			}
		}

		long long tiempoFlash = _tiempoVida - _ultimoImpacto;
		if (tiempoFlash < JEFE3_FLASH_IMPACTO && !(tiempoFlash % 6)) {
			_sis->playSonido(SOUND_ENEMIGO_HIT);
		}

		if (_vida <= 0) {
			nave->sumaPuntos(_puntos);
			mataMinis();
			muerete();
			return;
		}

		if (_minis.size()>0) {
			if (_tiempoVida > 160) {
				//ataque minijefes
 				int aux = _sis->dificultad() == DIFICULTAD_NORMAL ? JEFE3_INTERVALO_MINIS_ATTACK : JEFE3_INTERVALO_MINIS_ATTACK_HARD;
				int auxRandom = rand() % aux;
				if (auxRandom == 1) {
					list<cMiniJefe3*>::iterator it = _minis.begin();
					cMiniJefe3* single = *it;
					//single attack
					single->attack();
					it = _minis.erase(it);
				}
			}
		}

		else {
		// ataque frenetico si jefe esta solo
			int aux = _sis->dificultad() == DIFICULTAD_NORMAL ? JEFE3_INTERVALO_ATTACK : JEFE3_INTERVALO_ATTACK_HARD;
			int auxRandom = rand() % aux;
			if (auxRandom == 1) {
				int xTiro = _x;
				int yTiro = _y;
				int xNave, yNave;
				_sis->naveEspacial()->getPosicion(xNave, yNave);
				float xVec = float(xNave - xTiro);
				float yVec = float(yNave - yTiro);
				cDisparoJefe1* tiro = new cDisparoJefe1(_sis, xTiro, yTiro, xVec, yVec);
				nivel->pushDisparo(tiro);
			}
		}


		// actualizar estado
		/*if (_tiempoVida == 0) {
			_subState = JEFE3_MOVE_UP;
		}*/

	}

	else if (_state == ENEMIGO_EXPLO) {
		++_tiempoMuerto;
		if (_tiempoMuerto == JEFE3_TIEMPO_MUERTO) {
			((cNivel*)_sis->nivel())->termina();
		}

		for (list<cExplo>::iterator it = _exploCuerpo.begin(); it != _exploCuerpo.end();) {
			cExplo &explo = *it;
			bool borra = false;
			if (explo.delay) --explo.delay;
			else {
				++explo.seq;
				explo.delay = JEFE3_EXPLO_DELAY;
				borra = explo.seq == JEFE3_EXPLO_NUM_FRAMES;
			}
			if (borra) it = _exploCuerpo.erase(it);
			else ++it;
		}
		// genera una explosion random
		if (_exploCuerpo.size() < JEFE3_MAX_NUM_EXPLO && !(rand() % 4)) {
			_sis->playSonido(SOUND_EXPLO1);

			cExplo explo;
			explo.seq = 0;
			explo.delay = JEFE3_EXPLO_DELAY;
			explo.x = _x - (jefe3Mov[_seq][2]>>1) + (rand() % jefe3Mov[_seq][2]);
			explo.y = _y - (jefe3Mov[_seq][3]>>1) + (rand() % jefe3Mov[_seq][3]);
			_exploCuerpo.push_back(explo);
		}
	}
}

void cJefe3::eliminaMinisLista() {
	for (list<cMiniJefe3*>::iterator it = _minis.begin(); it != _minis.end();) {
		cMiniJefe3* single = *it;
		if (single->isDead()) {
			it = _minis.erase(it);
		}
		else ++it;
	}
}

void cJefe3::mataMinis() {
	for (list<cMiniJefe3*>::iterator it = _minis.begin(); it != _minis.end();) {
		cMiniJefe3* single = *it;
		single->muerete();
		++it;
	}
	eliminaMinisLista();
}

void cJefe3::pintaVivo() const {
	// textura jefe
	int tex = _sis->idTextura(TEX_JEFE3);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_JEFE3, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	// para este, sus coordenadas (_x, _y) corresponden con su esquina superior izquierda
	float xTexEne = jefe3Mov[_seq][0] / (float)wTex;
	float yTexEne = jefe3Mov[_seq][1] / (float)hTex;
	float wTexEne = jefe3Mov[_seq][2] / (float)wTex;
	float hTexEne = jefe3Mov[_seq][3] / (float)hTex;
	int wPixEne = jefe3Mov[_seq][2];
	int hPixEne = jefe3Mov[_seq][3];
	int xPixEne = _x - (wPixEne>>1);
	int yPixEne = GAME_HEIGHT - (_y + (hPixEne>>1));

	long long tiempoFlash = _tiempoVida - _ultimoImpacto;
	if (tiempoFlash < JEFE3_FLASH_IMPACTO) glColor3f(tiempoFlash % 3 == 0, tiempoFlash % 3 == 1, tiempoFlash % 3 == 2);
	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);
	if (tiempoFlash < JEFE3_FLASH_IMPACTO) glColor3f(1, 1, 1);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cJefe3::pintaExplo() const {
	// textura jefe
	int tex = _sis->idTextura(TEX_JEFE3);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_JEFE3, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	// para este, sus coordenadas (_x, _y) corresponden con su esquina superior izquierda
	float xTexEne = jefe3Mov[_seq][0] / (float)wTex;
	float yTexEne = jefe3Mov[_seq][1] / (float)hTex;
	float wTexEne = jefe3Mov[_seq][2] / (float)wTex;
	float hTexEne = jefe3Mov[_seq][3] / (float)hTex;
	int wPixEne = jefe3Mov[_seq][2];
	int hPixEne = jefe3Mov[_seq][3];
	int xPixEne = _x - (wPixEne >> 1);
	int yPixEne = GAME_HEIGHT - (_y + (hPixEne >> 1));

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
		float xTexCola = jefe3Explo[explo.seq][0] / (float)wTex;
		float yTexCola = jefe3Explo[explo.seq][1] / (float)hTex;
		float wTexCola = jefe3Explo[explo.seq][2] / (float)wTex;
		float hTexCola = jefe3Explo[explo.seq][3] / (float)hTex;
		int wPixCola = jefe3Explo[explo.seq][2];
		int hPixCola = jefe3Explo[explo.seq][3];
		int xPixCola = int(explo.x - (wPixCola >> 1));
		int yPixOffset = jefe3ExploMid[explo.seq / 5] - jefe3Explo[explo.seq][1];
		int yPixCola = int(GAME_HEIGHT - (explo.y - yPixOffset + hPixCola));

		glTexCoord2f(xTexCola, yTexCola + hTexCola);			glVertex2i(xPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola + hTexCola);	glVertex2i(xPixCola + wPixCola, yPixCola);
		glTexCoord2f(xTexCola + wTexCola, yTexCola);			glVertex2i(xPixCola + wPixCola, yPixCola + hPixCola);
		glTexCoord2f(xTexCola, yTexCola);						glVertex2i(xPixCola, yPixCola + hPixCola);

	}

	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void cJefe3::pinta() const {
	if (_state == ENEMIGO_MUERE) return;
	if (_state == ENEMIGO_VIVE) pintaVivo();
	else if (_state == ENEMIGO_EXPLO) pintaExplo();
}


//-----------------------------------
// minijefe 3
//-----------------------------------

int mini3Mov[4][4] = {
	{ 2, 73, 30, 28 },
	{ 34, 73, 31, 28 },
	{ 68, 72, 31, 29 },
	{ 101, 73, 31, 28 }
};

int mini3MovMid[2] = { 17, 85 };

int mini3Explo[6][4] = {
	{ 264, 318, 18, 15 },
	{ 217, 300, 28, 20 },
	{ 182, 297, 32, 27 },
	{ 146, 296, 32, 29 },
	{ 109, 296, 32, 30 },
	{ 76, 296, 32, 30 }
};

int mini3ExploMid = 311;

cMiniJefe3::cMiniJefe3(cSistema* sis, int x, int y) : cEnemigo(sis, x, y) {
	_sis->cargaTextura(TEX_MINIJEFE3, "img\\r-typesheet39.png");
	_sis->cargaTextura(TEX_NAVE1, "img\\r-typesheet1.png");

	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = JEFE3_MINI_MUEVE_DELAY;

	_vida = _sis->dificultad() == DIFICULTAD_NORMAL ? JEFE3_MINI_VIDA_INICIAL : JEFE3_MINI_VIDA_INICIAL_HARD;
	_puntos = JEFE3_MINI_PUNTOS;

	_tiempoVida = 0;
	_ultimoImpacto = -JEFE3_MINI_FLASH_IMPACTO;

	_subState = JEFE3_MOVE_UP;
	_pixelsAvanzaX = 0;
	_pixelsAvanzaY = 0;
	_esJefe = true;
}

cMiniJefe3::~cMiniJefe3() {

}

void cMiniJefe3::muerete() {
	_sis->playSonido(SOUND_EXPLO1);

	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_seq = 0;
	_delay = JEFE3_MINI_MUERE_DELAY;
	_tiempoMuerto = 0;
}

void cMiniJefe3::caja(cRect &rect) const {
	// estes no es necesario para un jefe
}

void cMiniJefe3::restaVida(int vida) {
	// impactos fisicos de la nave o magias le quitan muy poca vida
	cEnemigo::restaVida(vida / 10);
	_ultimoImpacto = _tiempoVida;
};

void cMiniJefe3::colision(cRect &rect, int &colMask) const {
	colMask = 0;
	if (_state == ENEMIGO_VIVE) {
		int wPixSolido = mini3Mov[_seq][2];
		int hPixSolido = mini3Mov[_seq][3];
		int xPixSolido = _x - (wPixSolido >> 1);
		int yPixSolido = _y - (hPixSolido >> 1);
		if (xPixSolido < rect.x + rect.w && xPixSolido + wPixSolido > rect.x &&
			yPixSolido < rect.y + rect.h && yPixSolido + hPixSolido > rect.y) {
			colMask = 1;
			return;
		}
	}
}

void cMiniJefe3::offset(int x, int y) {

}

void cMiniJefe3::moveIt(int x, int y) {
	_x += x;
	_y += y;
}

bool cMiniJefe3::isDead() {
	bool res = false;
	if (_state == ENEMIGO_MUERE) res = true;
	return res;
}

void cMiniJefe3::attack() {
	int nX, nY;
	cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
	nave->getPosicion(nX, nY);
	float vectX = float(nX - _x);
	float vectY = float(nY - _y);
	float len = sqrt(vectX*vectX + vectY*vectY);
	vectX /= len;
	vectY /= len;
	_pixelsAvanzaX = vectX;
	_pixelsAvanzaY = vectY;
	_subState = JEFE3_MINI_ATTACK;
}

void cMiniJefe3::logica() {
	if (_state == ENEMIGO_VIVE) {

		++_tiempoVida;

		cNivel* nivel = (cNivel*)_sis->nivel();
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();

		if (_subState == JEFE3_MINI_ATTACK) {
			//seguir su trayectoria
			_x += int(_pixelsAvanzaX*VELOCIDAD_JEFE3_MINI);
			_y += int(_pixelsAvanzaY*VELOCIDAD_JEFE3_MINI);
		}

		bool naveLoMata = false;

		// chequear impactos
		cRect rect;
		rect.w = mini3Mov[_seq][2];
		rect.h = mini3Mov[_seq][3];
		rect.x = _x - (rect.w >> 1);
		rect.y = _y - (rect.h >> 1);
		list<cDisparo*> disparos = nivel->disparos();
		for (list<cDisparo*>::iterator it = disparos.begin(); it != disparos.end(); ++it) {
			cDisparo* disparo = *it;
			if (!disparo->malo() && disparo->vive()) {
				int colMask;
				disparo->colision(rect, colMask);
				if (colMask) {
					_delay = JEFE3_MINI_MUEVE_DELAY;
					disparo->explota();
					// aplicar efecto del disparo
					_vida -= disparo->dano();
					if (_vida <= 0) naveLoMata = true;
					_ultimoImpacto = _tiempoVida;
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
				long long tiempoFlash = _tiempoVida - _ultimoImpacto;
				if (tiempoFlash >= JEFE3_MINI_FLASH_IMPACTO) {
					_vida -= escudo->dano();
					if (_vida <= 0) naveLoMata = true;
					_ultimoImpacto = _tiempoVida;
				}
			}
		}

		if (naveLoMata) nave->sumaPuntos(_puntos);

		if (_vida <= 0) {
			muerete();
			return;
		}

		long long tiempoFlash = _tiempoVida - _ultimoImpacto;
		if (tiempoFlash < JEFE3_MINI_FLASH_IMPACTO && !(tiempoFlash % 6)) {
			_sis->playSonido(SOUND_ENEMIGO_HIT);
		}

		// animacion
		if (_delay) --_delay;
		else {
			_seq = (_seq + 1) % JEFE3_MINI_NUM_FRAMES;
			_delay = JEFE3_MINI_MUEVE_DELAY;
		}

	}

	else if (_state == ENEMIGO_EXPLO) {
		if (_delay) {
			--_delay;
		}
		else {
			++_seq;
			_delay = JEFE3_MINI_MUERE_DELAY;
		}
		if (_seq == JEFE3_MINI_EXPLO_NUM_FRAMES) {
			_state = ENEMIGO_MUERE;
			_muerto = true;
		}
	}
}

void cMiniJefe3::pinta() const {
	if (_state == ENEMIGO_MUERE) return;

	float xTexEne, yTexEne, wTexEne, hTexEne;
	int xPixEne, yPixEne, yPixOffset, wPixEne, hPixEne;

	if (_state == ENEMIGO_VIVE) {
		int tex = _sis->idTextura(TEX_MINIJEFE3);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_MINIJEFE3, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexEne = mini3Mov[_seq][0] / (float)wTex;
		yTexEne = mini3Mov[_seq][1] / (float)hTex;
		wTexEne = mini3Mov[_seq][2] / (float)wTex;
		hTexEne = mini3Mov[_seq][3] / (float)hTex;
		wPixEne = mini3Mov[_seq][2];
		hPixEne = mini3Mov[_seq][3];
		xPixEne = _x - (wPixEne >> 1);
		yPixOffset = (hPixEne >> 1);// mini3MovMid[_seq / 8] - mini3Mov[_seq][1];
		yPixEne = GAME_HEIGHT - (_y - yPixOffset + hPixEne);
	}
	else if (_state == ENEMIGO_EXPLO) {
		int tex = _sis->idTextura(TEX_NAVE1);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_NAVE1, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexEne = mini3Explo[_seq][0] / (float)wTex;
		yTexEne = mini3Explo[_seq][1] / (float)hTex;
		wTexEne = mini3Explo[_seq][2] / (float)wTex;
		hTexEne = mini3Explo[_seq][3] / (float)hTex;
		wPixEne = mini3Explo[_seq][2];
		hPixEne = mini3Explo[_seq][3];
		xPixEne = _x - (wPixEne >> 1);
		yPixOffset = mini3ExploMid - mini3Explo[_seq][1];
		yPixEne = GAME_HEIGHT - (_y - yPixOffset + hPixEne);
	}

	long long tiempoFlash = _tiempoVida - _ultimoImpacto;
	bool flash = _state == ENEMIGO_VIVE && tiempoFlash < JEFE3_MINI_FLASH_IMPACTO;
	if (flash) glColor3f(tiempoFlash % 3 == 0, tiempoFlash % 3 == 1, tiempoFlash % 3 == 2);
	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);
	if (flash) glColor3f(1, 1, 1);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}