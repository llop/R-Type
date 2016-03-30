
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
														ficheroFondo) {
	//_posicion = 4200;//3217;
}


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
	if (!(_posicion%inter) && !_delay && _posicion < inter * 6) {
		int alturaRandom = rand() % 400;
		cEnemigo1* e = new cEnemigo1(_sis, rect.x + rect.w + 10, alturaRandom);
		pushEnemigo(e);
	}

	if (_posicion >= 800 && !(_posicion%inter) && !_delay && _posicion < 800+(inter * 4)) {
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

	// enemigo 3 (251, 14) -> (4016, 224), radio (14-5) -> 144, spawnPos = (4016-144-15-640) = 3217
	if (_posicion==3217 && !_delay) {
		cEnemigo3* enemigo3 = new cEnemigo3(_sis, 4016, 224, 10, 144.0f, 0.0f, 0.005f, false);
		pushEnemigo(enemigo3);
		
		enemigo3 = new cEnemigo3(_sis, 4016, 224, 10, 144.0f, acos(-1.0f), 0.005f, false);
		pushEnemigo(enemigo3);
	}

	// generar el jefe
	if (_posicion==4200 && !_delay) {
		cJefe1* jefe = new cJefe1(_sis);
		pushEnemigo(jefe);
	}
}

void cNivel1::posicionRespawn(int &x, int &y) {
	x = _posicion+100;
	y = 255;
}