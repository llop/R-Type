#pragma once


// includes
#include "cSistema.h"


#define ENEMIGO_VIVE 0
#define ENEMIGO_EXPLO 1
#define ENEMIGO_MUERE 2

class cEnemigo : public cSprite {
protected:
	
	int _vida;				// vida del enemigo

public:
	//cEnemigo(cSistema* sis) : cSprite(sis) {};
	cEnemigo(cSistema* sis, int x, int y) : cSprite(sis, x, y) {};
	virtual ~cEnemigo() {};


};

#define ENEMIGO1_ALT_MOV 50
#define ENEMIGO1_SPEED_LEFT 2
#define ENEMIGO1_INC_ANGLE 0.04f
#define ENEMIGO1_NUM_FRAMES 8
#define ENEMIGO1_MUEVE_DELAY 10
#define ENEMIGO1_MUERE_DELAY 10
#define ENEMIGO1_VIDA_INICIAL 20
#define ENEMIGO1_EXPLO_FRAMES 6

class cEnemigo1 : public cEnemigo {
private:

	float _angle;			//
	int _yBase;

public:
	//cEnemigo1(cSistema* sis);
	cEnemigo1(cSistema* sis, int x, int y);
	~cEnemigo1();

	void muerete();
	void getCaja(cRect &rect) const;
	
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};


#define ENEMIGO2_SPEED_LEFT 4

class cEnemigo2 : public cEnemigo {
private:



public:
	cEnemigo2(cSistema* sis);
	~cEnemigo2();

	void getCaja(cRect &rect) const;

	void logica();
	void pinta() const;

};
