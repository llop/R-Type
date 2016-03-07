
#include "cNivel.h"
#include "Globals.h"

#include <sstream>
using namespace std;


bool cNivel::cargaMapa(int tilesAncho, int tilesAlto, int anchoTile, int altoTile, 
						const char* ficheroMapa) {
	FILE *fd = fopen(ficheroMapa, "r");
	if (fd == NULL) return false;

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


cNivel::cNivel(cNaveEspacial* naveEspacial, 
				int tilesAncho, int tilesAlto, int anchoTile, int altoTile, 
				const char* ficheroMapa) {
	_naveEspacial = naveEspacial;
	_posicion = 0;

	if (!cargaMapa(tilesAncho, tilesAlto, anchoTile, altoTile, ficheroMapa)) {
		stringstream errMsg;
		errMsg<<"Error cargando el mapa de tiles en '" << ficheroMapa << "'.\n"
			<<"Dimensiones "<<tilesAncho<<"x"<<tilesAlto;
		throw exception(errMsg.str().c_str());
	}

	_data.LoadImage(IMG_BLOCKS, "blocks.png", GL_RGBA);
}

cNivel::~cNivel() {

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

void cNivel::logica(const cSistema &sis) {
	++_posicion;

	// tratar colisiones

	// aplicar logicas
	for (list<cItem*>::iterator it=_items.begin(); it!=_items.end(); ++it) 
		(*it)->logica(sis);
	for (list<cDisparo*>::iterator it=_disparos.begin(); it!=_disparos.end(); ++it) 
		(*it)->logica(sis);
	
	// jugador y enemigos (IA)
	_naveEspacial->logica(sis);
	for (list<cEnemigo*>::iterator it=_enemigos.begin(); it!=_enemigos.end(); ++it) 
		(*it)->logica(sis);
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
	glTranslatef(-_posicion, 0, 0);

	float coordx_tile, coordy_tile;
	int px, py;
	int colIni = _posicion / _anchoTile;


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _data.GetID(IMG_BLOCKS));
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
