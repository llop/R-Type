
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
	int iniPos = 40 * TILE_WIDTH - GAME_WIDTH;
	// primer enemigo1
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, 300);
		pushEnemigo(enemigo);
	}

	//enemigo anda izq
	iniPos = 45 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10, GAME_HEIGHT - (4 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}

	// 8 enemigos 1 altura random
	iniPos = 60 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 8)) {
		int numRand = rand() % 150;
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, 150 + numRand);
		pushEnemigo(enemigo);
	}
	
	//tiras enemigos 2
	iniPos = 70 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 5)) {
		cEnemigo2* enemigo2 = new cEnemigo2(_sis, rect.x + rect.w + 10, 150);
		pushEnemigo(enemigo2);
	}

	//tiras enemigos 1
	iniPos = 75 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 5)) {
		cEnemigo1* enemigo1 = new cEnemigo1(_sis, rect.x + rect.w + 10, 250);
		pushEnemigo(enemigo1);
	}

	//serpiente envuelve bloque
	iniPos = 88 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo3* enemigo3 = new cEnemigo3(_sis, 1494, 292, 6, 100.0f, 0.0f, 0.005f, false);
		pushEnemigo(enemigo3);
	}
	//escudo
	if (_posicion == 110 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 100, -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(enemigo);
	}

	//enemigo1 vertical
	if (_posicion == 130 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		int y = 100;
		for (int i = 0; i < 5; ++i) {
			cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y += 32);
			pushEnemigo(enemigo);
		}
	}
	
	//enemigo q persigue
	if (_posicion == 160 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo5* enemigo1 = new cEnemigo5(_sis, rect.x + rect.w + 8, 110);
		cEnemigo5* enemigo2 = new cEnemigo5(_sis, rect.x + rect.w + 8, 300);
		pushEnemigo(enemigo1);
		pushEnemigo(enemigo2);

	}

	//camina izq arriba
	iniPos = 180 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10, GAME_HEIGHT - (20 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}

	iniPos = 185 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10, GAME_HEIGHT - (20 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}

	iniPos = 190 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10, GAME_HEIGHT - (20 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}

	//tira enemigos2 abajo
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 5)) {
		cEnemigo2* enemigo2 = new cEnemigo2(_sis, rect.x + rect.w + 10, 300);
		pushEnemigo(enemigo2);
	}

	//disparo arriba
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 100, -5.0f, 0.0f, ITEM_DISPARO_RB);
		pushEnemigo(enemigo);
	}


	//camina izq arriba
	iniPos = 215 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10, GAME_HEIGHT - (16 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}
	iniPos = 220 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10, GAME_HEIGHT - (16 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}
	iniPos = 225 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10, GAME_HEIGHT - (16 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}

	// enemigos 1 abajo
	iniPos = 225 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 8)) {
		int numRand = rand() % 75;
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, 325 + numRand);
		pushEnemigo(enemigo);
	}

	//escudo abajo
	iniPos = 240 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 330, -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(enemigo);
	}



	//enemigo1 en flecha invertida

	if (_posicion == 288 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) - 64);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) + 64);
		pushEnemigo(enemigo);
	}
	else if (_posicion == 289 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) - 48);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) + 48);
		pushEnemigo(enemigo);
	}
	else if (_posicion == 290 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) - 32);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) + 32);
		pushEnemigo(enemigo);
	}
	else if (_posicion == 291 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) - 16);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) + 16);
		pushEnemigo(enemigo);
	}
	else if (_posicion == 292 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, GAME_HEIGHT >> 1);
		pushEnemigo(enemigo);
	}

	iniPos = 294 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 1)) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 350, -5.0f, 0.0f, ITEM_ESCUDO);
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