
#include "cNaveEspacial.h"
#include <gl/glut.h>




cNaveEspacial::cNaveEspacial() {

}

cNaveEspacial::~cNaveEspacial() {

}




void cNaveEspacial::arriba() {

}

void cNaveEspacial::abajo() {

}

void cNaveEspacial::adelante() {

}

void cNaveEspacial::atras() {

}

void cNaveEspacial::dispara() {

}

void cNaveEspacial::tira_magia() {

}



void cNaveEspacial::procesaTeclas(unsigned char *keys) {
	if (keys['w']||keys['W']) arriba();
	if (keys['s']||keys['S']) abajo();
	if (keys['a']||keys['A']) atras();
	if (keys['d']||keys['D']) adelante();

	if (keys[' ']) dispara();
	if (keys['m']||keys['M']) tira_magia();
}


void cNaveEspacial::logica(const cSistema &sis) {

}

void cNaveEspacial::pinta() const {

}