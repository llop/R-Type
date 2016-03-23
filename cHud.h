#pragma once


// includes
#include "cSistema.h"




class cHud : public cSprite {
private:
	
	void pintaFondo() const;
	void pintaPuntos() const;
	void pintaVidas() const;
	void pintaBarra() const;

public:
	cHud(cSistema* sis, int x, int y);
	
	void pinta() const;

};
