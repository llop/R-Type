
#include "cSistema.h"
#include "Globals.h"


//class cSistema {
//private:
//	cNaveEspacial* _naveEspacial;		//   nave espacial
//	list<cItem*> _items;				//   items
//	list<cEnemigo*> _enemigos;			//   enemigos
//	list<cDisparo*> _disparos;			//   disparos
//	cDecorado* _decorado;				//   decorado

cSistema::cSistema() {
	_estado = MENU;
}

cSistema::~cSistema() {

}


// consultar
cNaveEspacial* cSistema::getNaveEspacial() const {
	return _naveEspacial;
}

cNivel* cSistema::getNivel() const {
	return _nivel;
}


// establecer
void cSistema::setNaveEspacial(cNaveEspacial* naveEspacial) {
	_naveEspacial = naveEspacial;
}

void cSistema::setNivel(cNivel* nivel) {
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
	_naveEspacial->procesaTeclas(keys);
}


// aplicar la logica en este orden:
//   1 - decorado (se mueve)
//   2 - items y disparos
//   3 - nave y enemigos
void cSistema::logicaNivel() {
	_nivel->logica(*this);
}

void cSistema::logicaMenu() {

	_estado = NIVEL;
	_numNivel = 1;

	// carga nivel 1
	_naveEspacial = new cNaveEspacial();
	_nivel = new cNivel(_naveEspacial, 72, 30, 16, 16, "level-01.csv");

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
