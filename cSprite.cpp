
#include "cSprite.h"


cSprite::cSprite() {

}

cSprite::cSprite(int x, int y) {
	setPosicion(x, y);
}


void cSprite::getPosicion(int &x, int &y) {
	x = _x;
	y = _y;
}

void cSprite::setPosicion(int x, int y) {
	_x = x;
	_y = y;
}

