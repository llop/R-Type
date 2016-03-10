
#include "cSistema.h"


cSprite::cSprite(cSistema* sis) {
	_sis = sis;
	_muerto = false;
}

cSprite::cSprite(cSistema* sis, int x, int y) {
	_sis = sis;
	_muerto = false;
	setPosicion(x, y);
}


void cSprite::getPosicion(int &x, int &y) const {
	x = _x;
	y = _y;
}

void cSprite::setPosicion(int x, int y) {
	_x = x;
	_y = y;
}

bool cSprite::muerto() const {
	return _muerto;
}
