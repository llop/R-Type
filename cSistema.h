#pragma once


// includes
#include "cNaveEspacial.h"
#include "cData.h"
#include "cNivel.h"

#include <list>
using namespace std;

#define MENU 0
#define NIVEL 1


// maneja toda la interaccion:
//   nave espacial
//   items
//   enemigos
//   disparos
//   decorado
class cSistema {
private:

	cNaveEspacial* _naveEspacial;
	cNivel* _nivel;

	int _estado;
	int _numNivel;
	
	void logicaMenu();
	void logicaNivel();
	
	void pintaMenu();
	void pintaNivel();


public:
	cSistema();
	~cSistema();

	
	cNaveEspacial* getNaveEspacial() const;
	void setNaveEspacial(cNaveEspacial* naveEspacial);
	void delNaveEspacial();

	cNivel* getNivel() const;
	void setNivel(cNivel* nivel);
	void delNivel();



	// esto para el game loop
	void procesaTeclas(unsigned char *keys);
	void logica();
	void pinta();

};
