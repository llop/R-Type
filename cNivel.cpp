
#include "cNivel.h"
#include "Globals.h"

#include <sstream>
using namespace std;


bool cNivel::cargaMapa(int tilesAncho, int tilesAlto, int anchoTile, int altoTile, 
						const char* ficheroMapa) {
	FILE *fd;
	errno_t err = fopen_s(&fd, ficheroMapa, "r");
	if (err != 0) return false;

	_tilesAncho = tilesAncho;
	_tilesAlto = tilesAlto;

	_anchoTile = anchoTile;
	_altoTile = altoTile;

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
				int tilesAncho, int tilesAlto, int anchoTile, int altoTile, 
				const char* ficheroMapa, const char* ficheroTextura) : cSprite(sis) {
	_naveEspacial = naveEspacial;
	_posicion = 0;

	if (!cargaMapa(tilesAncho, tilesAlto, anchoTile, altoTile, ficheroMapa)) {
		stringstream errMsg;
		errMsg<<"Error cargando el mapa de tiles en '" << ficheroMapa << "'.\n"
			<<"Dimensiones "<<tilesAncho<<"x"<<tilesAlto;
		throw exception(errMsg.str().c_str());
	}

	_sis->cargaTextura(TEX_NIVEL1, ficheroTextura);
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
}


int cNivel::getPosicion() const {
	return _posicion;
}

const list<cItem*> cNivel::getItems() const {
	return _items;
}

const list<cEnemigo*> cNivel::getEnemigos() const {
	return _enemigos;
}

const list<cDisparo*> cNivel::getDisparos() const {
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

void cNivel::delItem(cItem* item) {
	_items.remove(item);
}

void cNivel::delEnemigo(cEnemigo* enemigo) {
	_enemigos.remove(enemigo);
}

void cNivel::delDisparo(cDisparo* disparo) {
	_disparos.remove(disparo);
}


void cNivel::avanzaPosicion() {
	// decidir aqui cuanto avanza el scroll por frame
	int avanza = 1;
	if (_posicion == 512) avanza = 0;		// es el final del escenario: detener el scroll

	// avanzar automaticamente todo
	_posicion += avanza;
	for (list<cItem*>::iterator it=_items.begin(); it!=_items.end(); ++it) {
		int x, y;
		(*it)->getPosicion(x, y);
		(*it)->setPosicion(x + avanza, y);
	}
	for (list<cDisparo*>::iterator it=_disparos.begin(); it!=_disparos.end(); ++it) {
		int x, y;
		(*it)->getPosicion(x, y);
		(*it)->setPosicion(x + avanza, y);
	}
	for (list<cEnemigo*>::iterator it=_enemigos.begin(); it!=_enemigos.end(); ++it) {
		int x, y;
		(*it)->getPosicion(x, y);
		(*it)->setPosicion(x + avanza, y);
	}
	int xNave, yNave;
	_naveEspacial->getPosicion(xNave, yNave);
	_naveEspacial->setPosicion(xNave + avanza, yNave);
}

void cNivel::generaEnemigos() {
	if (_posicion == 300) {
		cEnemigo1* n = new cEnemigo1(_sis, _posicion + GAME_WIDTH + 10, 200);
		pushEnemigo(n);
	}
}

void cNivel::aplicaLogicas() {
	// aplicar logicas
	for (list<cItem*>::iterator it=_items.begin(); it!=_items.end(); ++it) (*it)->logica();
  	for (list<cDisparo*>::iterator it=_disparos.begin(); it!=_disparos.end(); ++it) (*it)->logica();
	
	// jugador y enemigos (IA)
	_naveEspacial->logica();
	for (list<cEnemigo*>::iterator it=_enemigos.begin(); it!=_enemigos.end(); ++it) (*it)->logica();
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

void cNivel::trataColisiones() {
	// la nave

	// los enemigos

	// los disparos

	// los items pueden atravesar paredes!
}

// maneja toda la interaccion:
//   nave espacial
//   items
//   enemigos
//   disparos
//   decorado
void cNivel::logica() {
	avanzaPosicion();
	generaEnemigos();
	trataColisiones();
	aplicaLogicas();
	aplicaMuertes();
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
	float xTranslate = (float)-_posicion;
	glTranslatef(xTranslate, 0, 0);


	// pintar mierdas
	for (list<cItem*>::const_iterator it = _items.begin(); it != _items.end(); ++it) (*it)->pinta();
	for (list<cDisparo*>::const_iterator it = _disparos.begin(); it != _disparos.end(); ++it) (*it)->pinta();
	for (list<cEnemigo*>::const_iterator it = _enemigos.begin(); it != _enemigos.end(); ++it) (*it)->pinta();

	// pintar la nave
	_naveEspacial->pinta();

	// pintar el fondo lo ultimo
	float coordx_tile, coordy_tile;
	int px, py;
	int colIni = _posicion / _anchoTile;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _sis->getIdTextura(TEX_NIVEL1));
	glBegin(GL_QUADS);
	for (int i = 0; i < 30; ++i) {
		py =  _tilesAlto * _altoTile - (i+1) * _altoTile;

		for (int j = 0; j < 40 + 1; ++j) {
			int col = j + colIni;
			if (col < _tilesAncho) {
				int index = i * _tilesAncho + col;
				int tile = _mapa[index];
				if (tile != -1) {
					if (tile == 0) tile = 1;
					else if (tile == 2) tile = 2;
					else if (tile == 8) tile = 3;
					else if (tile == 10) tile = 4;

					if (tile % 2) coordx_tile = 0.0f;
					else coordx_tile = 0.5f;
					if (tile < 3) coordy_tile = 0.0f;
					else coordy_tile = 0.5f;

					px = col * _anchoTile;
					glTexCoord2f(coordx_tile, coordy_tile + 0.25f);			glVertex2i(px, py);
					glTexCoord2f(coordx_tile + 0.25f, coordy_tile + 0.25f);	glVertex2i(px + _anchoTile, py);
					glTexCoord2f(coordx_tile + 0.25f, coordy_tile);			glVertex2i(px + _anchoTile, py + _altoTile);
					glTexCoord2f(coordx_tile, coordy_tile);					glVertex2i(px, py + _altoTile);
				}
			}
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
