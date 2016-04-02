#pragma once


// includes
#include "cSistema.h"




class cHud : public cSprite {
protected:

	long long _tiempoVida;
	
	void pintaFondo() const;
	void pintaPuntos() const;
	void pintaVidas() const;
	void pintaMagias() const;
	void pintaBarra() const;

public:
	cHud(cSistema* sis, int x, int y);
	
	void logica();
	void pinta() const;

};
