
#include "cNivel.h"


cNivel1::cNivel1(cSistema* sis, cNaveEspacial* naveEspacial, 
					int tilesAncho, int tilesAlto,
					const char* ficheroMapa, 
					int idTextura, int idFondo, 
					const char* ficheroTextura,
					const char* ficheroFondo) : cNivel(sis, naveEspacial, 
														tilesAncho, tilesAlto,
														ficheroMapa, 
														idTextura, idFondo, 
														ficheroTextura,
														ficheroFondo) {}


int cNivel1::avanzaPosicion() {
	// avanzar si toca
	int avanza = 0;
	if (_delay) --_delay;
	else { 
		avanza = NIVEL_AVANCE;
		_delay = NIVEL_DELAY;
	}
	return avanza;
}

void cNivel1::generaEnemigos() {
	cRect rect;
	caja(rect);
	// generar una cadena de malos
	int inter = 8;
	if (!(_posicion%inter) && !_delay && _posicion<inter*6) {
		cEnemigo1* e = new cEnemigo1(_sis, rect.x + rect.w + 10, 200);
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
}

void cNivel1::trataColisiones() {
	// la nave

	// los enemigos

	// los disparos

	// los items pueden atravesar paredes!
}
