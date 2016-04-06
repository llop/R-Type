
#include "Globals.h"
#include "cSistema.h"
#include "cNaveEspacial.h"
#include "cMenu.h"
#include "cNivel.h"



cSistema::cSistema() {
	_naveEspacial = NULL;
	_nivel = NULL;
	_numNivel = NIVEL1;
	_dificultad = DIFICULTAD_NORMAL;
	_menu = NULL;
	_data = new cData();
	_sonidos = new cSoundManager();
	_estado = MENU;
	_tiempo = 0;
}

cSistema::~cSistema() {
	if (_data != NULL) {
		delete _data;
		_data = NULL;
	}

	if (_sonidos != NULL) {
		delete _sonidos;
		_sonidos = NULL;
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


void cSistema::cargaSonido(int id, const char* ficheroSonido, bool loop, int num, long long delay) {
	_sonidos->cargaSonido(id, ficheroSonido, loop, num, delay);
}

void cSistema::playSonido(int id) {
	_sonidos->playSonido(id);
}

void cSistema::stopSonido(int id) {
	_sonidos->stopSonido(id);
}

void cSistema::stopSonidos() {
	_sonidos->stopSonidos();
}


// consultar
cSprite* cSistema::naveEspacial() const {
	return _naveEspacial;
}

cSprite* cSistema::nivel() const {
	return _nivel;
}

int cSistema::dificultad() const {
	return _dificultad;
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
		if (_nivel != NULL) ((cNivel*)_nivel)->procesaTeclas(keys);
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
	// detener musicas
	stopSonidos();

	// 
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

	_numNivel = NIVEL2;
	_dificultad = ((cMenu*)_menu)->dificultad();
	cargaNivel();
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
	cNaveEspacial* nave = (cNaveEspacial*)_naveEspacial;
	if (_numNivel == NIVEL1) {
		_nivel = new cNivel1(this, nave, 
							328, 28, "maps\\level-01.csv", 
							TEX_NIVEL1, TEX_FONDO1, 
							"maps\\stage1-01.png", 
							"img\\Outer-Space-Wallpaper.png",
							SOUND_NIVEL1, "wavs\\rtype-004.wav");
	} else if (_numNivel == NIVEL2) {
		_nivel = new cNivel2(this, nave, 
							288, 28, "maps\\level-02.csv", 
							TEX_NIVEL2, TEX_FONDO2, 
							"maps\\textura-nivel2.png", 
							"img\\level2-back.png",
							SOUND_NIVEL2, "wavs\\rtype-004.wav");
	} else if (_numNivel == NIVEL3) {
		_nivel = new cNivel3(this, nave, // funcionaaa yuhuuu!!!!
							328, 28, "maps\\level-03.csv",
							TEX_NIVEL3, TEX_FONDO3,
							"maps\\stage3-03.png",
							"img\\Outer-Space-Wallpaper.png", 
							SOUND_NIVEL3, "wavs\\rtype-004.wav");
	} else if (_numNivel == NIVEL4) {
		_nivel = new cNivel4(this, nave,
							96, 28, "maps\\level-04.csv",
							TEX_NIVEL4, TEX_FONDO4,
							"maps\\level-4.png",
							"img\\level4-back.png",
							SOUND_NIVEL4, "wavs\\rtype-004.wav");
	} else if (_numNivel == NUM_NIVELES) {

		// partida completada
		cMenu* menu = (cMenu*)_menu;
		
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

	if (_nivel != NULL) {
		int x, y;
		nave->getPosicion(x, y);
		nave->renace(x, y);
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
	++_tiempo;

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

	_sonidos->suena();

	if (_estado == MENU) pintaMenu();
	else if (_estado == NIVEL) pintaNivel();
}
