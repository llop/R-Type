
#include "cNivel.h"


cNivel3::cNivel3(cSistema* sis, cNaveEspacial* naveEspacial,
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
	//_posicion = 5084-GAME_WIDTH;
}

float cNivel3::factorDificultad() const {
	float factor = _posicion / float(_posicionFinal);
	return 3 * (factor / NUM_NIVELES) + 1;
};


int cNivel3::avanzaPosicion() {
	// avanzar si toca
	int avanza = 0;
	if (_delay) --_delay;
	else {
		avanza = NIVEL_AVANCE;
		_delay = NIVEL_DELAY;
	}
	return avanza;
}

void cNivel3::generaEnemigos() {
	cRect rect;
	caja(rect);
	// generar una cadena de malos
	int inter = 10;
	int iniPos = 50 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, 200);
		pushEnemigo(enemigo);
	}

	iniPos = 60 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 3)) {
		int numRand = rand() % 200;
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, 100 + numRand);
		pushEnemigo(enemigo);
	}

	/*iniPos = 90 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 5)) {
	cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 200);
	pushEnemigo(enemigo);
	}*/

	iniPos = 90 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 4)) {
		cEnemigo1* enemigo1 = new cEnemigo1(_sis, rect.x + rect.w + 10, 250);
		cEnemigo2* enemigo2 = new cEnemigo2(_sis, rect.x + rect.w + 10, 100);
		pushEnemigo(enemigo1);
		pushEnemigo(enemigo2);
	}

	if (_posicion == 110 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 300, -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(enemigo);
	}

	if (_posicion == 130 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		int y = 100;
		for (int i = 0; i < 5; ++i) {
			cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y += 32);
			pushEnemigo(enemigo);
		}
	}
	
	if (_posicion == 150 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo5* enemigo = new cEnemigo5(_sis, rect.x + rect.w + 8, 200);
		pushEnemigo(enemigo);
	}

	// generar el jefe
	if (_posicion == 5084 - GAME_WIDTH && !_delay) {
		cJefe3* jefe = new cJefe3(_sis);
		pushEnemigo(jefe);
	}
}

void cNivel3::posicionRespawn(int &x, int &y) {
	x = _posicion + 100;
	y = 244;
}