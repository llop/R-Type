
#include "cNivel.h"


cNivel2::cNivel2(cSistema* sis, cNaveEspacial* naveEspacial, 
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
	//_posicion = 3392;
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

float cNivel2::factorDificultad() const {
	float factor = _posicion / float(_posicionFinal);
	return 2 * (factor / NUM_NIVELES) + 1;
};

void cNivel2::generaEnemigos() {
	cRect rect;
	caja(rect);

	int inter = 8;
	int iniPos = 52 * TILE_WIDTH - GAME_WIDTH;
	if (!_delay && _posicion >= iniPos && _posicion <= iniPos + (inter * 5) && !(_posicion%inter)) {
		int diff = (_posicion - iniPos)/inter;
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (13*TILE_HEIGHT)+(diff*2*TILE_HEIGHT));
		pushEnemigo(enemigo);
	}

	inter = 4;
	iniPos = 74 * TILE_WIDTH - GAME_WIDTH;
	if (!_delay && _posicion >= iniPos && _posicion <= iniPos + (inter * 6) && !(_posicion%inter)) {
		int diff = (_posicion - iniPos) / inter;
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, (20 * TILE_HEIGHT) - (diff * 2 * TILE_HEIGHT));
		pushEnemigo(enemigo);
	}

	// escudo bueno
	if (_posicion == 84 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 6 * (TILE_HEIGHT), -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(enemigo);
	}

	// gripaus
	if (_posicion == 100 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		int y = 0;
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 4; ++j) {
				cEnemigo5* enemigo = new cEnemigo5(_sis, rect.x + (10 * j*TILE_HEIGHT), y);
				pushEnemigo(enemigo);
			}
			y += GAME_HEIGHT;
		}
	}

	if (_posicion == 139 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		int y = 0;
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 4; ++j) {
				cEnemigo5* enemigo = new cEnemigo5(_sis, rect.x + (10 * j*TILE_HEIGHT), y);
				pushEnemigo(enemigo);
			}
			y += GAME_HEIGHT;
		}
	}

	inter = 8;
	iniPos = 152 * TILE_WIDTH - GAME_WIDTH;
	if (!(_posicion%inter) && !_delay && _posicion >= iniPos && _posicion < iniPos + (inter * 8)) {
		cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, 13 * TILE_HEIGHT);
		pushEnemigo(enemigo);
	}

	// mas rapido 2 veces, para joder XD
	if (_posicion == 166 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 9 * (TILE_HEIGHT), -5.0f, 0.0f, ITEM_VELOCIDAD);
		pushEnemigo(enemigo);
	}
	if (_posicion == 166 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 17 * (TILE_HEIGHT), -5.0f, 0.0f, ITEM_VELOCIDAD);
		pushEnemigo(enemigo);
	}

	//--------------------------------------------------------------------
	// uno que anda ataca desde atras
	//--------------------------------------------------------------------
	if (_posicion == 148 * TILE_WIDTH && !_delay) {
		cEnemigo4* enemigo = new cEnemigo4(_sis, rect.x - 10, GAME_HEIGHT - (10 * TILE_HEIGHT) - 12, ENEMIGO4_ANDA_DER);
		pushEnemigo(enemigo);
	}

	// tres gusanillos
	int radiCuc = 144;
	if (_posicion == 182 * TILE_WIDTH - GAME_WIDTH - radiCuc - 10 && !_delay) {
		cEnemigo3* enemigo3 = new cEnemigo3(_sis, 182 * TILE_WIDTH, 14 * TILE_HEIGHT, 9, float(radiCuc), 0.0f, 0.005f, false);
		pushEnemigo(enemigo3);
		enemigo3 = new cEnemigo3(_sis, 182 * TILE_WIDTH, 14 * TILE_HEIGHT, 9, float(radiCuc), _2PI / 3, 0.005f, false);
		pushEnemigo(enemigo3);
		enemigo3 = new cEnemigo3(_sis, 182 * TILE_WIDTH, 14 * TILE_HEIGHT, 9, float(radiCuc), 2 * _2PI / 3, 0.005f, false);
		pushEnemigo(enemigo3);
	}


	//--------------------------------------------------------------------------
	// flecha de enemigos1
	//--------------------------------------------------------------------------
	iniPos = 120;
	int y = 15*TILE_HEIGHT;
	if (_posicion == (iniPos) * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y);
		pushEnemigo(enemigo);
	} else if (_posicion == (iniPos+1)* TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y - 16);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y + 16);
		pushEnemigo(enemigo);
	} else if (_posicion == (iniPos + 2) * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y - 32);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y + 32);
		pushEnemigo(enemigo);
	} else if (_posicion == (iniPos + 3) * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y - 48);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y + 48);
		pushEnemigo(enemigo);
	} else if (_posicion == (iniPos + 4) * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y - 64);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y + 64);
		pushEnemigo(enemigo);
	} else if (_posicion == (iniPos + 5) * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y - 80);
		pushEnemigo(enemigo);
		enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, y + 80);
		pushEnemigo(enemigo);
	}

	if (!_delay && _posicion >= 224 * TILE_WIDTH - GAME_WIDTH &&
		_posicion <= 258 * TILE_WIDTH - GAME_WIDTH) {
		if (!(_posicion % 24)) {
			cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (4 * TILE_HEIGHT) + (rand() % (18 * TILE_HEIGHT)));
			pushEnemigo(enemigo);
		}
		if (!(_posicion % 32)) {
			cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, (8 * TILE_HEIGHT) + (rand() % (12 * TILE_HEIGHT)));
			pushEnemigo(enemigo);
		}
	}

	// 2 escudos
	if (_posicion == 235 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 4 * (TILE_HEIGHT), -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(enemigo);
	}
	if (_posicion == 248 * TILE_WIDTH - GAME_WIDTH && !_delay) {
		cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 22 * (TILE_HEIGHT), -5.0f, 0.0f, ITEM_ESCUDO);
		pushEnemigo(enemigo);
	}

	/*
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
	
	*/


	//------------------------------------------------------------------------------
	// generar jefe
	//------------------------------------------------------------------------------
	if (_posicion == 3392 && !_delay) {
		_tiempoJefe = _tiempo;

		cJefe2* jefe = new cJefe2(_sis);
		pushEnemigo(jefe);
	}

	if (_tiempoJefe >= 0) {
		long long offset = 260;
		long long interval = _tiempo - _tiempoJefe;
		if (interval == offset) {
			_sis->stopSonido(_idMusica);
			_sis->playSonido(SOUND_JEFE_INTRO);
		}
		if (interval == offset + 292) {
			_sis->stopSonido(SOUND_JEFE_INTRO);
			_sis->playSonido(SOUND_JEFE);
			_tiempoJefe = -1;
		}
	}
}

void cNivel2::posicionRespawn(int &x, int &y) {
	x = _posicion+100;
	y = 255;
}