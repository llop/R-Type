
#include "cNivel.h"


cNivel2::cNivel2(cSistema* sis, cNaveEspacial* naveEspacial, 
					int tilesAncho, int tilesAlto,
					const char* ficheroMapa, 
					int idTextura, int idFondo, 
					const char* ficheroTextura,
					const char* ficheroFondo) : cNivel(sis, naveEspacial, 
														tilesAncho, tilesAlto,
														ficheroMapa, 
														idTextura, idFondo, 
														ficheroTextura,
														ficheroFondo) {
	_posicion = 3900;
}


int cNivel2::avanzaPosicion() {
	// avanzar si toca
	int avanza = 0;
	if (_delay) --_delay;
	else { 
		avanza = NIVEL_AVANCE;
		_delay = NIVEL_DELAY;
	}
	return avanza;
}

void cNivel2::generaEnemigos() {
	cRect rect;
	caja(rect);
	// generar una cadena de malos
	int inter = 8;
	if (!(_posicion%inter) && !_delay && _posicion < inter * 6) {
		cEnemigo1* e = new cEnemigo1(_sis, rect.x + rect.w + 10, 200);
		pushEnemigo(e);
	}

	if (!(_posicion%inter) && !_delay && _posicion < (inter * 4)) { // cambiar posicio
		cEnemigo2* e = new cEnemigo2(_sis, rect.x + rect.w + 10, 100);
		pushEnemigo(e);
	}

	if (_posicion==200 && !_delay) {
		cEnemigoItem* e = new cEnemigoItem(_sis, rect.x + rect.w + 8, 200, -5.0f, 0.0f, ITEM_DISPARO_RB);
		pushEnemigo(e);
	}
	if (_posicion==300 && !_delay) {
		cEnemigoItem* e = new cEnemigoItem(_sis, rect.x + rect.w + 8, 200, -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(e);
	}
	if (_posicion==400 && !_delay) {
		cEnemigoItem* e = new cEnemigoItem(_sis, rect.x + rect.w + 8, 200, -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(e);
	}
	if (_posicion==500 && !_delay) {
		cEnemigoItem* e = new cEnemigoItem(_sis, rect.x + rect.w + 8, 200, -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(e);
	}

	// generar el jefe
	if (_posicion==3900 && !_delay) {
		cJefe2* jefe = new cJefe2(_sis);
		pushEnemigo(jefe);
	}
}

void cNivel2::posicionRespawn(int &x, int &y) {
	x = _posicion+100;
	y = 255;
}