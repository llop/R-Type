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
	virtual void trataColisiones() {};		// mira si alguien ha chocado con alguna pared

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

	void caja(cRect &rect) const;
	
	void colision(cRect &caja, int &colMask) const;
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
	void trataColisiones();

public:

	cNivel1(cSistema* sis, cNaveEspacial* naveEspaciall, 
			int tilesAncho, int tilesAlto, 
			const char* ficheroMapa, 
			int idTextura, int idFondo, 
			const char* ficheroTextura,
			const char* ficheroFondo);

};