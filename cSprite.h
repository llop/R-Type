#pragma once


#include "cData.h"

/**
 * Un sprite es un coso que se tiene que implementar
 */
class cSistema;
class cSprite {
protected:

	int _x, _y;		// coordenadas
	cData _data;


public:
	cSprite();
	cSprite(int x, int y);

	virtual ~cSprite() {};		// las subclases deben implementar el destructor


	void getPosicion(int &x, int &y);
	void setPosicion(int x, int y);


	// esta mierda que la implementen las subclases
	virtual void logica(const cSistema &sis) {};
	virtual void pinta() const {};

};