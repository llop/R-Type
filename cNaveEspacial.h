#pragma once


// includes
#include "cSistema.h"
#include "cEscudo.h"
#include <vector>
using namespace std;


#define NAVE_VIVE 0
#define NAVE_EXPLO 1
#define NAVE_MUERE 2

#define NAVE_BAJA_MAS 0
#define NAVE_BAJA 1
#define NAVE_LADO 2
#define NAVE_SUBE 3
#define NAVE_SUBE_MAS 4

#define NAVE_EXPLO_FRAMES 8

#define NAVE_MUEVE_DELAY 6
#define NAVE_EXPLO_DELAY 6

#define NAVE_PIXELS_MUEVE 3.0f
#define NAVE_PIXELS_MUEVE_MAX 6.0f
#define NAVE_PIXELS_MUEVE_INC 0.5f

#define NAVE_TIRO_ESCUDO_DELAY 4
#define NAVE_TIRO_DELAY 4
#define NAVE_TIRO_CIR_DELAY 16
#define NAVE_TIRO_FACTOR 1
#define NAVE_FLASH_DELAY 4
#define NAVE_FACTOR_CARGA_TIRO 4 
#define NAVE_MAX_CARGA_TIRO 5

#define NAVE_VIDAS_INICIO 3
#define NAVE_VIDA_INICIAL 100
#define NAVE_MAGIAS_INICIAL 3

#define NAVE_SIN_ESCUDO 0
#define NAVE_ESCUDO1 1
#define NAVE_ESCUDO2 2
#define NAVE_ESCUDO3 3

#define NAVE_ESCUDO_SEC_X_OFFSET 8
#define NAVE_ESCUDO_SEC_Y_OFFSET 32

#define NAVE_TIEMPO_INVENCIBLE 240
#define NAVE_TIEMPO_PARPADEO 3

#define NAVE_PUNTOS_VIDA 1000

#define NAVE_MAGIA_DELAY 240


class cNaveEspacial : public cSprite {
private:

	int _vidas;					// cuantas vidas quedan
	int _vida;					// cuantos toques le quedan a la nave
	int _magias;				// cuantas magias quedan
	long long _tiempoVida;		// cuanto tiempo lleva viva la nave

	int _tipoTiro;
	long long _tiroDelay;
	bool _tiroPulsado;		
	long long _ultimoTiro;		// cuando disparo por ultima vez
	long long _ultimoTiroEscudo;
	long long _ultimaMagia;
	int _cargaTiro;

	float _pixelsMueve;

	long long _puntos;			// la puntuacion

	float _xF, _yF;
	int _lastX, _lastY;

	int _nivelEscudos;
	vector<cEscudo*> _escudos;

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

	void anadeEscudo();
	void lanzaEscudo();
	void delEscudos();

public:
	cNaveEspacial(cSistema* sis);
	~cNaveEspacial();

	void sumaPuntos(long long puntos);
	long long puntos() const;
	int vidas() const;
	int magias() const;
	bool puedeTirarMagia() const;
	int cargaDisparo() const;

	vector<cEscudo*>& escudos();

	void renace(int x, int y);
	void muerete();
	void caja(cRect &rect) const;
	void offset(int x, int y);

	// solo necesita procesar las teclas
	void procesaTeclas(unsigned char* keys);
	void logica();
	void pinta() const;

};
