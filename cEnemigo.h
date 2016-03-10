#pragma once


// includes
#include "cSistema.h"


class cEnemigo : public cSprite {
protected:
	
	int _vida;				// vida del enemigo
	
	int _state;				// vivo o explotando
	int _seq;				// numero del frame en la secuencia
	int _delay;				// tiempo entre frames
	
	void movimiento();

public:
	cEnemigo(cSistema* sis);
	virtual ~cEnemigo() {};


};

#define ENEMIGO1_ALT_MOV 50
#define ENEMIGO1_SPEED_LEFT 2

class cEnemigo1 : public cEnemigo {
private:

	float _angle;			//

public:
	cEnemigo1(cSistema* sis);
	~cEnemigo1();

	void getCaja(cRect &rect) const;

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
