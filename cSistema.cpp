
#include "Globals.h"
#include "cSistema.h"
#include "cNaveEspacial.h"
#include "cMenu.h"
#include "cNivel.h"



cSistema::cSistema() {
	_naveEspacial = NULL;
	_nivel = NULL;
	_numNivel = 1;
	_menu = NULL;
	_data = new cData();
	_estado = MENU;
}

cSistema::~cSistema() {
	if (_data != NULL) {
		delete _data;
		_data = NULL;
	}

	delNaveEspacial();
	delNivel();
	
	if (_menu != NULL) {
		delete _menu;
		_menu = NULL;
	}
}


void cSistema::cargaTextura(int id, const char* ficheroTextura) {
	_data->loadImage(id, ficheroTextura, GL_RGBA);
}

int cSistema::idTextura(int id) const {
	return _data->getID(id);
}

void cSistema::tamanoTextura(int id, int &width, int &height) const {
	_data->getSize(id, &width, &height);
}


// consultar
cSprite* cSistema::naveEspacial() const {
	return _naveEspacial;
}

cSprite* cSistema::nivel() const {
	return _nivel;
}


// eliminar
void cSistema::delNaveEspacial() {
	if (_naveEspacial != NULL) {
		delete _naveEspacial;
		_naveEspacial = NULL;
	}
}

void cSistema::delNivel() {
	if (_nivel != NULL) {
		delete _nivel;
		_nivel = NULL;
	}
}


// esto para el game loop
void cSistema::procesaTeclas(unsigned char *keys) {
	if (_estado == MENU) {
		if (_menu != NULL) ((cMenu*)_menu)->procesaTeclas(keys);
	} else if (_estado == NIVEL) {
		if (_naveEspacial != NULL) ((cNaveEspacial*)_naveEspacial)->procesaTeclas(keys);
	}
}


// aplicar la logica en este orden:
//   1 - decorado (se mueve)
//   2 - items y disparos
//   3 - nave y enemigos
void cSistema::logicaNivel() {
	_nivel->logica();
}

void cSistema::logicaMenu() {
	if (_menu == NULL) {
		_naveEspacial = new cNaveEspacial(this);
		_menu = new cMenu(this);
	}
	_menu->logica();
}

void cSistema::avanzaNivel() {
	_estado = MENU;
	cMenu* menu = (cMenu*)_menu;
	if (_nivel != NULL) {
		cNivel* nivel = (cNivel*)_nivel;
		menu->setScore(_numNivel, nivel->puntos());
		int posicionNivel = nivel->getPosicion();
		_naveEspacial->offset(-posicionNivel, 0);
		delNivel();
	}
	menu->setPantalla(PANTALLA_SCORE);
	++_numNivel;
}

void cSistema::arrancaPartida() {
	// crear una nave y nivel frescos
	delNaveEspacial();
	_naveEspacial = new cNaveEspacial(this);

	_numNivel = 1;
	cargaNivel();

	// posicionar la nave
	int x, y;
	((cNivel*)_nivel)->posicionRespawn(x, y);
	((cNaveEspacial*)_naveEspacial)->renace(x, y);
}

void cSistema::gameOver() {
	_estado = MENU;
	cMenu* menu = (cMenu*)_menu;
	menu->setPantalla(PANTALLA_GAME_OVER);
}


// freir el nivel que habia y poner el que hay ahora
void cSistema::cargaNivel() {
	_estado = NIVEL;
	delNivel();
	if (_numNivel == 1) {
		_nivel = new cNivel1(this, (cNaveEspacial*)_naveEspacial, 
							328, 28, "maps\\level-01.csv", 
							TEX_NIVEL1, TEX_FONDO1, 
							"maps\\stage1-01.png", 
							"img\\Outer-Space-Wallpaper.png");
	} else if (_numNivel == 2) {
		_nivel = new cNivel2(this, (cNaveEspacial*)_naveEspacial, 
							288, 28, "maps\\level-02.csv", 
							TEX_NIVEL2, TEX_FONDO2, 
							"maps\\textura-nivel2.png", 
							"img\\level2-back.png");
	} else if (_numNivel == 3) {
		_nivel = new cNivel3(this, (cNaveEspacial*)_naveEspacial, // funcionaaa yuhuuu!!!!
							328, 28, "maps\\level-03.csv", 
							TEX_NIVEL3, TEX_FONDO3, 
							"maps\\stage3-03.png", 
							"img\\Outer-Space-Wallpaper.png");
	} else if (_numNivel == 4) {

		// partida completada
		cMenu* menu = (cMenu*)_menu;
		cNaveEspacial* nave = (cNaveEspacial*)_naveEspacial;
		
		_estado = MENU;

		// hi-score?
		long long puntos = nave->puntos();
		if (menu->esHiScore(puntos)) {
			menu->setHiScore(puntos);
			menu->setPantalla(PANTALLA_SET_HI_SCORE);
		} else {
			menu->setPantalla(PANTALLA_COMPLETE);
		}
	}
}

void cSistema::continuePartida() {
	// crear una nave y nivel frescos
	delNaveEspacial();
	_naveEspacial = new cNaveEspacial(this);

	cargaNivel();

	// posicionar la nave
	int x, y;
	((cNivel*)_nivel)->posicionRespawn(x, y);
	((cNaveEspacial*)_naveEspacial)->renace(x, y);
}

void cSistema::logica() {
	if (_estado == MENU) logicaMenu();
	else if (_estado == NIVEL) logicaNivel();
}


void cSistema::pintaMenu() {
	if (_menu != NULL) _menu->pinta();
}

void cSistema::pintaNivel() {
	if (_nivel != NULL) _nivel->pinta();
}

void cSistema::pinta() {
	if (_estado == MENU) pintaMenu();
	else if (_estado == NIVEL) pintaNivel();
}
