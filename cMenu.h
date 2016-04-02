#pragma once

#include "cSistema.h"
#include <vector>
#include <string>
using namespace std;


// _state
#define PANTALLA_INICIO 0
#define PANTALLA_INSTRUCCIONES 1
#define PANTALLA_CREDITOS 2
#define PANTALLA_HI_SCORE 3
#define PANTALLA_GAME_OVER 4
#define PANTALLA_SCORE 5
#define PANTALLA_COMPLETE 6
#define PANTALLA_SET_HI_SCORE 7

// inicio:
// 1 - start game
// 2 - dificultad (opciones?)
// 3 - instrucciones
// 4 - creditos
#define INICIO_START 0
#define INICIO_DIFICULTAD 1
#define INICIO_INSTRUCCIONES 2
#define INICIO_CREDITOS 3
#define INICIO_HI_SCORE 4
#define INICIO_NUM_OPCIONES 5

#define INSTRUCCIONES_VUELVE 0
#define CREDITOS_VUELVE 0
#define HI_SCORE_VUELVE 0

#define GAME_OVER_CONTINUE 0
#define GAME_OVER_QUIT 1
#define GAME_OVER_NUM_OPCIONES 2

#define MENU_INTERVALO_INPUT 16

#define MENU_ENTER_KEY 13

#define MENU_TIEMPO_SCORE 480

#define MENU_AVANCE_NAVE 2

#define MENU_COMPLETE_RESPONSIVE 300
#define MENU_SET_HI_SCORE_RESPONSIVE 300

#define MENU_NUM_HI_SCORES 3
#define MENU_HI_SCORE_NAME_LEN 8
#define MENU_HI_SCORE_DEF_NAME "aaa     "


class cMenu : public cSprite {
protected:
	
	long long _tiempoVida;
	long long _ultimoInput;
	long long _tiempoScore;
	long long _tiempoComplete;
	long long _tiempoSetHiScore;

	int _dificultad;
	int _seleccionado;
	int _continua;

	int _nivel;
	long long _puntos;
	
	// hi-score
	const char* _ficheroHiScore;
	vector<pair<string, long long> > _hiScores;

	int _hiScoreNameChar;
	string _hiScoreNameTmp;
	long long _hiScorePointsTmp;

	bool cargaHiScore();
	bool guardaHiScore();
	
	void pintaString(int x, int y, int* alfabeto, const string &str) const;
	void pintaStringCentrado(int y, int* alfabeto, const string &str) const;
	void pintaHiScoreString(int x, int y, int* alfabeto, const string &str) const;
	void pintaNumero(int x, int y, int* alfabeto, long long num) const;


public:
	cMenu(cSistema* sis);

	// modificadoras
	void setDificultad(int dificultad);
	void setPantalla(int pantalla);
	void setScore(int nivel, long long puntos);
	void setHiScore(long long puntos);

	int dificultad() const;

	bool esHiScore(long long puntos) const;

	void procesaTeclas(unsigned char* keys);
	void logica();
	void pinta() const;
};

