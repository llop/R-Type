

#include "cNivel.h"

// fmod, openal
cNivel1::cNivel1(cSistema* sis, cNaveEspacial* naveEspacial, 
					int tilesAncho, int tilesAlto,
					const char* ficheroMapa, 
					int idTextura, int idFondo, 
					const char* ficheroTextura,
					const char* ficheroFondo,
					int idMusica, const char* ficheroMusica) : 
							cNivel(sis, naveEspacial, 
									tilesAncho, tilesAlto,
									ficheroMapa, 
									idTextura, idFondo, 
									ficheroTextura,
									ficheroFondo, 
									idMusica, ficheroMusica) {
	_posicion = 4200-640;//3218;
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
	
	int inter = 8;
	int iniPos = 50 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos+(inter*8)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 120);
		pushEnemigo(enemigo);
	}
	
	//--------------------------------------------------------------------
	// un escudo
	//--------------------------------------------------------------------
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
		enemigo = new cEnemigo4(_sis, rect.x+rect.w - 100, y, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}

	if (_posicion == 75 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		int y = 16;
		for (int i = 0; i < 10; ++i) {
			cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y += 32);
			pushEnemigo(enemigo);
		}
	}

	//--------------------------------------------------------------------
	// tiras de enemigo 2
	//--------------------------------------------------------------------
	inter = 8;
	iniPos = 92 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 8)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 16 * TILE_HEIGHT);
		pushEnemigo(enemigo);
	}
	iniPos = 102 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 8)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 4 * TILE_HEIGHT);
		pushEnemigo(enemigo);
	}
	iniPos = 112 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 12)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 10 * TILE_HEIGHT);
		pushEnemigo(enemigo);
	}
	iniPos = 124 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 12)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 18 * TILE_HEIGHT);
		pushEnemigo(enemigo);
	}
	iniPos = 132 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 12)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 8 * TILE_HEIGHT);
		pushEnemigo(enemigo);
	}

	//--------------------------------------------------------------------
	// tiras de enemigo 1
	//--------------------------------------------------------------------
	if (_posicion == 140 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		int y = 16;
		for (int i = 0; i < 10; ++i) {
			cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y += 32);
			pushEnemigo(enemigo);
		}
	}
	
	//--------------------------------------------------------------------
	// otro escudo
	//--------------------------------------------------------------------
	if (_posicion == 158 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 13*TILE_HEIGHT, -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(enemigo);
	}

	//--------------------------------------------------------------------------
	// enemigos andantes
	//--------------------------------------------------------------------------
	if (_posicion == 138 * TILE_WIDTH - GAME_WIDTH + 64 && !_delay) {
		int y = -16;
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10 - 64, y, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10 - 64, y, ENEMIGO4_ANDA_DER);
		pushEnemigo(enemigo);
	}

	if (_posicion >= 162 * TILE_WIDTH - GAME_WIDTH && _posicion <= 188 * TILE_WIDTH - GAME_WIDTH && 
		!(_posicion%128) && !_delay) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x + rect.w + 10, GAME_HEIGHT-(4*TILE_HEIGHT)-12, ENEMIGO4_ANDA_IZQ);
		pushEnemigo(enemigo);
	}

	//--------------------------------------------------------------------------
	// enemigos 1 al azar
	//--------------------------------------------------------------------------
	if (_posicion >= 176 * TILE_WIDTH - GAME_WIDTH && _posicion <= 188 * TILE_WIDTH - GAME_WIDTH &&
		!(_posicion % 64) && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (4*TILE_HEIGHT)+(rand()%(18*TILE_HEIGHT)));
		pushEnemigo(enemigo);
	}

	//--------------------------------------------------------------------
	// disparo guay
	//--------------------------------------------------------------------
	if (_posicion == 202 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 16 * TILE_HEIGHT, -5.0f, 0.0f, ITEM_DISPARO_RB);
		pushEnemigo(enemigo);
	}

	//--------------------------------------------------------------------
	// tira ene2
	//--------------------------------------------------------------------
	iniPos = 214 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 12)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 16 * TILE_HEIGHT);
		pushEnemigo(enemigo);
	}
	
	//--------------------------------------------------------------------
	// uno que anda ataca desde atras
	//--------------------------------------------------------------------
	if (_posicion == 214 * TILE_WIDTH && !_delay) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x - 10, GAME_HEIGHT - (10 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_DER);
		pushEnemigo(enemigo);
	}

	//--------------------------------------------------------------------------
	// gusanos
	//--------------------------------------------------------------------------
	// enemigo 3 (251, 14) -> (4016, 224), radio (14-5) -> 144, spawnPos = (4016-144-15-640) = 3217
	if (_posicion==3218 && !_delay) {
		cEnemigo3* enemigo3 = new cEnemigo3(_sis, 4016, 224, 10, 144.0f, 0.0f, 0.005f, false);
		pushEnemigo(enemigo3);

		enemigo3 = new cEnemigo3(_sis, 4016, 224, 10, 144.0f, acos(-1.0f), 0.005f, false);
		pushEnemigo(enemigo3);
	}
	
	//--------------------------------------------------------------------------
	// flecha de enemigos1
	//--------------------------------------------------------------------------
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

	//--------------------------------------------------------------------------
	// generar el jefe
	//--------------------------------------------------------------------------
	if (_posicion==4200 && !_delay) {
		cJefe1* jefe = new cJefe1(_sis);
		pushEnemigo(jefe);
	}
}

void cNivel1::posicionRespawn(int &x, int &y) {
	x = _posicion+100;
	y = 255;
}