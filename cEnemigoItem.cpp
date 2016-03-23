#include "cNivel.h"
#include "cEnemigo.h"
#include <gl/glut.h>

int eneItemMov[7][4] = {
	{ 13, 6, 23, 22 },
	{ 57, 7, 27, 32 },
	{ 104, 8, 22, 22 },
	{ 143, 6, 26, 26 },
	{ 182, 6, 22, 27 },
	{ 219, 8, 26, 26 },
	{ 260, 8, 23, 22 }
};
int eneItemMovMid = 15;

int eneItemDie[6][4] = {
	{ 264, 318, 18, 15 },
	{ 217, 300, 28, 20 },
	{ 182, 297, 32, 27 },
	{ 146, 296, 32, 29 },
	{ 109, 296, 32, 30 },
	{ 76, 296, 32, 30 }
};
int eneItemDieMid = 311;


cEnemigoItem::cEnemigoItem(cSistema* sis, int x, int y, 
							float xVec, float yVec, 
							int item) : cEnemigo(sis, x, y) {
	_sis->cargaTextura(TEX_ENE_ITEM, "img\\item-enemy.png");
	_sis->cargaTextura(TEX_NAVE1, "img\\r-typesheet1.png");
	
	_state = ENEMIGO_VIVE;
	_subState = ENEMIGO_ITEM_VUELA;
	_seq = 0;
	_delay = ENEMIGO_ITEM_MUEVE_DELAY;
	
	_vida = ENEMIGO_ITEM_VIDA_INICIAL;
	_puntos = ENEMIGO_ITEM_PUNTOS;

	_xF = float(_x);
	_yF = float(_y);
	_xVec = xVec;
	_yVec = yVec;
	float f = sqrt(_xVec*_xVec + _yVec*_yVec);
	_xVec /= f;
	_yVec /= f;
	_xVec *= ENEMIGO_ITEM_AVANCE;
	_yVec *= ENEMIGO_ITEM_AVANCE;
	_ava = -ENEMIGO_ITEM_AVANCE;

	_item = item;
}

cEnemigoItem::~cEnemigoItem() {
	
}

void cEnemigoItem::offset(int x, int y) {
	_xF += x;
	_yF += y;
}

void cEnemigoItem::muerete() {
	// soltar el regalito
	cRect myRect;
	caja(myRect);
	int xItem = myRect.x+(myRect.w>>1);
	int yItem = myRect.y+(myRect.h>>1);
	cItem* item = new cItem(_sis, xItem, yItem, _item);
	((cNivel*)_sis->nivel())->pushItem(item);

	_vida = 0;
	_state = ENEMIGO_EXPLO;
	_seq = 0;
	_delay = ENEMIGO_ITEM_MUERE_DELAY;
}


void cEnemigoItem::caja(cRect &rect) const {
	int seq = _seq;
	if (_subState == ENEMIGO_ITEM_ANDA) seq += ENEMIGO_ITEM_VUELA_NUM_FRAMES;
	rect.w = eneItemMov[seq][2] - 2;
	rect.h = eneItemMov[seq][3] - 2;
	rect.x = int(_xF + 1);
	rect.y = int(_yF + 1);
}

void cEnemigoItem::colision(cRect &rect, int &colMask) const {
	cRect myRect;
	caja(myRect);
	colMask = 0;
	if (_state == ENEMIGO_VIVE) {
		if (myRect.x < rect.x+rect.w && myRect.x+myRect.w > rect.x &&
			myRect.y < rect.y+rect.h && myRect.y+myRect.h > rect.y) {
				colMask = 1;
		}
	}
}

void cEnemigoItem::logica() {
	if (_state == ENEMIGO_VIVE) {

		// actualizar posicion
		if (_subState == ENEMIGO_ITEM_ANDA) {
			_xF += _ava;
		} else if (_subState == ENEMIGO_ITEM_VUELA) {
			_xF += _xVec;
			_yF += _yVec;
		}

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
			if (!disparo->malo()) {
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
		list<cEscudo*> escudos = nivel->escudos();
		for (list<cEscudo*>::iterator it = escudos.begin(); it != escudos.end(); ++it) {
			cEscudo* escudo = *it;
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
		if (_subState == ENEMIGO_ITEM_ANDA) {
			_xF += _ava;
		} else if (_subState == ENEMIGO_ITEM_VUELA) {
			_xF += _xVec;
			_yF += _yVec;
		}

		if (_subState == ENEMIGO_ITEM_ANDA) {
			if (_delay) --_delay;
			else {
				_seq = (_seq + 1) % ENEMIGO_ITEM_ANDA_NUM_FRAMES;
				_delay = ENEMIGO_ITEM_MUEVE_DELAY;
			}
		} else if (_subState == ENEMIGO_ITEM_VUELA) {
			if (_delay) --_delay;
			else {
				_seq = (_seq + 1) % ENEMIGO_ITEM_VUELA_NUM_FRAMES;
				_delay = ENEMIGO_ITEM_MUEVE_DELAY;
			}
		}
		

		//IA DISPAROS
		/*
		int auxRandom = rand() % 1000;
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
		*/
	} else if (_state == ENEMIGO_EXPLO) {
		// avanzar en la animacion
		if (_delay) {
			--_delay;
		} else {
			++_seq;
			_delay = ENEMIGO_ITEM_MUERE_DELAY;
		}
		if (_seq == ENEMIGO1_EXPLO_FRAMES) {
			_state = ENEMIGO_MUERE;
			_muerto = true;
		}
	}
}

void cEnemigoItem::pinta() const{
	if (_state == ENEMIGO_MUERE) return;
	
	float xTexEne, yTexEne, wTexEne, hTexEne;
	int xPixEne, yPixEne, wPixEne, hPixEne;
	int wTex, hTex;

	if (_state == ENEMIGO_VIVE) {
		int tex = _sis->idTextura(TEX_ENE_ITEM);
		_sis->tamanoTextura(TEX_ENE_ITEM, wTex, hTex);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		int seq = _seq;
		if (_subState == ENEMIGO_ITEM_ANDA) seq += ENEMIGO_ITEM_VUELA_NUM_FRAMES;

		xTexEne = eneItemMov[seq][0] / (float)wTex;
		yTexEne = eneItemMov[seq][1] / (float)hTex;
		wTexEne = eneItemMov[seq][2] / (float)wTex;
		hTexEne = eneItemMov[seq][3] / (float)hTex;
		wPixEne = eneItemMov[seq][2];
		hPixEne = eneItemMov[seq][3]; 
		xPixEne = int(_xF);
		yPixEne = int(GAME_HEIGHT - _yF - hPixEne);
	} else if (_state == ENEMIGO_EXPLO) {
		int tex = _sis->idTextura(TEX_NAVE1);
		_sis->tamanoTextura(TEX_NAVE1, wTex, hTex);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);

		xTexEne = eneItemDie[_seq][0] / (float)wTex;
		yTexEne = eneItemDie[_seq][1] / (float)hTex;
		wTexEne = eneItemDie[_seq][2] / (float)wTex;
		hTexEne = eneItemDie[_seq][3] / (float)hTex;
		wPixEne = eneItemDie[_seq][2];
		hPixEne = eneItemDie[_seq][3];
		xPixEne = int(_xF);
		yPixEne = int(GAME_HEIGHT - _yF - hPixEne);
	}

	glTexCoord2f(xTexEne, yTexEne + hTexEne);			glVertex2i(xPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne + hTexEne);	glVertex2i(xPixEne + wPixEne, yPixEne);
	glTexCoord2f(xTexEne + wTexEne, yTexEne);			glVertex2i(xPixEne + wPixEne, yPixEne + hPixEne);
	glTexCoord2f(xTexEne, yTexEne);						glVertex2i(xPixEne, yPixEne + hPixEne);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
