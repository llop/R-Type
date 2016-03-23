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
int e2movMid = 17;

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
	_sis->cargaTextura(TEX_ENE1, "img\\r-typesheet8.png");

	_angle = 0;
	_yBase = _y;

	_state = ENEMIGO_VIVE;
	_seq = 0;
	_delay = ENEMIGO2_MUEVE_DELAY;

	_vida = ENEMIGO2_VIDA_INICIAL;
}

cEnemigo2::~cEnemigo2() {

}


void cEnemigo2::caja(cRect &rect) const {

}

void cEnemigo2::colision(cRect &caja,int &colMask) const {

}

void cEnemigo2::logica() {

}

void cEnemigo2::pinta() const {

}

