#pragma once


// includes
#include "cSistema.h"



#define ITEM_VELOCIDAD 0
#define ITEM_DISPARO_RB 5
#define ITEM_VIDA 4
#define ITEM_ESCUDO 3


class cItem : public cSprite {
protected:
	int _tipo;

public:
	cItem(cSistema* sis, int x, int y, int tipo);

	int tipo() const;

	void muerete();

	void colision(cRect &rect, int &colMask) const;
	void caja(cRect& rect) const;
	void logica();
	void pinta() const;
};
