#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>


int e2mov[16][4] = {
	{ 7, 7, 22, 22 },
	{ 38, 7, 27, 23 },
	{ 71, 7, 27, 23 },
	{ 105, 7, 25, 23 },
	{ 137, 7, 26, 21 },
	{ 170, 7, 25, 23 },
	{ 202, 7, 27, 23 },
	{ 235, 7, 27, 23 },
	{ 7, 40, 22, 22 },
	{ 37, 39, 27, 23 },
	{ 70, 39, 27, 23 },
	{ 104, 39, 25, 23 },
	{ 137, 41, 26, 21 },
	{ 171, 39, 25, 23 },
	{ 204, 39, 27, 23 },
	{ 236, 39, 27, 23 }
};
int e2movMid[2] = { 17, 50 };

int e2die[6][4] = {
	{ 264, 318, 18, 15 },
	{ 217, 300, 28, 20 },
	{ 182, 297, 32, 27 },
	{ 146, 296, 32, 29 },
	{ 109, 296, 32, 30 },
	{ 76, 296, 32, 30 }
};
int e2dieMid = 311;


cEnemigo2::cEnemigo2(cSistema* sis, int x, int y) : cEnemigo(sis, x, y) {
	_sis->cargaTextura(TEX_ENE2, "img\\r-typesheet8.png");
	_sis->cargaTextura(TEX_NAVE1, "img\\r-typesheet1.png");

	_angle = 0;
	_yBase = _y;

	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = ENEMIGO2_MUEVE_DELAY;

	_vida = ENEMIGO2_VIDA_INICIAL;
	_puntos = ENEMIGO2_PUNTOS;

	_esJefe = false;
}

cEnemigo2::~cEnemigo2() {

}

void cEnemigo2::muerete() {
	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_seq = 0;
	_delay = ENEMIGO2_MUERE_DELAY;
}

void cEnemigo2::caja(cRect &rect) const {
	rect.w = e2mov[_seq][2] - 2;
	rect.h = e2mov[_seq][3] - 2;
	rect.x = _x - (rect.w >> 1);
	rect.y = _y - (rect.h >> 1);
}

void cEnemigo2::colision(cRect &rect,int &colMask) const {
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

void cEnemigo2::logica() { // cambiar logica quan es vegi
	if (_state == ENEMIGO_VIVE) {

		// actualizar posicion
		_angle += ENEMIGO2_INC_ANGLE;
		_x -= ENEMIGO2_SPEED_LEFT;
		_y = int(_yBase + sin(_angle) * ENEMIGO2_ALT_MOV);

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

		// animacion
		if (_delay) --_delay;
		else {
			_seq = (_seq + 1) % ENEMIGO2_NUM_FRAMES;
			_delay = ENEMIGO2_MUEVE_DELAY;
		}

		//IA DISPAROS
		int auxRandom = rand() % 250;
		if (auxRandom == 1) {
			// meter el nuevo disparo en el nivel
			int nX, nY;
			nave->getPosicion(nX, nY);
			float vectX = float(nX - _x);
			float vectY = float(nY - _y);
			float len = sqrt(vectX*vectX + vectY*vectY);
			vectX /= len;
			vectY /= len;
			nivel->pushDisparo(new cDisparoEnemigo(_sis, _x, _y, vectX, vectY));
		}
	}
	else if (_state == ENEMIGO_EXPLO) {
		// avanzar en la animacion
		if (_delay) {
			--_delay;
		}
		else {
			++_seq;
			_delay = ENEMIGO2_MUERE_DELAY;
		}
		if (_seq == ENEMIGO2_EXPLO_FRAMES) {
			_state = ENEMIGO_MUERE;
			_muerto = true;
		}
	}
}

void cEnemigo2::pinta() const {
	if (_state == ENEMIGO_MUERE) return;

	float xTexEne, yTexEne, wTexEne, hTexEne;
	int xPixEne, yPixEne, yPixOffset, wPixEne, hPixEne;

	if (_state == ENEMIGO_VIVE) {
		int tex = _sis->idTextura(TEX_ENE2);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_ENE2, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexEne = e2mov[_seq][0] / (float)wTex;
		yTexEne = e2mov[_seq][1] / (float)hTex;
		wTexEne = e2mov[_seq][2] / (float)wTex;
		hTexEne = e2mov[_seq][3] / (float)hTex;
		wPixEne = e2mov[_seq][2];
		hPixEne = e2mov[_seq][3];
		xPixEne = _x - (wPixEne >> 1);
		yPixOffset = e2movMid[_seq/8] - e2mov[_seq][1];
		yPixEne = GAME_HEIGHT - (_y - yPixOffset + hPixEne);
	}
	else if (_state == ENEMIGO_EXPLO) {
		int tex = _sis->idTextura(TEX_NAVE1);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_NAVE1, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexEne = e2die[_seq][0] / (float)wTex;
		yTexEne = e2die[_seq][1] / (float)hTex;
		wTexEne = e2die[_seq][2] / (float)wTex;
		hTexEne = e2die[_seq][3] / (float)hTex;
		wPixEne = e2die[_seq][2];
		hPixEne = e2die[_seq][3];
		xPixEne = _x - (wPixEne >> 1);
		yPixOffset = e2dieMid - e2die[_seq][1];
		yPixEne = GAME_HEIGHT - (_y - yPixOffset + hPixEne);
	}

	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

