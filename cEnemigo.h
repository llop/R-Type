#pragma once


// includes
#include "cSistema.h"
#include <vector>
#include <list>
using namespace std;


#define ENEMIGO_VIVE 0
#define ENEMIGO_EXPLO 1
#define ENEMIGO_MUERE 2

class cEnemigo : public cSprite {
protected:
	
	int _vida;				// vida del enemigo
	long long _puntos;		// los puntos que sumara si la nave lo mata
	bool _esJefe;

public:
	cEnemigo(cSistema* sis) : cSprite(sis), _esJefe(false) {};
	cEnemigo(cSistema* sis, int x, int y) : cSprite(sis, x, y), _esJefe(false) {};

	long long puntos() const { return _puntos; };
	virtual void restaVida(int vida) {
		_vida -= vida;
		if (_vida<=0) {
			((cNaveEspacial*)_sis->naveEspacial())->sumaPuntos(_puntos);
			muerete();
		}
	};
	bool jefe() const { return _esJefe; };
	virtual void explota() {
		muerete();
	};

};

#define ENEMIGO1_ALT_MOV 50
#define ENEMIGO1_SPEED_LEFT 2
#define ENEMIGO1_INC_ANGLE 0.04f
#define ENEMIGO1_NUM_FRAMES 8
#define ENEMIGO1_MUEVE_DELAY 10
#define ENEMIGO1_MUERE_DELAY 10
#define ENEMIGO1_VIDA_INICIAL 20
#define ENEMIGO1_EXPLO_FRAMES 6
#define ENEMIGO1_PUNTOS 1

class cEnemigo1 : public cEnemigo {
private:

	float _angle;
	int _yBase;

public:
	//cEnemigo1(cSistema* sis);
	cEnemigo1(cSistema* sis, int x, int y);
	~cEnemigo1();

	void muerete();
	void caja(cRect &rect) const;
	
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};


#define ENEMIGO2_ALT_MOV 50
#define ENEMIGO2_SPEED_LEFT 2
#define ENEMIGO2_INC_ANGLE 0.04f
#define ENEMIGO2_NUM_FRAMES 16
#define ENEMIGO2_MUEVE_DELAY 10
#define ENEMIGO2_MUERE_DELAY 10
#define ENEMIGO2_VIDA_INICIAL 20
#define ENEMIGO2_EXPLO_FRAMES 6
#define ENEMIGO2_PUNTOS 1

class cEnemigo2 : public cEnemigo {
private:

	float _angle;
	int _yBase;

public:
	cEnemigo2(cSistema* sis, int x, int y);
	~cEnemigo2();

	void muerete();
	void caja(cRect &rect) const;

	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};



//--------------------------------------------------------------------
// Enemigo que suelta item
//--------------------------------------------------------------------

#define ENEMIGO_ITEM_VUELA 0
#define ENEMIGO_ITEM_ANDA 1

#define ENEMIGO_ITEM_AVANCE 1

#define ENEMIGO_ITEM_VIDA_INICIAL 20
#define ENEMIGO_ITEM_PUNTOS 2

#define ENEMIGO_ITEM_MUEVE_DELAY 10
#define ENEMIGO_ITEM_MUERE_DELAY 10

#define ENEMIGO_ITEM_VUELA_NUM_FRAMES 2
#define ENEMIGO_ITEM_ANDA_NUM_FRAMES 4

class cEnemigoItem : public cEnemigo {
private:

	int _subState;
	float _xF, _yF;
	float _ava;				// pixels que avanza a pie
	float _xVec, _yVec;		// distancia que avanza volando
	int _item;

public:
	cEnemigoItem(cSistema* sis, int x, int y, 
					float xVec, float yVec, 
					int item);
	~cEnemigoItem();

	void offset(int x, int y);

	void muerete();
	void caja(cRect &rect) const;
	
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};



struct cExplo {
	int x, y;
	int seq, delay;
};

//----------------------------------------------------------------------
// jefe nivel 1
//----------------------------------------------------------------------

#define JEFE1_MUEVE_DELAY 10
#define JEFE1_VIDA_INICIAL 500
#define JEFE1_PUNTOS 35

#define JEFE1_NUM_FRAMES 24

#define JEFE1_ALIEN_DENTRO 0
#define JEFE1_ALIEN_SALIENDO 1
#define JEFE1_ALIEN_FUERA 2
#define JEFE1_ALIEN_VOMITA 3

#define JEFE1_TIRO_DELAY 400

#define JEFE1_INC_ANG_COLA 0.02f
#define JEFE1_INC_MOV 1

#define JEFE1_MUERE_DELAY 10
#define JEFE1_COLA_EXPLO_DELAY 6
#define JEFE1_EXPLO_DELAY 4
#define JEFE1_COLA_EXPLO_NUM_FRAMES 6
#define JEFE1_EXPLO_NUM_FRAMES 9
#define JEFE1_MAX_NUM_EXPLO 5

#define JEFE1_FLASH_IMPACTO 24

#define JEFE1_TIEMPO_MUERTO 1000


class cJefe1 : public cEnemigo {
protected:

	long long _tiempoVida;
	long long _ultimoTiro;
	long long _ultimoImpacto;
	int _tiempoMuerto;
	
	int _subState;

	bool _anguloCrece;
	float _anguloCola;

	bool _sube;

	int _seqExplo;
	int _delayExploCola;
	int _bolaExplo;
	
	list<cExplo> _exploCuerpo;
	void pintaVivo() const;
	void pintaExplo() const;

public:
	cJefe1(cSistema* sis);
	~cJefe1();

	void restaVida(int vida);
	void muerete();
	void caja(cRect &rect) const;
	void offset(int x, int y);
	
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};





#define ENEMIGO3_INC_ANGLE 0.04f
#define ENEMIGO3_MUEVE_DELAY 10
#define ENEMIGO3_MUERE_DELAY 10
#define ENEMIGO3_VIDA_INICIAL 40
#define ENEMIGO3_EXPLO_FRAMES 6
#define ENEMIGO3_PUNTOS 2
#define ENEMIGO3_MAX_NUM_EXPLO 3

class cEnemigo3 : public cEnemigo {
private:
	
	float _angle;
	float _incAngle;
	float _diffAngle;
	int _numPiezas;
	float _radio;
	bool _antihorario;
	vector<vector<int>> _piezas;
	
	list<cExplo> _exploCuerpo;
	void pintaVivo() const;
	void pintaExplo() const;

public:
	cEnemigo3(cSistema* sis, int x, int y, int numPiezas, float radio, float angle, float incAngle, bool antihorario);
	~cEnemigo3();
	
	void explota();
	void muerete();
	void restaVida(int vida);
	void caja(cRect &rect) const;

	void offset(int x, int y);
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};


//----------------------------------------------------------------------
// jefe nivel 2
//----------------------------------------------------------------------

#define JEFE2_MUEVE_DELAY 10
#define JEFE2_VIDA_INICIAL 2000
#define JEFE2_PUNTOS 65

#define JEFE2_FLASH_IMPACTO 24

#define JEFE2_IDLE 0
#define JEFE2_VULVA_CERRADA 1
#define JEFE2_VULVA_ABIERTA 2

#define JEFE2_MUERE_DELAY 10

#define JEFE2_INTERVALO_ATAQUE1 500
#define JEFE2_INTERVALO_ATAQUE2 800
#define JEFE2_INTERVALO_SALIDA 200
#define JEFE2_INTERVALO_ENTRADA 600

#define JEFE2_INTERVALO_GUSANO1 800
#define JEFE2_TIEMPO_GUSANO1 320
#define JEFE2_INTERVALO_GUSANO2 500
#define JEFE2_TIEMPO_GUSANO2 320

#define JEFE2_TIEMPO_MUERTO 800


class cJefe2 : public cEnemigo {
protected:

	long long _tiempoVida;
	long long _ultimaSalida;
	long long _ultimoAtaque;
	long long _ultimoImpacto;
	int _tiempoMuerto;
	
	vector<cEnemigo3*> _gusanos;
	vector<long long> _ultimaSalidaGusano;
	vector<long long> _tiempoGusano;
	
	int _subState;
	int _seqExplo;
	list<cExplo> _exploCuerpo;
	
	void pintaVivo() const;
	void pintaExplo() const;

public:
	cJefe2(cSistema* sis);
	~cJefe2();

	void restaVida(int vida);
	void muerete();
	void caja(cRect &rect) const;
	void offset(int x, int y);
	
	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};

//----------------------------------------------------------------------
// jefe nivel 3
//----------------------------------------------------------------------
#define JEFE3_MUEVE_DELAY 10
#define JEFE3_VIDA_INICIAL 1000
#define JEFE3_PUNTOS 80

#define JEFE3_MINI_MUEVE_DELAY 10
#define JEFE3_MINI_VIDA_INICIAL 100
#define JEFE3_MINI_PUNTOS 20
#define JEFE3_INC_MOV 1

#define JEFE3_FLASH_IMPACTO 24
#define JEFE3_MINI_FLASH_IMPACTO 24

#define JEFE3_IDLE 0
#define JEFE3_MOVE_UP 1
#define JEFE3_MOVE_DOWN 2
#define JEFE3_MOVE_RIGHT 3
#define JEFE3_MOVE_LEFT 4
#define JEFE3_MINI_ATTACK 5

#define JEFE3_MUERE_DELAY 10
#define JEFE3_MINI_MUERE_DELAY 10
#define JEFE3_EXPLO_DELAY 4
#define JEFE3_EXPLO_NUM_FRAMES 5
#define JEFE3_MINI_NUM_FRAMES 4
#define JEFE3_MINI_EXPLO_NUM_FRAMES 6
#define JEFE3_MAX_NUM_EXPLO 5

#define JEFE3_NUM_MINIS 30
#define JEFE3_INTERVALO_MINIS_ATTACK 200
#define VELOCIDAD_JEFE3_MINI 5

#define JEFE3_TIEMPO_MUERTO 400

class cMiniJefe3 : public cEnemigo {
protected:
	long long _tiempoVida;
	long long _ultimoImpacto;
	int _tiempoMuerto;
	int _subState;
	bool _attacking;
	float _pixelsAvanzaX;
	float _pixelsAvanzaY;

public:	
	cMiniJefe3(cSistema* sis, int x, int y);
	~cMiniJefe3();

	void muerete();
	void caja(cRect &rect) const;
	void restaVida(int vida);
	void colision(cRect &caja, int &colMask) const;
	void offset(int x, int y);
	void moveIt(int x, int y);
	bool isDead();
	void attack();

	void logica();
	void pinta() const;
};


class cJefe3 : public cEnemigo {
protected:

	long long _tiempoVida;
	long long _ultimoImpacto;
	int _tiempoMuerto;

	list<cMiniJefe3*> _minis;
	//vector<long long> _ultimoAtaqueMini;
	//vector<long long> _tiempoMini;

	int _subState;
	int _seqExplo;
	list<cExplo> _exploCuerpo;

	void pintaVivo() const;
	void pintaExplo() const;
	
	void mataMinis();
	void eliminaMinisLista();

public:
	cJefe3(cSistema* sis);
	~cJefe3();
	void creaMinis();
	void restaVida(int vida);
	void muerete();
	void caja(cRect &rect) const;
	void offset(int x, int y);

	void colision(cRect &caja, int &colMask) const;

	void logica();
	void pinta() const;

};

