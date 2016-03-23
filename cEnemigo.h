#pragma once


// includes
#include "cSistema.h"


#define ENEMIGO_VIVE 0
#define ENEMIGO_EXPLO 1
#define ENEMIGO_MUERE 2

class cEnemigo : public cSprite {
protected:
	
	int _vida;				// vida del enemigo
	long long _puntos;		// los puntos que sumara si la nave lo mata

public:
	//cEnemigo(cSistema* sis) : cSprite(sis) {};
	cEnemigo(cSistema* sis, int x, int y) : cSprite(sis, x, y) {};

	long long puntos() const { return _puntos; };

};

#define ENEMIGO1_ALT_MOV 50
#define ENEMIGO1_SPEED_LEFT 2
#define ENEMIGO1_INC_ANGLE 0.04f
#define ENEMIGO1_NUM_FRAMES 8
#define ENEMIGO1_MUEVE_DELAY 10
#define ENEMIGO1_MUERE_DELAY 10
#define ENEMIGO1_VIDA_INICIAL 20
#define ENEMIGO1_EXPLO_FRAMES 6
#define ENEMIGO1_PUNTOS 1

class cEnemigo1 : public cEnemigo {
private:

	float _angle;
	int _yBase;

public:
	//cEnemigo1(cSistema* sis);
	cEnemigo1(cSistema* sis, int x, int y);
	~cEnemigo1();

	void muerete();
	void caja(cRect &rect) const;
	
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};


#define ENEMIGO2_ALT_MOV 50
#define ENEMIGO2_SPEED_LEFT 2
#define ENEMIGO2_INC_ANGLE 0.04f
#define ENEMIGO2_NUM_FRAMES 8
#define ENEMIGO2_MUEVE_DELAY 10
#define ENEMIGO2_MUERE_DELAY 10
#define ENEMIGO2_VIDA_INICIAL 20
#define ENEMIGO2_EXPLO_FRAMES 6
#define ENEMIGO2_PUNTOS 1

class cEnemigo2 : public cEnemigo {
private:

	float _angle;
	int _yBase;

public:
	cEnemigo2(cSistema* sis, int x, int y);
	~cEnemigo2();

	void caja(cRect &rect) const;

	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};



//--------------------------------------------------------------------
// Enemigo que suelta item
//--------------------------------------------------------------------

#define ENEMIGO_ITEM_VUELA 0
#define ENEMIGO_ITEM_ANDA 1

#define ENEMIGO_ITEM_AVANCE 1

#define ENEMIGO_ITEM_VIDA_INICIAL 20
#define ENEMIGO_ITEM_PUNTOS 2

#define ENEMIGO_ITEM_MUEVE_DELAY 10
#define ENEMIGO_ITEM_MUERE_DELAY 10

#define ENEMIGO_ITEM_VUELA_NUM_FRAMES 2
#define ENEMIGO_ITEM_ANDA_NUM_FRAMES 4

class cEnemigoItem : public cEnemigo {
private:

	int _subState;
	float _xF, _yF;
	float _ava;				// pixels que avanza a pie
	float _xVec, _yVec;		// distancia que avanza volando
	int _item;

public:
	cEnemigoItem(cSistema* sis, int x, int y, 
					float xVec, float yVec, 
					int item);
	~cEnemigoItem();

	void offset(int x, int y);

	void muerete();
	void caja(cRect &rect) const;
	
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};



