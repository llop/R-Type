#pragma once


// includes
#include "cSistema.h"
#include <list>
using namespace std;


#define ENEMIGO_VIVE 0
#define ENEMIGO_EXPLO 1
#define ENEMIGO_MUERE 2

class cEnemigo : public cSprite {
protected:
	
	int _vida;				// vida del enemigo
	long long _puntos;		// los puntos que sumara si la nave lo mata

public:
	cEnemigo(cSistema* sis) : cSprite(sis) {};
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
#define ENEMIGO2_NUM_FRAMES 16
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

	void muerete();
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



struct cExplo {
	int x, y;
	int seq, delay;
};

//----------------------------------------------------------------------
// jefe nivel 1
//----------------------------------------------------------------------

#define JEFE1_MUEVE_DELAY 10
#define JEFE1_VIDA_INICIAL 100
#define JEFE1_PUNTOS 35

#define JEFE1_NUM_FRAMES 24

#define JEFE1_ALIEN_DENTRO 0
#define JEFE1_ALIEN_SALIENDO 1
#define JEFE1_ALIEN_FUERA 2
#define JEFE1_ALIEN_VOMITA 3

#define JEFE1_TIRO_DELAY 400

#define JEFE1_INC_ANG_COLA 0.02f
#define JEFE1_INC_MOV 1

#define JEFE1_MUERE_DELAY 10
#define JEFE1_COLA_EXPLO_DELAY 6
#define JEFE1_EXPLO_DELAY 4
#define JEFE1_COLA_EXPLO_NUM_FRAMES 6
#define JEFE1_EXPLO_NUM_FRAMES 9
#define JEFE1_MAX_NUM_EXPLO 5

#define JEFE1_FLASH_IMPACTO 20


class cJefe1 : public cEnemigo {
protected:

	long long _tiempoVida;
	long long _ultimoTiro;
	long long _ultimoImpacto;
	
	int _subState;

	bool _anguloCrece;
	float _anguloCola;

	bool _sube;

	int _seqExplo;
	int _delayExploCola;
	int _bolaExplo;
	
	list<cExplo> _exploCuerpo;
	
	void pintaVivo() const;
	void pintaExplo() const;

public:
	cJefe1(cSistema* sis);
	~cJefe1();

	void muerete();
	void caja(cRect &rect) const;
	void offset(int x, int y);
	
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};



