#pragma once


// includes
#include "cSistema.h"


class cDisparo : public cSprite {
protected:

	bool _malo;		// si es malo, lo ha tirado un enemigo
	int _dano;		// cuanta vida quita
	int _seq;		// numero del frame en la secuencia
	int _delay;		// tiempo entre frames

public:
	cDisparo(cSistema* sis) : cSprite(sis) {};
	cDisparo(cSistema* sis, int x, int y) : cSprite(sis, x, y) {};
	virtual ~cDisparo() {};

	bool malo() { return _malo; };
	int dano() { return _dano; };
};


#define DISPARO_NAVE_DELAY 10

// disparo de la nave
class cDisparoNave : public cDisparo {
protected:

	int _tamano;		// tamano del proyectil
	int _pixelsAvanza;	// cuantos pixels avanza por frame

public:
	cDisparoNave(cSistema* sis, int x, int y, int tamano);
	~cDisparoNave();

	void getCaja(cRect &rect) const;

	void logica();
	void pinta() const;
};

#define DISPARO_ENEMIGO_DELAY 10
#define DANO_DISPARO_ENEMIGO 100
#define VELOCIDAD_DISPARO_ENEMIGO 2

// disparo enemigo
class cDisparoEnemigo : public cDisparo {
protected:

	float _pixelsAvanzaX;
	float _pixelsAvanzaY;

public:
	cDisparoEnemigo(cSistema* sis, int xene, int yene, float xu, float yu);
	~cDisparoEnemigo();

	void getCaja(cRect &rect) const;

	void logica();
	void pinta() const;
};