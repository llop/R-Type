

#include "cNivel.h"

// fmod, openal
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
	//_posicion = 4200-640;//3218;
}


float cNivel1::factorDificultad() const {
	float factor = _posicion / float(_posicionFinal);
	return (factor / NUM_NIVELES) + 1;
};

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
	//int inter = 8;
	//if (!(_posicion%inter) && !_delay && _posicion < inter * 6) {
	//	int alturaRandom = rand() % 400;
	//	cEnemigo1* e = new cEnemigo1(_sis, rect.x + rect.w + 10, alturaRandom);
	//	pushEnemigo(e);
	//}

	//if (_posicion >= 800 && !(_posicion%inter) && !_delay && _posicion < 800+(inter * 4)) {
	//	cEnemigo2* e = new cEnemigo2(_sis, rect.x + rect.w + 10, 100);
	//	pushEnemigo(e);
	//}

	//if (_posicion==200 && !_delay) {
	//	cEnemigoItem* e = new cEnemigoItem(_sis, rect.x + rect.w + 8, 200, -5.0f, 0.0f, ITEM_DISPARO_RB);
	//	pushEnemigo(e);
	//}
	//if (_posicion==300 && !_delay) {
	//	cEnemigoItem* e = new cEnemigoItem(_sis, rect.x + rect.w + 8, 200, -5.0f, 0.0f, ITEM_ESCUDO);
	//	pushEnemigo(e);
	//}
	//if (_posicion==400 && !_delay) {
	//	cEnemigoItem* e = new cEnemigoItem(_sis, rect.x + rect.w + 8, 200, -5.0f, 0.0f, ITEM_ESCUDO);
	//	pushEnemigo(e);
	//}
	//if (_posicion==500 && !_delay) {
	//	cEnemigoItem* e = new cEnemigoItem(_sis, rect.x + rect.w + 8, 200, -5.0f, 0.0f, ITEM_ESCUDO);
	//	pushEnemigo(e);
	//}

	if (_posicion == 42 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo5* enemigo = new cEnemigo5(_sis, rect.x + rect.w + 8, 300);
		pushEnemigo(enemigo);
	}

	int inter = 8;
	int iniPos = 50 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos+(inter*8)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 120);
		pushEnemigo(enemigo);
	}
	
	if (_posicion == 60 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 300, -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(enemigo);
	}

	if (_posicion == 68 * TILE_WIDTH - GAME_WIDTH + 64 && !_delay) {
		int y = -16;
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10 - 64, y, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}
	if (_posicion == 68 * TILE_WIDTH - GAME_WIDTH + 72 && !_delay) {
		int y = -16;
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10 - 72, y, ENEMIGO4_ANDA_DER);
		pushEnemigo(enemigo);
	}
	if (_posicion == 92 * TILE_WIDTH - 100 && !_delay) {
		int y = -16;
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + 100, y, ENEMIGO4_ANDA_DER);
		pushEnemigo(enemigo);
	}

	if (_posicion == 75 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		int y = 16;
		for (int i = 0; i < 10; ++i) {
			cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y += 32);
			pushEnemigo(enemigo);
		}
	}

	if (_posicion == 138 * TILE_WIDTH - GAME_WIDTH + 64 && !_delay) {
		int y = -16;
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10 - 64, y, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}


	// enemigo 3 (251, 14) -> (4016, 224), radio (14-5) -> 144, spawnPos = (4016-144-15-640) = 3217
	if (_posicion==3218 && !_delay) {
		cEnemigo3* enemigo3 = new cEnemigo3(_sis, 4016, 224, 10, 144.0f, 0.0f, 0.005f, false);
		pushEnemigo(enemigo3);

		enemigo3 = new cEnemigo3(_sis, 4016, 224, 10, 144.0f, acos(-1.0f), 0.005f, false);
		pushEnemigo(enemigo3);
	}
	
	if (_posicion == 288 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, GAME_HEIGHT >> 1);
		pushEnemigo(enemigo);
	} else if (_posicion == 289 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) - 16);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) + 16);
		pushEnemigo(enemigo);
	} else if (_posicion == 290 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) - 32);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) + 32);
		pushEnemigo(enemigo);
	} else if (_posicion == 291 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) - 48);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) + 48);
		pushEnemigo(enemigo);
	} else if (_posicion == 292 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) - 64);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (GAME_HEIGHT >> 1) + 64);
		pushEnemigo(enemigo);
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