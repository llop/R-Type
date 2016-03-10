#pragma once


// includes
#include "cSistema.h"
#include "cNaveEspacial.h"
#include "cItem.h"
#include "cEnemigo.h"
#include "cDisparo.h"

#include <list>
#include <vector>
using namespace std;


class cNivel : public cSprite {
private:

	int _posicion;

	cNaveEspacial* _naveEspacial;		// nave espacial
	list<cItem*> _items;				// items
	list<cEnemigo*> _enemigos;			// enemigos
	list<cDisparo*> _disparos;			// disparos
	
	int _tilesAncho, _tilesAlto;		// dimensiones del mapa en tiles
	int _anchoTile, _altoTile;			// dimensiones de un solo tile en pixels

	vector<int> _mapa;					// el mapa de tiles


	bool cargaMapa(int tilesAncho, int tilesAlto, int anchoTile, int altoTile, 
					const char* ficheroMapa);

	void avanzaPosicion();		// se encarga del scroll
	void generaEnemigos();		// genera los enemigos del nivel
	void trataColisiones();		// mira si alguien ha chocado con alguna pared
	void aplicaLogicas();		// hace que todo el mundo corra su lógica
	void aplicaMuertes();		// maneja las muertes de los sprites

public:
	cNivel(cSistema* sis, cNaveEspacial* naveEspaciall, 
			int tilesAncho, int tilesAlto, int anchoTile, int altoTile, 
			const char* ficheroMapa, const char* ficheroTextura);
	virtual ~cNivel();
	

	int getPosicion() const;
	//void setDimensionesMapa(int tilesAncho, int tilesAlto);
	void getDimensionesMapa(int &tilesAncho, int &tilesAlto) const;
	//void setDimensionesTile(int anchoTile, int altoTile);
	void getDimensionesTile(int &anchoTile, int &altoTile) const;
	

	// consultar
	const list<cItem*> getItems() const;
	const list<cEnemigo*> getEnemigos() const;
	const list<cDisparo*> getDisparos() const;
	
	// establecer
	void pushItem(cItem* item);
	void pushEnemigo(cEnemigo* enemigo);
	void pushDisparo(cDisparo* disparo);

	// eliminar
	void delItem(cItem* item);
	void delEnemigo(cEnemigo* enemigo);
	void delDisparo(cDisparo* disparo);
	

	void logica();
	void pinta() const;

};
