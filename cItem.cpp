
#include "cItem.h"
#include "cNivel.h"
#include <gl/glut.h>



int itemTex[8][4] = {
	{ 6, 5, 16, 16 },
	{ 34, 5, 16, 16 },
	{ 64, 5, 16, 16 },
	{ 6, 32, 16, 16 },
	{ 35, 32, 16, 16 },
	{ 63, 31, 16, 16 },
	{ 92, 32, 16, 16 },
	{ 120, 32, 16, 16 }
};

cItem::cItem(cSistema* sis, int x, int y, int tipo) : cSprite(sis, x, y) {
	_sis->cargaTextura(TEX_ITEMS, "img\\items.png");
	_tipo = tipo;
}

int cItem::tipo() const {
	return _tipo;
}

void cItem::muerete() {
	// desaparecer inmediatamente
	_muerto = true;
}

void cItem::caja(cRect& rect) const {
	rect.w = itemTex[_tipo][2];
	rect.h = itemTex[_tipo][3];
	rect.x = _x - (rect.w>>1);
	rect.y = _y - (rect.h>>1);
}

void cItem::colision(cRect &rect, int &colMask) const {
	colMask = 0;
	if (!_muerto) {
		cRect myRect;
		caja(myRect);
		if (myRect.x < rect.x+rect.w && myRect.x+myRect.w > rect.x &&
			myRect.y < rect.y+rect.h && myRect.y+myRect.h > rect.y) {
				colMask = 1;
		}
	}
}

void cItem::logica() {
	// matar el item cuando queda fuera de la pantalla
	cNivel* nivel = (cNivel*)_sis->nivel();
	cRect rect;
	caja(rect);
	if (nivel->fueraLimites(rect)) {
		_muerto = true;
		return;
	}
}

void cItem::pinta() const {
	int wTex, hTex;
	_sis->tamanoTextura(TEX_ITEMS, wTex, hTex);
	int tex = _sis->idTextura(TEX_ITEMS);
	float xTexIte = itemTex[_tipo][0] / (float)wTex;
	float yTexIte = itemTex[_tipo][1] / (float)hTex;
	float wTexIte = itemTex[_tipo][2] / (float)wTex;
	float hTexIte = itemTex[_tipo][3] / (float)hTex;
	int wPixIte = itemTex[_tipo][2];
	int hPixIte = itemTex[_tipo][3]; 
	int xPixIte = _x - (wPixIte>>1);
	int yPixIte = int(GAME_HEIGHT - _y - (hPixIte>>1));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	glTexCoord2f(xTexIte, yTexIte + hTexIte);			glVertex2i(xPixIte, yPixIte);
	glTexCoord2f(xTexIte + wTexIte, yTexIte + hTexIte);	glVertex2i(xPixIte + wPixIte, yPixIte);
	glTexCoord2f(xTexIte + wTexIte, yTexIte);			glVertex2i(xPixIte + wPixIte, yPixIte + hPixIte);
	glTexCoord2f(xTexIte, yTexIte);						glVertex2i(xPixIte, yPixIte + hPixIte);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

