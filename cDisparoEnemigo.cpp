
#include "cDisparo.h"
#include "cNivel.h"
#include <gl/glut.h>


#include "cDisparo.h"


int tiroEnemigo[4][4] = {
	{ 212, 279, 6, 6 },
	{ 227, 278, 10, 8 },
	{ 243, 277, 12, 10 },
	{ 272, 287, 14, 12 }
};
int tiroEnemigoMid = 281;


cDisparoEnemigo::cDisparoEnemigo(cSistema* sis, int x, int y, float xu, float yu) : cDisparo(sis, x, y) {
	_dano = DANO_DISPARO_ENEMIGO;

	_seq = 0;
	_delay = DISPARO_ENEMIGO_DELAY;

	_pixelsAvanzaX = xu;
	_pixelsAvanzaY = yu;
}

cDisparoEnemigo::~cDisparoEnemigo() {
}

void cDisparoEnemigo::getCaja(cRect &rect) const {
	rect.w = tiroEnemigo[_seq][2];
	rect.h = tiroEnemigo[_seq][3];
	rect.x = _x;
	rect.y = _y;
}

void cDisparoEnemigo::logica() {
	cNivel* nivel = ((cNivel*)_sis->getNivel());
	_x += _pixelsAvanzaX*VELOCIDAD_DISPARO_ENEMIGO;
	_y += _pixelsAvanzaY*VELOCIDAD_DISPARO_ENEMIGO;
	cRect caja;
	getCaja(caja);
	if (nivel->fueraLimites(caja)) {
		_muerto = true;
		return;
	}
}

void cDisparoEnemigo::pinta() const {
	int tex = _sis->getIdTextura(TEX_ENE1);
	int wTex, hTex;
	_sis->getTamanoTextura(TEX_ENE1, wTex, hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);

	float xTexTiro, yTexTiro, wTexTiro, hTexTiro;
	int xPixTiro, yPixTiro, yPixOffsetTiro, wPixTiro, hPixTiro;

	xTexTiro = tiroEnemigo[_seq][0] / (float)wTex;
	yTexTiro = tiroEnemigo[_seq][1] / (float)hTex;
	wTexTiro = tiroEnemigo[_seq][2] / (float)wTex;
	hTexTiro = tiroEnemigo[_seq][3] / (float)hTex;
	wPixTiro = tiroEnemigo[_seq][2];
	hPixTiro = tiroEnemigo[_seq][3];
	xPixTiro = _x;
	yPixOffsetTiro = tiroEnemigoMid - tiroEnemigo[_seq][1];
	yPixTiro = GAME_HEIGHT - (_y - yPixOffsetTiro + hPixTiro);

	glTexCoord2f(xTexTiro, yTexTiro + hTexTiro);			glVertex2i(xPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro + hTexTiro);	glVertex2i(xPixTiro + wPixTiro, yPixTiro);
	glTexCoord2f(xTexTiro + wTexTiro, yTexTiro);			glVertex2i(xPixTiro + wPixTiro, yPixTiro + hPixTiro);
	glTexCoord2f(xTexTiro, yTexTiro);						glVertex2i(xPixTiro, yPixTiro + hPixTiro);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}


