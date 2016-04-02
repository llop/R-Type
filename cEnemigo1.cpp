#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>

int e1mov[8][4] = {
	{ 5, 6, 21, 24 },
	{ 38, 12, 21, 14 },
	{ 71, 16, 21, 7 },
	{ 104, 10, 21, 14 },
	{ 137, 6, 21, 24 },
	{ 170, 12, 21, 14 },
	{ 203, 13, 21, 7 },
	{ 236, 10, 21, 14 }
};
int e1movMid = 17;

int e1die[6][4] = {
	{ 264, 318, 18, 15 },
	{ 217, 300, 28, 20 },
	{ 182, 297, 32, 27 },
	{ 146, 296, 32, 29 },
	{ 109, 296, 32, 30 },
	{ 76, 296, 32, 30 }
};
int e1dieMid = 311;


cEnemigo1::cEnemigo1(cSistema* sis, int x, int y) : cEnemigo(sis, x, y) {
	_sis->cargaTextura(TEX_ENE1, "img\\r-typesheet5.png");
	_sis->cargaTextura(TEX_NAVE1, "img\\r-typesheet1.png");
	
	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = ENEMIGO1_MUEVE_DELAY;
	
	_vida = _sis->dificultad()==DIFICULTAD_NORMAL ? ENEMIGO1_VIDA_INICIAL : ENEMIGO1_VIDA_INICIAL_HARD;
	_vida = int(_vida*((cNivel*)_sis->nivel())->factorDificultad());
 	_puntos = ENEMIGO1_PUNTOS;

	_esJefe = false;
}

cEnemigo1::~cEnemigo1(){
	
}

void cEnemigo1::muerete() {
	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_seq = 0;
	_delay = ENEMIGO1_MUERE_DELAY;
}


void cEnemigo1::caja(cRect &rect) const {
	rect.w = e1mov[_seq][2] - 2;
	rect.h = e1mov[_seq][3] - 2;
	rect.x = _x - (rect.w>>1);
	rect.y = _y - (rect.h>>1);
}

void cEnemigo1::colision(cRect &rect, int &colMask) const {
	colMask = 0;
	if (_state == ENEMIGO_VIVE) {
		cRect myRect;
		caja(myRect);
		if (myRect.x < rect.x+rect.w && myRect.x+myRect.w > rect.x &&
			myRect.y < rect.y+rect.h && myRect.y+myRect.h > rect.y) {
				colMask = 1;
		}
	}
}

void cEnemigo1::logica() {
	if (_state == ENEMIGO_VIVE) {

		_x -= ENEMIGO1_SPEED_LEFT;
		
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
			_seq = (_seq + 1) % ENEMIGO1_NUM_FRAMES;
			_delay = ENEMIGO1_MUEVE_DELAY;
		}

		//IA DISPAROS
		int modulo = _sis->dificultad() == DIFICULTAD_NORMAL ? ENEMIGO1_DISPARO : ENEMIGO1_DISPARO_HARD;
		modulo = int(modulo/nivel->factorDificultad());
		if (!(rand()%modulo)) {
			// meter el nuevo disparo en el nivel
			int nX, nY;
			nave->getPosicion(nX, nY);
			float vectX = float(nX - _x);
			float vectY = float(nY - _y);
			float len = sqrt(vectX*vectX + vectY*vectY);
			nivel->pushDisparo(new cDisparoEnemigo(_sis, _x, _y, vectX/len, vectY/len));
		}
	} else if (_state == ENEMIGO_EXPLO) {
		// avanzar en la animacion
		if (_delay) {
			--_delay;
		} else {
			++_seq;
			_delay = ENEMIGO1_MUERE_DELAY;
		}
		if (_seq == ENEMIGO1_EXPLO_FRAMES) {
			_state = ENEMIGO_MUERE;
			_muerto = true;
		}
	}
}

void cEnemigo1::pinta() const{
	if (_state == ENEMIGO_MUERE) return;
	
	float xTexEne, yTexEne, wTexEne, hTexEne;
	int xPixEne, yPixEne, yPixOffset, wPixEne, hPixEne;

	if (_state == ENEMIGO_VIVE) {
		int tex = _sis->idTextura(TEX_ENE1);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_ENE1, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		wPixEne = e1mov[_seq][2];
		hPixEne = e1mov[_seq][3];
		xTexEne = e1mov[_seq][0] / (float)wTex;
		yTexEne = e1mov[_seq][1] / (float)hTex;
		wTexEne = wPixEne / (float)wTex;
		hTexEne = hPixEne / (float)hTex;
		xPixEne = _x - (wPixEne >> 1);
		yPixOffset = e1movMid - e1mov[_seq][1];
		yPixEne = GAME_HEIGHT - (_y - yPixOffset + hPixEne);
	} else if (_state == ENEMIGO_EXPLO) {
		int tex = _sis->idTextura(TEX_NAVE1);
		int wTex, hTex;
		_sis->tamanoTextura(TEX_NAVE1, wTex, hTex);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		wPixEne = e1die[_seq][2];
		hPixEne = e1die[_seq][3];
		xTexEne = e1die[_seq][0] / (float)wTex;
		yTexEne = e1die[_seq][1] / (float)hTex;
		wTexEne = wPixEne / (float)wTex;
		hTexEne = hPixEne / (float)hTex;
		xPixEne = _x - (wPixEne >> 1);
		yPixOffset = e1dieMid - e1die[_seq][1];
		yPixEne = GAME_HEIGHT - (_y - yPixOffset + hPixEne);
	}

	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
