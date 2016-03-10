
#include "Globals.h"
#include "cSistema.h"
#include "cNaveEspacial.h"
#include "cNivel.h"



cSistema::cSistema() {
	_naveEspacial = NULL;
	_nivel = NULL;
	_data = new cData();
	_estado = MENU;
}

cSistema::~cSistema() {
	delete _data;
	_data = NULL;
	if (_naveEspacial != NULL) {
		delete _naveEspacial;
		_naveEspacial = NULL;
	}
	if (_nivel != NULL) {
		delete _nivel;
		_nivel = NULL;
	}
}


void cSistema::cargaTextura(int id, const char* ficheroTextura) {
	_data->LoadImage(id, ficheroTextura, GL_RGBA);
}

int cSistema::getIdTextura(int id) const {
	return _data->GetID(id);
}

void cSistema::getTamanoTextura(int id, int &width, int &height) const {
	_data->GetSize(id, &width, &height);
}


// consultar
cSprite* cSistema::getNaveEspacial() const {
	return _naveEspacial;
}

cSprite* cSistema::getNivel() const {
	return _nivel;
}


// establecer
void cSistema::setNaveEspacial(cSprite* naveEspacial) {
	_naveEspacial = naveEspacial;
}

void cSistema::setNivel(cSprite* nivel) {
	_nivel = nivel;
}

// eliminar
void cSistema::delNaveEspacial() {
	_naveEspacial = NULL;
}

void cSistema::delNivel() {
	_nivel = NULL;
}


// esto para el game loop
void cSistema::procesaTeclas(unsigned char *keys) {
	if (_naveEspacial != NULL) {
		((cNaveEspacial*)_naveEspacial)->procesaTeclas(keys);
	}
}


// aplicar la logica en este orden:
//   1 - decorado (se mueve)
//   2 - items y disparos
//   3 - nave y enemigos
void cSistema::logicaNivel() {
	((cNivel*)_nivel)->logica();
}

void cSistema::logicaMenu() {

	_estado = NIVEL;
	_numNivel = 1;

	// carga nivel 1
	_naveEspacial = new cNaveEspacial(this);
	_nivel = new cNivel(this, (cNaveEspacial*)_naveEspacial, 72, 30, 16, 16, "level-01.csv", "img\\blocks.png");

}

void cSistema::logica() {
	if (_estado == MENU) logicaMenu();
	else if (_estado == NIVEL) logicaNivel();
}


void cSistema::pintaMenu() {

}

void cSistema::pintaNivel() {
	_nivel->pinta();
}

void cSistema::pinta() {
	if (_estado == MENU) pintaMenu();
	else if (_estado == NIVEL) pintaNivel();
}
