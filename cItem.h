#pragma once


// includes
#include "cSistema.h"



#define ITEM_VELOCIDAD 0
#define ITEM_DISPARO_RB 5
#define ITEM_VIDA 4
#define ITEM_ESCUDO 3

#define ITEM_VELOCIDAD_PUNTOS 4
#define ITEM_DISPARO_RB_PUNTOS 2
#define ITEM_VIDA_PUNTOS 5
#define ITEM_ESCUDO_PUNTOS 1

class cItem : public cSprite {
protected:
	int _tipo;
	long long _puntos;

public:
	cItem(cSistema* sis, int x, int y, int tipo);

	int tipo() const;
	long long puntos() const;

	void muerete();

	void colision(cRect &rect, int &colMask) const;
	void caja(cRect& rect) const;
	void logica();
	void pinta() const;
};
