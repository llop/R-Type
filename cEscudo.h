#pragma once


// includes
#include "cSistema.h"


#define ESCUDO_MUEVE_DELAY 6
#define ESCUDO_NUM_FRAMES 4
#define ESCUDO_SEC_NUM_FRAMES 12

#define ESCUDO_TIRO_SPEED 16.0f
#define ESCUDO_VUELTA_SPEED 4

#define ESCUDO_TIRO 0
#define ESCUDO_VUELTA 1
#define ESCUDO_ANCLADO 2

#define ESCUDO_DIST_ANCLA 10.0f

#define ESCUDO_DANO 500

#define ESCUDO_FRENTE 0
#define ESCUDO_ARRIBA 1
#define ESCUDO_ABAJO 2


class cEscudo : public cSprite {
protected:

	float _xF, _yF;
	float _xAnt, _yAnt;

	int _dano;
	int _tipo;
	int _numFrames;

public:
	cEscudo(cSistema* sis, int x, int y, int tipo);
	~cEscudo();

	void dispara();
	void lanza();
	void choca();

	bool anclado() const;
	int dano() const;

	void muerete();
	void colision(cRect &rect, int &colMask) const;
	void caja(cRect &rect) const;
	void offset(int x, int y);

	// solo necesita procesar las teclas
	void procesaTeclas(unsigned char* keys);

	
	void logica();
	void pinta() const;
};