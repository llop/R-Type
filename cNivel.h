#pragma once


// includes
#include "cSistema.h"
#include "cNaveEspacial.h"
#include "cItem.h"
#include "cEnemigo.h"
#include "cDisparo.h"
#include "cEscudo.h"
#include "cHud.h"

#include <list>
#include <vector>
using namespace std;


class cNivel : public cSprite {
protected:

	int _posicion;							// posicion del scroll
	int _posicionFinal;						// final del scroll

	cNaveEspacial* _naveEspacial;			// nave espacial
	list<cItem*> _items;					// items
	list<cEnemigo*> _enemigos;				// enemigos
	list<cDisparo*> _disparos;				// disparos
	list<cEscudo*> _escudos;				// escudos
	cHud* _hud;
	

	int _tilesAncho, _tilesAlto;			// dimensiones del mapa en tiles
	
	int _idTextura, _idFondo;							// 
	int _tilesAnchoTex, _tilesAltoTex;		// dimensiones de la textura en tiles

	vector<int> _mapa;						// el mapa de tiles


	bool cargaMapa(const char* ficheroMapa);

	virtual int avanzaPosicion() = 0;		// se encarga del scroll
	virtual void generaEnemigos() {};		// genera los enemigos del nivel

	void aplicaScroll();
	void aplicaLogicas();		// hace que todo el mundo corra su lógica
	void aplicaMuertes();		// maneja las muertes de los sprites

public:
	cNivel(cSistema* sis, cNaveEspacial* naveEspaciall, 
			int tilesAncho, int tilesAlto,
			const char* ficheroMapa, 
			int idTextura, int idFondo, 
			const char* ficheroTextura,
			const char* ficheroFondo);
	~cNivel();
	

	int getPosicion() const;
	//void setDimensionesMapa(int tilesAncho, int tilesAlto);
	void getDimensionesMapa(int &tilesAncho, int &tilesAlto) const;
	

	// consultar
	const list<cItem*> items() const;
	const list<cEnemigo*> enemigos() const;
	const list<cDisparo*> disparos() const;
	const list<cEscudo*> escudos() const;
	
	// establecer
	void pushItem(cItem* item);
	void pushEnemigo(cEnemigo* enemigo);
	void pushDisparo(cDisparo* disparo);
	void pushEscudo(cEscudo* escudo);

	virtual void posicionRespawn(int &x, int &y) {}

	// para el nivel la caja es el rectangulo que representa las coordenadas de la pantalla
	void caja(cRect &rect) const;
	
	// rect: miramos si este rectangulo ha chocado contra algo del nivel
	// colisionMask: indica si ha chocado arriba, abajo, izquierda o derecha
	//               es una mascara de bits (COLISION_ARRIBA, COLISION_ABAJO, COLISION_IZQ, COLISION_DER)
	//               por ejemplo, para saber is ha chocado la base del rectangulo:
	//                 colisionMask & COLISION_ABAJO
	// x, y: las coordenadas del choque, sólo si se ha producido
	// objeto: si hubo colision, el tipo de objeto contra el que ha chocado el rectangulo
	//         puede ser un tile (COLISION_TILE) o los limites de la pantalla (COLISION_PANTALLA)
	void colisionNivel(const cRect &rect, int &colisionMask, int &x, int &y, int &objeto);

	void colision(cRect &caja, int &colMask) const;

	// determina si un rectangulo queda fuera de la pantalla
	bool fueraLimites(cRect &caja) const;

	void logica();
	void pinta() const;

};



#define NIVEL_AVANCE 1
#define NIVEL_DELAY 1

class cNivel1 : public cNivel {
protected:

	int avanzaPosicion();
	void generaEnemigos();

public:

	cNivel1(cSistema* sis, cNaveEspacial* naveEspaciall, 
			int tilesAncho, int tilesAlto, 
			const char* ficheroMapa, 
			int idTextura, int idFondo, 
			const char* ficheroTextura,
			const char* ficheroFondo);

	void posicionRespawn(int &x, int &y);
};



#define NIVEL2_AVANCE 1
#define NIVEL2_DELAY 1

class cNivel2 : public cNivel {
protected:

	int avanzaPosicion();
	void generaEnemigos();

public:

	cNivel2(cSistema* sis, cNaveEspacial* naveEspaciall, 
			int tilesAncho, int tilesAlto, 
			const char* ficheroMapa, 
			int idTextura, int idFondo, 
			const char* ficheroTextura,
			const char* ficheroFondo);

	void posicionRespawn(int &x, int &y);
};