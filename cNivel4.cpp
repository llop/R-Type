
#include "cNivel.h"


cNivel4::cNivel4(cSistema* sis, cNaveEspacial* naveEspacial,
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
	_iterAvance = 0;
}

float cNivel4::factorDificultad() const {
	float posFinal = float((40*TILE_WIDTH)*NIVEL4_ITER+((_tilesAncho-40)*TILE_WIDTH));
	float factor = (_posicion+(40*TILE_WIDTH)*_iterAvance) / posFinal;
	return 4 * (factor / NUM_NIVELES) + 1;
};


int cNivel4::avanzaPosicion() {
	int avanza = NIVEL4_AVANCE;
	if (!((_posicion+NIVEL4_AVANCE)%(40*TILE_WIDTH)) && _iterAvance<NIVEL4_ITER) {
		++_iterAvance;
		avanza = -_posicion;
	}
	if (_posicion + avanza > _posicionFinal) avanza = _posicionFinal - _posicion;

	if (_iterAvance<NIVEL4_ITER || avanza<0) {
		for (list<cItem*>::iterator it = _items.begin(); it != _items.end(); ++it) (*it)->offset(avanza, 0);
	}

	return avanza;
}

void cNivel4::generaEnemigos() {
	int posReal = _posicion + (40 * TILE_WIDTH)*_iterAvance;

	if (_iterAvance>= 2 && _iterAvance < NIVEL4_ITER) {
		cRect rect;
		caja(rect);

		int modulo = _sis->dificultad() == DIFICULTAD_NORMAL ? NIVEL4_FREC_GRIPAU : NIVEL4_FREC_GRIPAU_HARD;
		modulo = int(modulo / factorDificultad());
		if (!(rand() % modulo)) {
			// generar un gripau al azar
			int x = _posicion + (rand() % GAME_WIDTH);
			int y = rand() % 2 ? 0 : GAME_HEIGHT;
			cEnemigo5* enemigo = new cEnemigo5(_sis, x, y);
			pushEnemigo(enemigo);
		}

		
		if (_iterAvance == 2 && !_posicion) {
			cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 16 * TILE_HEIGHT, -5.0f, 0.0f, ITEM_ESCUDO);
			pushEnemigo(enemigo);
		}
		if (_iterAvance == 5 && !_posicion) {
			cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 10 * TILE_HEIGHT, -5.0f, 0.0f, ITEM_ESCUDO);
			pushEnemigo(enemigo);
		}
		if (_iterAvance == 10 && !_posicion) {
			cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 4 * TILE_HEIGHT, -5.0f, 0.0f, ITEM_ESCUDO);
			pushEnemigo(enemigo);
		}

		if (_iterAvance == 36 && !_posicion) {
			cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 10 * TILE_HEIGHT, -5.0f, 0.0f, ITEM_DISPARO_RB);
			pushEnemigo(enemigo);
		}
		if (_iterAvance == 37 && !_posicion) {
			cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 10 * TILE_HEIGHT, -5.0f, 0.0f, ITEM_VIDA);
			pushEnemigo(enemigo);
		}
		if (_iterAvance == 38 && !_posicion) {
			cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 10 * TILE_HEIGHT, -5.0f, 0.0f, ITEM_ESCUDO);
			pushEnemigo(enemigo);
		}
		if (_iterAvance == 39 && !_posicion) {
			cEnemigoItem* enemigo = new cEnemigoItem(_sis, rect.x + rect.w + 8, 10 * TILE_HEIGHT, -5.0f, 0.0f, ITEM_ESCUDO);
			pushEnemigo(enemigo);
		}

		if (_iterAvance < 10 && !(posReal%(NIVEL4_AVANCE*32))) {
			cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (4 * TILE_HEIGHT) + (rand() % (18 * TILE_HEIGHT)));
			pushEnemigo(enemigo);
		}
		if (_iterAvance>=10 && _iterAvance<20 && !(posReal%(NIVEL4_AVANCE*24))) {
			cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, (8 * TILE_HEIGHT) + (rand() % (12 * TILE_HEIGHT)));
			pushEnemigo(enemigo);
		}
		if (_iterAvance>=20 && _iterAvance<38) {
			if (!(posReal % (NIVEL4_AVANCE * 24))) {
				cEnemigo1* enemigo = new cEnemigo1(_sis, rect.x + rect.w + 10, (4 * TILE_HEIGHT) + (rand() % (18 * TILE_HEIGHT)));
				pushEnemigo(enemigo);
			}
			if (!(posReal % (NIVEL4_AVANCE * 32))) {
				cEnemigo2* enemigo = new cEnemigo2(_sis, rect.x + rect.w + 10, (8 * TILE_HEIGHT) + (rand() % (12 * TILE_HEIGHT)));
				pushEnemigo(enemigo);
			}
		}

	}

	int posFinal = (40 * TILE_WIDTH)*NIVEL4_ITER + ((_tilesAncho - 40)*TILE_WIDTH);
	if (posReal == (40 * TILE_WIDTH)*NIVEL4_ITER) {
		_tiempoJefe = _tiempo;

		_sis->stopSonido(_idMusica);
		_sis->playSonido(SOUND_JEFE_INTRO);

		cJefe4* enemigo = new cJefe4(_sis);
		pushEnemigo(enemigo);
	}


	if (_tiempoJefe >= 0) {
		long long interval = _tiempo - _tiempoJefe;
		if (interval == 292) {
			_sis->stopSonido(SOUND_JEFE_INTRO);
			_sis->playSonido(SOUND_JEFE);
			_tiempoJefe = -1;
		}
	}
}

void cNivel4::posicionRespawn(int &x, int &y) {
	x = _posicion + 100;
	y = GAME_HEIGHT>>1;
}