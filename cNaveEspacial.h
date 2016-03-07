#pragma once


// includes
#include "cSprite.h"
//#include <vector>
//using namespace std;


class cNaveEspacial : public cSprite {
private:

	int _vidas;				// cuantas vidas quedan
	int _vida;				// cuantos toques le quedan a la nave
	int _magias;			// cuantas magias quedan
	int _tiempo_vida;		// cuanto tiempo lleva viva la nave


	// acciones
	void arriba();
	void abajo();
	void adelante();
	void atras();

	void dispara();
	void tira_magia();


public:
	cNaveEspacial();
	~cNaveEspacial();


	// solo necesita procesar las teclas
	void procesaTeclas(unsigned char *keys);

	
	void logica(const cSistema &sis);
	void pinta() const;

};
