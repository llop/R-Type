#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>


int ene5Mov[32][4] = {
	{ 204, 136, 26, 27 },
	{ 240, 136, 23, 27 },
	{ 168, 171, 31, 22 },
	{ 201, 170, 31, 23 },
	{ 139, 201, 25, 28 },
	{ 174, 201, 25, 28 },
	{ 102, 233, 26, 29 },
	{ 138, 233, 26, 29},
	{ 102, 201, 25, 28 },
	{ 67, 201, 25, 28 },
	{ 67, 171, 31, 22 },
	{ 34, 170, 31, 23 },
	{ 36, 136, 26, 27 },
	{ 3, 136, 23, 27 },
	{ 36, 102, 26, 27 },
	{ 3, 102, 23, 27 },
	{ 67, 72, 31, 22 },
	{ 34, 72, 31, 23 },
	{ 102, 36, 25, 28 },
	{ 67, 36, 25, 28 },
	{ 138, 3, 26, 29 },
	{ 102, 3, 26, 29 },
	{ 139, 36, 25, 28 },
	{ 174, 36, 25, 28 },
	{ 168, 72, 31, 22 },
	{ 201, 72, 31, 23 },
	{ 204, 102, 26, 27 },
	{ 240, 102, 23, 27 }
};

int ene5Explo[6][4] = {
	{ 202, 266,	30, 28 },
	{ 168, 265, 31, 32 },
	{ 134, 265, 32, 32 },
	{ 102, 266, 31, 30 },
	{ 69, 266, 31, 30 },
	{ 36, 265, 31, 31 }
};


cEnemigo5::cEnemigo5(cSistema* sis, int x, int y) : cEnemigo(sis, x, y) {
	_sis->cargaTextura(TEX_ENE5, "img\\r-typesheet29.png");

	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = ENEMIGO5_MUERE_DELAY;

	_vida = _sis->dificultad() == DIFICULTAD_NORMAL ? ENEMIGO5_VIDA_INICIAL : ENEMIGO5_VIDA_INICIAL_HARD;
	_vida = int(_vida*((cNivel*)_sis->nivel())->factorDificultad());
	_puntos = ENEMIGO5_PUNTOS;

	_esJefe = false;

	_angulo = 0.0f;
}

cEnemigo5::~cEnemigo5() {

}

void cEnemigo5::muerete() {
	_sis->playSonido(SOUND_EXPLO2);

	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_seq = 0;
	_delay = ENEMIGO1_MUERE_DELAY;
}
void cEnemigo5::caja(cRect &rect) const {
	int seq = int(ENEMIGO5_NUM_ANGULOS * _angulo / _2PI);
	seq = (seq - 1 + ENEMIGO5_NUM_ANGULOS) % ENEMIGO5_NUM_ANGULOS;
	seq = ENEMIGO5_MOV_FRAMES * seq + _seq;
	rect.w = ene5Mov[seq][2] - 2;
	rect.h = ene5Mov[seq][3] - 2;
	rect.x = _x - (rect.w >> 1);
	rect.y = _y - (rect.h >> 1);
}

void cEnemigo5::colision(cRect &rect, int &colMask) const {
	colMask = 0;
	if (_state == ENEMIGO_VIVE) {
		cRect myRect;
		caja(myRect);
		if (myRect.x < rect.x + rect.w && myRect.x + myRect.w > rect.x &&
			myRect.y < rect.y + rect.h && myRect.y + myRect.h > rect.y) {
			colMask = 1;
		}
	}
}

float _E5_2PI = float(2 * acos(-1.0f));
inline float corrAngEne5(float ang) {
	while (ang<0) ang += _E5_2PI;
	while (ang >= _E5_2PI) ang -= _E5_2PI;
	return ang;
}

void cEnemigo5::logica() {
	if (_state == ENEMIGO_VIVE) {

		// estos los necesitaremos por ahi
		cNivel* nivel = (cNivel*)_sis->nivel();
		cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();

		// recalcula angulo
		int xNave, yNave;
		nave->getPosicion(xNave, yNave);
		float xVec = float(xNave-_x);
		float yVec = float(yNave-_y);
		_angulo = corrAngEne5(atan(yVec/xVec));
		if (yVec <= 0.0f) _angulo = corrAngEne5(_angulo+(_2PI/ENEMIGO5_NUM_ANGULOS));
		if (xVec <= 0.0f) _angulo = corrAngEne5(_angulo+_PI);

		// avanza
		float dist = sqrt(xVec*xVec + yVec*yVec);
		xVec /= dist;
		yVec /= dist;
		float avance = float(_sis->dificultad()==DIFICULTAD_NORMAL ? ENEMIGO5_AVANCE : ENEMIGO5_AVANCE_HARD);
		avance *= nivel->factorDificultad();
		_x = int(_x+xVec*avance);
		_y = int(_y+yVec*avance);

		// matar el enemigo si queda fuera de la pantalla
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
					else _sis->playSonido(SOUND_ENEMIGO_HIT);
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
				else _sis->playSonido(SOUND_ENEMIGO_HIT);
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

		// animacion
		if (_delay) --_delay;
		else {
			_seq = (_seq + 1) % ENEMIGO5_MOV_FRAMES;
			_delay = ENEMIGO5_MUEVE_DELAY;
		}
	}
	else if (_state == ENEMIGO_EXPLO) {
		// avanzar en la animacion
		if (_delay) {
			--_delay;
		}
		else {
			++_seq;
			_delay = ENEMIGO5_MUERE_DELAY;
		}
		if (_seq == ENEMIGO5_EXPLO_FRAMES) {
			_state = ENEMIGO_MUERE;
			_muerto = true;
		}
	}
}

void cEnemigo5::pinta() const {
	if (_state == ENEMIGO_MUERE) return;

	float xTexEne, yTexEne, wTexEne, hTexEne;
	int xPixEne, yPixEne, wPixEne, hPixEne;

	int tex = _sis->idTextura(TEX_ENE5);
	int wTex, hTex;
	_sis->tamanoTextura(TEX_ENE5, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	if (_state == ENEMIGO_VIVE) {
		int seq = int(ENEMIGO5_NUM_ANGULOS * _angulo / _2PI);
		seq = (seq-1+ ENEMIGO5_NUM_ANGULOS)% ENEMIGO5_NUM_ANGULOS;
		seq = ENEMIGO5_MOV_FRAMES * seq + _seq;
		wPixEne = ene5Mov[seq][2];
		hPixEne = ene5Mov[seq][3];
		xTexEne = ene5Mov[seq][0] / float(wTex);
		yTexEne = ene5Mov[seq][1] / float(hTex);
		wTexEne = wPixEne / float(wTex);
		hTexEne = hPixEne / float(hTex);
		xPixEne = _x - (wPixEne >> 1);
		yPixEne = GAME_HEIGHT - (_y + (hPixEne>>1));
	} else if (_state == ENEMIGO_EXPLO) {
		wPixEne = ene5Explo[_seq][2];
		hPixEne = ene5Explo[_seq][3];
		xTexEne = ene5Explo[_seq][0] / float(wTex);
		yTexEne = ene5Explo[_seq][1] / float(hTex);
		wTexEne = wPixEne / float(wTex);
		hTexEne = hPixEne / float(hTex);
		xPixEne = _x - (wPixEne >> 1);
		yPixEne = GAME_HEIGHT - (_y + (hPixEne>>1));
	}

	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}