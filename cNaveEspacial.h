#pragma once


// includes
#include "cSistema.h"
//#include <vector>
//using namespace std;


#define NAVE_VIVE 0
#define NAVE_EXPLO 1
#define NAVE_MUERE 2

#define NAVE_BAJA_MAS 0
#define NAVE_BAJA 1
#define NAVE_LADO 2
#define NAVE_SUBE 3
#define NAVE_SUBE_MAS 4

#define NAVE_EXPLO_FRAMES 8

#define NAVE_MUEVE_DELAY 10
#define NAVE_EXPLO_DELAY 10

#define NAVE_PIXELS_MUEVE 2

#define NAVE_TIRO_DELAY 10
#define NAVE_TIRO_FACTOR 2
#define NAVE_FLASH_DELAY 5

#define NAVE_VIDAS_INICIO 3
#define NAVE_VIDA_INICIAL 100
#define NAVE_MAGIAS_INICIAL 3



class cNaveEspacial : public cSprite {
private:

	int _vidas;				// cuantas vidas quedan
	int _vida;				// cuantos toques le quedan a la nave
	int _magias;			// cuantas magias quedan
	int _tiempo_vida;		// cuanto tiempo lleva viva la nave

	int _state;				// vivo o explotando
	int _seq;				// numero del frame en la secuencia
	int _delay;				// tiempo entre frames

	bool _tiro_pulsado;		
	int _ultimo_tiro;		// cuando disparo por ultima vez
	int _carga_tiro;


	// acciones
	void arriba();
	void abajo();
	void adelante();
	void atras();
	void quieto();
	
	void dispara();
	void no_dispara();
	void tira_magia();

	void reset();

public:
	cNaveEspacial(cSistema* sis);
	~cNaveEspacial();

	void muerete();
	void getCaja(cRect &rect) const;

	// solo necesita procesar las teclas
	void procesaTeclas(unsigned char* keys);

	
	void logica();
	void pinta() const;

};
