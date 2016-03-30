#pragma once

#include "cSistema.h"
#include <string>
using namespace std;


// _state
#define PANTALLA_INICIO 0
#define PANTALLA_INSTRUCCIONES 1
#define PANTALLA_CREDITOS 2
#define PANTALLA_GAME_OVER 3
#define PANTALLA_SCORE 4

// inicio:
// 1 - start game
// 2 - dificultad (opciones?)
// 3 - instrucciones
// 4 - creditos
#define INICIO_START 0
#define INICIO_DIFICULTAD 1
#define INICIO_INSTRUCCIONES 2
#define INICIO_CREDITOS 3
#define INICIO_NUM_OPCIONES 4

#define INSTRUCCIONES_VUELVE 0

#define CREDITOS_VUELVE 0

#define GAME_OVER_CONTINUE 0
#define GAME_OVER_QUIT 1
#define GAME_OVER_NUM_OPCIONES 2

#define MENU_INTERVALO_INPUT 20

#define MENU_ENTER_KEY 13

#define MENU_TIEMPO_SCORE 1000

#define MENU_AVANCE_NAVE 2


class cMenu : public cSprite {
protected:
	
	long long _tiempoVida;
	long long _ultimoInput;
	int _tiempoScore;

	int _dificultad;
	int _seleccionado;
	int _continua;

	int _nivel;
	long long _puntos;
	
	void pintaString(int x, int y, int* alfabeto, const string &str) const;
	void pintaNumero(int x, int y, int* alfabeto, long long num) const;


public:
	cMenu(cSistema* sis);

	// modificadoras
	void setDificultad(int dificultad);
	void setPantalla(int pantalla);
	void setScore(int nivel, long long puntos);

	int dificultad() const;

	void procesaTeclas(unsigned char* keys);
	void logica();
	void pinta() const;
};

