
#include "cNivel.h"
#include "Globals.h"

#include <sstream>
using namespace std;


int pausaMov[4] = { 360, 377, 114, 18 };

bool cNivel::cargaMapa(const char* ficheroMapa) {
	FILE *fd;
	errno_t err = fopen_s(&fd, ficheroMapa, "r");
	if (err != 0) return false;

	int tamanoMapa = _tilesAncho * _tilesAlto;
	_mapa.resize(tamanoMapa);

	// leer
	fseek(fd, 0 , SEEK_END);							// pillar tamaño fichero
	long lSize = ftell(fd);
	rewind(fd);
	char* buffer = (char*)malloc(sizeof(char)*lSize);	// leerse todo el fichero de una
	fread(buffer, 1, lSize, fd);
	fclose(fd);
	stringstream ss;									// meter el contenido en un stream
	ss << buffer;
	free(buffer);

	// parsear csv
	int i = 0;
	string line, numStr;
	stringstream iss;
	while (getline(ss, line)) {
		iss << line;
		while (i < tamanoMapa && getline(iss, numStr, ',')) {
			_mapa[i] = atoi(numStr.c_str());
			++i;
		}
		iss.clear();
    }

	return true;
}


cNivel::cNivel(cSistema* sis, cNaveEspacial* naveEspacial, 
				int tilesAncho, int tilesAlto,
				const char* ficheroMapa, 
				int idTextura, int idFondo, 
				const char* ficheroTextura,
				const char* ficheroFondo,
				int idMusica, const char* ficheroMusica) : cSprite(sis) {

	_sis->cargaTextura(TEX_MENU, "img\\menu.png");

	_sis->cargaSonido(idMusica, ficheroMusica, true);
	_sis->playSonido(idMusica);
	_idMusica = idMusica;
	_naveEspacial = naveEspacial;
	//vector<cEscudo*> escudos = _naveEspacial->escudos();
	//for (unsigned int i=0; i<escudos.size(); ++i) pushEscudo(escudos[i]);

	_tilesAncho = tilesAncho;
	_tilesAlto = tilesAlto;

	// si no carga bien, mal rollo
	if (!cargaMapa(ficheroMapa)) {
		stringstream errMsg;
		errMsg<<"Error cargando el mapa de tiles en '"<<ficheroMapa <<"'.\n"<<"Dimensiones "<<tilesAncho<<"x"<<tilesAlto;
		throw exception(errMsg.str().c_str());
	}

	_idTextura = idTextura;
	_sis->cargaTextura(_idTextura, ficheroTextura);

	int texWid, texHei;
	_sis->tamanoTextura(_idTextura, texWid, texHei);
	_tilesAnchoTex = texWid/TILE_WIDTH;
	_tilesAltoTex = texHei/TILE_HEIGHT;
	
	_idFondo = idFondo;
	_sis->cargaTextura(_idFondo, ficheroFondo);

	

	// posicion final del mapa en pixels
	// por ejemplo: 288*16 - 40*16 (anchura en tiles del mapa * ancho tile - anchura en tiles de la pantalla * ancho tile)
	// 40 = (GAME_WIDTH / _anchoTile)
	_posicionFinal = _tilesAncho*TILE_WIDTH-MAP_WIDTH*TILE_WIDTH;
	_posicion = 0;
	_delay = NIVEL_DELAY;

	// crear el hud
	_hud = new cHud(_sis, 0, HUD_HPIX);

	_state = NIVEL_NACE;
	_seq = 0;
	_maxFade = NIVEL_FADE;

	_puntos = 0;

	_enPausa = false;
	_tiempoPausa = NIVEL_INTERVALO_PAUSA;
	_tiempoMagia = NIVEL_DELAY_MAGIA;
	_tiempo = 0;
}

cNivel::~cNivel() {
	// al destruir el nivel, hay que destruir cosas que hayan quedado por ahi dentro
	for (list<cItem*>::iterator it=_items.begin(); it!=_items.end();) {
		cItem* item = *it;
		it = _items.erase(it);
		delete item;
	}
	for (list<cDisparo*>::iterator it=_disparos.begin(); it!=_disparos.end();) {
		cDisparo* disparo = *it;
		it = _disparos.erase(it);
		delete disparo;
	}
	for (list<cEnemigo*>::iterator it=_enemigos.begin(); it!=_enemigos.end();) {
		cEnemigo* enemigo = *it;
		it = _enemigos.erase(it);
		delete enemigo;
	}
	// el hud
	if (_hud != NULL) {
		delete _hud;
		_hud = NULL;
	}
}

void cNivel::tiraMagia() {
	for (list<cDisparo*>::iterator it = _disparos.begin(); it != _disparos.end(); ++it) {
		cDisparo* disparo = *it;
		if (disparo->malo() && disparo->vive()) disparo->explota();
	}
	for (list<cEnemigo*>::iterator it = _enemigos.begin(); it != _enemigos.end(); ++it) {
		cEnemigo* enemigo = *it;
		enemigo->restaVida(NIVEL_DANO_MAGIA);
	}

	_tiempoMagia = 0;
}


void cNivel::sumaPuntos(long long puntos) {
	_puntos += puntos;
}
long long cNivel::puntos() const {
	return _puntos;
}


int cNivel::getPosicion() const {
	return _posicion;
}

const list<cItem*> cNivel::items() const {
	return _items;
}

const list<cEnemigo*> cNivel::enemigos() const {
	return _enemigos;
}

const list<cDisparo*> cNivel::disparos() const {
	return _disparos;
}

void cNivel::pushItem(cItem* item) {
	_items.insert(_items.end(), item);
}

void cNivel::pushEnemigo(cEnemigo* enemigo) {
	_enemigos.insert(_enemigos.end(), enemigo);
}

void cNivel::pushDisparo(cDisparo* disparo) {
	_disparos.insert(_disparos.end(), disparo);
}


void cNivel::procesaTeclas(unsigned char* keys) {

	if ((keys['p'] || keys['P']) && _tiempoPausa >= NIVEL_INTERVALO_PAUSA) {
		_enPausa = !_enPausa;
		_tiempoPausa = 0;
		if (_enPausa) _sis->pauseSonidos();
		else _sis->resumeSonidos();
	}

	/*if (keys['s'] || keys['S']) {
		_sis->activeSound();
		if (_sis->soundEnabled()) _sis->playSonido(_idMusica);
		else _sis->stopSonidos();
	}*/

	if (!_enPausa) _naveEspacial->procesaTeclas(keys);

}


void cNivel::aplicaScroll() {
	// es el final del escenario: detener el scroll
	if (_posicion >= _posicionFinal) {
		//_posicion = _posicionFinal;
		return;
	}

	// que sea la subclase quien implementa el avance en cada momento
	int avanza = avanzaPosicion();

	// avanzar automaticamente todo:
	// la nave, el HUD, y los enemigos que se dejen
	if (avanza) {
		_posicion += avanza;
		for (list<cDisparo*>::iterator it = _disparos.begin(); it != _disparos.end(); ++it) (*it)->offset(avanza, 0);
		for (list<cEnemigo*>::iterator it=_enemigos.begin(); it!=_enemigos.end(); ++it) (*it)->offset(avanza, 0);
		_naveEspacial->offset(avanza, 0);
		_hud->offset(avanza, 0);
	}
}

void cNivel::aplicaLogicas() {
	// aplicar logicas
	for (list<cItem*>::iterator it = _items.begin(); it != _items.end(); ++it) (*it)->logica();
	for (list<cDisparo*>::iterator it = _disparos.begin(); it != _disparos.end(); ++it) (*it)->logica();

	// jugador y enemigos (IA)
	_naveEspacial->logica();
	//for (list<cEscudo*>::iterator it = _escudos.begin(); it != _escudos.end(); ++it) (*it)->logica();

	for (list<cEnemigo*>::iterator it = _enemigos.begin(); it != _enemigos.end(); ++it) (*it)->logica();

	_hud->logica();
}

void cNivel::aplicaMuertes() {
	// matar a quien lo necesite
	// quitar sprite de la lista correspondiente y liberar la memoria
	for (list<cItem*>::iterator it=_items.begin(); it!=_items.end();) {
		cItem* item = *it;
		if (item->muerto()) {
			it = _items.erase(it);
			delete item;
		} else ++it;
	}
	for (list<cDisparo*>::iterator it=_disparos.begin(); it!=_disparos.end();) {
		cDisparo* disparo = *it;
		if (disparo->muerto()) {
			it = _disparos.erase(it);
			delete disparo;
		} else ++it;
	}
	for (list<cEnemigo*>::iterator it=_enemigos.begin(); it!=_enemigos.end();) {
		cEnemigo* enemigo = *it;
		if (enemigo->muerto()) {
			it = _enemigos.erase(it);
			delete enemigo;
		} else ++it;
	}
}


// rect: miramos si este rectangulo ha chocado contra algo del nivel
// colisionMask: indica si ha chocado arriba, abajo, izquierda o derecha
//               es una mascara de bits (COLISION_ARRIBA, COLISION_ABAJO, COLISION_IZQ, COLISION_DER)
//               por ejemplo, para saber is ha chocado la base del rectangulo:
//                 colisionMask & COLISION_ABAJO
// x, y: las coordenadas del choque, sólo si se ha producido
// objeto: si hubo colision, el tipo de objeto contra el que ha chocado el rectangulo
//         puede ser un tile (COLISION_TILE) o los limites de la pantalla (COLISION_PANTALLA)
void cNivel::colisionNivel(const cRect &rect, int &colisionMask, int &x, int &y, int &objeto) {
	colisionMask = 0;
	objeto = 0;

	int tileArriba = rect.y / TILE_HEIGHT;
	int tileAbajo = (rect.y+rect.h) / TILE_HEIGHT;
	int tileIzquierda = rect.x / TILE_WIDTH;
	int tileDerecha = (rect.x+rect.w) / TILE_WIDTH;

	//--------------------------------------------------------------------------
	// mirar los tiles
	//--------------------------------------------------------------------------
	if (tileArriba<_tilesAlto && tileIzquierda<_tilesAncho && tileDerecha>=0 && tileAbajo>=0) {
		// colisiona arriba
		if (tileArriba>=0) {
			int i = tileArriba;
			int j = max(0, tileIzquierda);
			int k = min(_tilesAncho-1, tileDerecha);
			while (j <= k) {
				int tile = _mapa[i * _tilesAncho + j];
				if (tile != -1) {
					colisionMask |= COLISION_ARRIBA;
					objeto |= COLISION_TILE;
					y = (tileArriba+1)*TILE_HEIGHT;
				}
				++j;
			}
		}
		// colisiona abajo
		if (tileAbajo<_tilesAlto) {
			int i = tileAbajo;
			int j = max(0, tileIzquierda);
			int k = min(_tilesAncho-1, tileDerecha);
			while (j <= k) {
				int tile = _mapa[i * _tilesAncho + j];
				if (tile != -1) {
					colisionMask |= COLISION_ABAJO;
					objeto |= COLISION_TILE;
					y = tileAbajo*TILE_HEIGHT;
				}
				++j;
			}
		}
		// colisiona izquierda 
		if (tileIzquierda>=0) {
			int i = max(0, tileArriba);
			int j = tileIzquierda;
			int k = min(_tilesAlto-1, tileAbajo);
			while (i <= k) {
				int tile = _mapa[i * _tilesAncho + j];
				if (tile != -1) {
					colisionMask |= COLISION_IZQ;
					objeto |= COLISION_TILE;
					x = (tileIzquierda+1)*TILE_WIDTH;
				}
				++i;
			}
		}
		// colisiona derecha
		if (tileDerecha<_tilesAncho) {
			int i = max(0, tileArriba);
			int j = tileDerecha;
			int k = min(_tilesAlto-1, tileAbajo);
			while (i <= k) {
				int tile = _mapa[i * _tilesAncho + j];
				if (tile != -1) {
					colisionMask |= COLISION_DER;
					objeto |= COLISION_TILE;
					x = tileDerecha*TILE_WIDTH;
				}
				++i;
			}
		}
	}

	// mirar si queda fuera de la pantalla
	if (rect.y+rect.h >= GAME_HEIGHT - HUD_HPIX) {
		colisionMask |= COLISION_ABAJO;
		objeto |= COLISION_PANTALLA;
		y = GAME_HEIGHT - HUD_HPIX;
	} 
	if (rect.y < 0) {
		colisionMask |= COLISION_ARRIBA;
		objeto |= COLISION_PANTALLA;
		y = 0;
	}
	if (rect.x+rect.w >= _posicion + GAME_WIDTH) {
		colisionMask |= COLISION_DER;
		objeto |= COLISION_PANTALLA;
		x = _posicion + GAME_WIDTH;
	} 
	if (rect.x < _posicion) {
		colisionMask |= COLISION_IZQ;
		objeto |= COLISION_PANTALLA;
		x = _posicion;
	}
	
}

void cNivel::colision(cRect &caja, int &colMask) const {
	colMask = 0;

	int tileArriba = max(0, caja.y / TILE_HEIGHT);
	int tileAbajo = min(_tilesAlto-1, (caja.y + caja.h) / TILE_HEIGHT);
	int tileIzquierda = max(0, caja.x / TILE_WIDTH);
	int tileDerecha = min(_tilesAncho-1, (caja.x + caja.w) / TILE_WIDTH);

	if (tileArriba>=_tilesAlto || tileIzquierda>=_tilesAncho || tileDerecha<0 || tileAbajo<0) return;

	// colisiona arriba
	int i = tileArriba;
	int j = tileIzquierda;
	while (j <= tileDerecha) {
		int tile = _mapa[i * _tilesAncho + j];
		if (tile != -1) colMask |= COLISION_ARRIBA;
		++j;
	}
	
	// colisiona abajo
	i = tileAbajo;
	j = tileIzquierda;
	while (j <= tileDerecha) {
		int tile = _mapa[i * _tilesAncho + j];
		if (tile != -1) colMask |= COLISION_ABAJO;
		++j;
	}

	// colisiona izquierda 
	i = tileArriba;
	j = tileIzquierda;
	while (i <= tileAbajo) {
		int tile = _mapa[i * _tilesAncho + j];
		if (tile != -1) colMask |= COLISION_IZQ;
		++i;
	}

	// colisiona derecha
	i = tileArriba;
	j = tileDerecha;
	while (i <= tileAbajo) {
		int tile = _mapa[i * _tilesAncho + j];
		if (tile != -1) colMask |= COLISION_DER;
		++i;
	}

}


void cNivel::caja(cRect &rect) const {
	rect.x = _posicion;
	rect.y = 0;
	rect.w = GAME_WIDTH;
	rect.h = GAME_HEIGHT - HUD_HEIGHT;
}

bool cNivel::fueraLimites(cRect &rect) const {
	bool res = true;
	cRect myRect;
	caja(myRect);
	myRect.x -=10;
	myRect.y -=5;
	myRect.w += 20;
	myRect.h += 10;
	if (myRect.x < rect.x+rect.w && myRect.x+myRect.w > rect.x &&
		myRect.y < rect.y+rect.h && myRect.y+myRect.h > rect.y) {
			res = false;
	}
	return res;
}

// maneja toda la interaccion:
//   nave espacial
//   items
//   enemigos
//   disparos
//   decorado
void cNivel::logica() {
	++_tiempo;

	++_tiempoPausa;
	if (_enPausa) return;

	++_tiempoMagia;

	aplicaScroll();
	generaEnemigos();
	aplicaLogicas();
	aplicaMuertes();

	if (_state == NIVEL_NACE) {
		// fade in
		++_seq;
		if (_seq == NIVEL_FADE) _state = NIVEL_VIVE;
	} else if (_state == NIVEL_MUERE) {
		// fade out	
		--_seq;
		if (!_seq) _sis->avanzaNivel();
	} else if (_state == NIVEL_ABORT) {
		--_seq;
		if (!_seq) _sis->gameOver();
	}
}

void cNivel::termina() {
	if (_state != NIVEL_ABORT) {
		_state = NIVEL_MUERE;
		_maxFade = NIVEL_FADE;
	}
}

void cNivel::gameOver() {
	if (_state != NIVEL_MUERE) {
		_seq = NIVEL_GAME_OVER_FADE;
		_maxFade = NIVEL_GAME_OVER_FADE;
	}
	_state = NIVEL_ABORT;
	
}



void cNivel::pinta() const {
	/*
	https://www.opengl.org/archives/resources/faq/technical/viewing.htm
	Where should my camera go, the ModelView or Projection matrix?
	The GL_PROJECTION matrix should contain only the projection transformation calls it needs 
	to transform eye space coordinates into clip coordinates.
	The GL_MODELVIEW matrix, as its name implies, should contain modeling and viewing transformations, 
	which transform object space coordinates into eye space coordinates. Remember to place the camera 
	transformations on the GL_MODELVIEW matrix and never on the GL_PROJECTION matrix.
	Think of the projection matrix as describing the attributes of your camera, such as field of view, focal length, 
	fish eye lens, etc. Think of the ModelView matrix as where you stand with the camera and the direction you point it.
	*/
	// Mover el mundo para poner lo que queremos frente a la camara
	float xTranslate = float(-_posicion);
	glTranslatef(xTranslate, 0, 0);

	// pintar el fondo del escenario
	if (_tiempoMagia < NIVEL_DELAY_MAGIA && !(_tiempoMagia%4)) {
		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glVertex2i(_posicion, HUD_HPIX);
		glVertex2i(_posicion + GAME_WIDTH, HUD_HPIX);
		glVertex2i(_posicion + GAME_WIDTH, GAME_HEIGHT);
		glVertex2i(_posicion, GAME_HEIGHT);
		glEnd();
	} else {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _sis->idTextura(_idFondo));
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1);		glVertex2i(_posicion, HUD_HPIX);
		glTexCoord2f(1, 1);		glVertex2i(_posicion + GAME_WIDTH, HUD_HPIX);
		glTexCoord2f(1, 0);		glVertex2i(_posicion + GAME_WIDTH, GAME_HEIGHT);
		glTexCoord2f(0, 0);		glVertex2i(_posicion, GAME_HEIGHT);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	// pintar el fondo
	int px, py;
	int colIni = _posicion / TILE_WIDTH;

	float xTile, yTile;
	int wTex, hTex;
	_sis->tamanoTextura(_idTextura, wTex, hTex);

	int alturaEnPixels = (MAP_HEIGHT+HUD_HEIGHT)*TILE_HEIGHT;
	float wTile = TILE_WIDTH/float(wTex);
	float hTile = TILE_HEIGHT/float(hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _sis->idTextura(_idTextura));
	glBegin(GL_QUADS);
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		py = alturaEnPixels - (i+1) * TILE_HEIGHT;
		for (int j = 0; j <= MAP_WIDTH; ++j) {
			int col = j + colIni;
			if (col < _tilesAncho) {
				int index = i * _tilesAncho + col;
				int tile = _mapa[index];
				if (tile != -1) {
					xTile = (tile%_tilesAnchoTex)/float(_tilesAnchoTex);
					yTile = (tile/_tilesAnchoTex)/float(_tilesAltoTex);
					px = col * TILE_WIDTH;
					glTexCoord2f(xTile, yTile + hTile);			glVertex2i(px, py);
					glTexCoord2f(xTile + wTile, yTile + hTile);	glVertex2i(px + TILE_WIDTH, py);
					glTexCoord2f(xTile + wTile, yTile);			glVertex2i(px + TILE_WIDTH, py + TILE_HEIGHT);
					glTexCoord2f(xTile, yTile);					glVertex2i(px, py + TILE_HEIGHT);
				}
			}
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// dejar los tiros por encima
	// pintar mierdas
	for (list<cEnemigo*>::const_reverse_iterator it = _enemigos.rbegin(); it != _enemigos.rend(); ++it) (*it)->pinta();
	for (list<cItem*>::const_iterator it = _items.begin(); it != _items.end(); ++it) (*it)->pinta();

	// pintar la nave
	if (_state == NIVEL_VIVE) _naveEspacial->pinta();
	for (list<cDisparo*>::const_iterator it = _disparos.begin(); it != _disparos.end(); ++it) (*it)->pinta();
	
	_hud->pinta();

	if (_state == NIVEL_NACE || _state == NIVEL_MUERE || _state == NIVEL_ABORT) {
		float alpha = (_maxFade-_seq)/float(_maxFade);
		//  fade in/out
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glColor4f(0, 0, 0, alpha); 
		glVertex2i(_posicion, 0);
		glVertex2i(_posicion+GAME_WIDTH, 0);
		glVertex2i(_posicion+GAME_WIDTH, GAME_HEIGHT);
		glVertex2i(_posicion, GAME_HEIGHT);
		glColor3f(1, 1, 1);
		glEnd();
		glDisable(GL_BLEND);
		
		_naveEspacial->pinta();
		//for (list<cEscudo*>::const_iterator it = _escudos.begin(); it != _escudos.end(); ++it) (*it)->pinta();
	}

	if (_enPausa) {
		int texPausa = _sis->idTextura(TEX_MENU);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texPausa);
		glBegin(GL_QUADS);

		int yTit = (GAME_HEIGHT- pausaMov[3])>>1;
		int wTex, hTex;
		_sis->tamanoTextura(TEX_MENU, wTex, hTex);
		int wPixTit = pausaMov[2];
		int hPixTit = pausaMov[3];
		int xPixTit = (GAME_WIDTH >> 1) - (wPixTit >> 1);
		int yPixTit = GAME_HEIGHT - (yTit + hPixTit);
		float wTexTit = wPixTit / (float)wTex;
		float hTexTit = hPixTit / (float)hTex;
		float xTexTit = pausaMov[0] / (float)wTex;
		float yTexTit = pausaMov[1] / (float)hTex;
		glTexCoord2f(xTexTit, yTexTit + hTexTit);			glVertex2i(_posicion+xPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit + hTexTit);	glVertex2i(_posicion + xPixTit + wPixTit, yPixTit);
		glTexCoord2f(xTexTit + wTexTit, yTexTit);			glVertex2i(_posicion + xPixTit + wPixTit, yPixTit + hPixTit);
		glTexCoord2f(xTexTit, yTexTit);						glVertex2i(_posicion + xPixTit, yPixTit + hPixTit);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	
}
