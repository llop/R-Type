#pragma once


// includes
#include "cSistema.h"


#define DISPARO_VIVE 0
#define DISPARO_EXPLO 1
#define DISPARO_MUERE 2


class cDisparo : public cSprite {
protected:

	bool _malo;		// si es malo, lo ha tirado un enemigo
	int _dano;		// cuanta vida quita

public:
	cDisparo(cSistema* sis) : cSprite(sis) {};
	cDisparo(cSistema* sis, int x, int y) : cSprite(sis, x, y) {};
	virtual ~cDisparo() {};

	bool malo() const { return _malo; };
	int dano() const { return _dano; };

	bool vive() const { return _state==DISPARO_VIVE; }

	virtual void explota() {};
};


#define DISPARO_NAVE_DELAY 6
#define DISPARO_NAVE_CIR_DELAY 2
#define DISPARO_EXPLO_DELAY 6

#define DISPARO_NAVE_NORMAL 0
#define DISPARO_NAVE_CIRCULAR 1

// disparo de la nave
class cDisparoNave : public cDisparo {
protected:

	int _tipo;			// normal o circular
	int _tamano;		// tamano del proyectil
	int _pixelsAvanza;	// cuantos pixels avanza por frame

public:
	cDisparoNave(cSistema* sis, int x, int y, int tipo, int tamano);
	~cDisparoNave();

	void colision(cRect &rect, int &colMask) const;
	void caja(cRect &rect) const;

	void explota();
	void muerete();

	void logica();
	void pinta() const;
};

#define DISPARO_ENEMIGO_DELAY 4
#define DISPARO_ENEMIGO_VIVE_FRAMES 4
#define DISPARO_ENEMIGO_EXPLO_FRAMES 5
#define DANO_DISPARO_ENEMIGO 100
#define VELOCIDAD_DISPARO_ENEMIGO 3

// disparo enemigo
class cDisparoEnemigo : public cDisparo {
protected:

	float _xF, _yF;
	float _pixelsAvanzaX;
	float _pixelsAvanzaY;

public:
	cDisparoEnemigo(cSistema* sis, int xene, int yene, float xu, float yu);
	~cDisparoEnemigo();
	
	void colision(cRect &rect, int &colMask) const;
	void caja(cRect &rect) const;

	void explota();
	void muerete();

	void logica();
	void pinta() const;
};


// disparo enemigo 3
#define DISPARO_ENEMIGO3_EXPLO_FRAMES 5
#define DISPARO_ENEMIGO3_EXPLO_DELAY 6

class cDisparoEnemigo3 : public cDisparo {
protected:

	float _xF, _yF;
	float _pixelsAvanzaX;
	float _pixelsAvanzaY;

public:
	cDisparoEnemigo3(cSistema* sis, int xene, int yene, float xu, float yu);
	~cDisparoEnemigo3();
	
	void colision(cRect &rect, int &colMask) const;
	void caja(cRect &rect) const;

	void explota();
	void muerete();

	void logica();
	void pinta() const;
};



#define DISPARO_ESCUDO1_FRENTE 0
#define DISPARO_ESCUDO1_ARRIBA 1
#define DISPARO_ESCUDO1_ABAJO 2
#define DISPARO_ESCUDO2_ARRIBA 3
#define DISPARO_ESCUDO2_ABAJO 4

#define DISPARO_ESCUDO_AVANCE 15
#define DISPARO_ESCUDO_DANO 20

class cDisparoEscudo : public cDisparo {
protected:

	int _tipo;


public:
	cDisparoEscudo(cSistema* sis, int x, int y, int tipo);
	~cDisparoEscudo();

	void colision(cRect &rect, int &colMask) const;
	void caja(cRect &rect) const;

	void explota();
	void muerete();

	void logica();
	void pinta() const;
};


// disparo jefe1

#define VELOCIDAD_DISPARO_JEFE1 12
#define DISPARO_JEFE1_DELAY 6
#define DANO_DISPARO_JEFE1 500

class cDisparoJefe1 : public cDisparo {
protected:

	float _xF, _yF;
	float _xVec, _yVec;

public:
	cDisparoJefe1(cSistema* sis, int x, int y, float xVec, float yVec);
	~cDisparoJefe1();
	
	void colision(cRect &rect, int &colMask) const;
	void caja(cRect &rect) const;

	void explota();
	void muerete();

	void logica();
	void pinta() const;
};