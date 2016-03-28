#pragma once


#include "cData.h"


#define GAME_WIDTH	640
#define GAME_HEIGHT 480

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define MAP_WIDTH 40
#define MAP_HEIGHT 28

#define HUD_HEIGHT 2
#define HUD_HPIX 32


#define MENU 0
#define NIVEL 1


struct cRect {
	int x, y, w, h;
};


class cSprite;
class cSistema {
private:

	cSprite* _naveEspacial;
	cSprite* _nivel;

	cData* _data;

	int _estado;
	int _numNivel;
	
	void logicaMenu();
	void logicaNivel();
	
	void pintaMenu();
	void pintaNivel();


public:
	cSistema();
	~cSistema();

	
	cSprite* naveEspacial() const;
	void setNaveEspacial(cSprite* naveEspacial);
	void delNaveEspacial();

	cSprite* nivel() const;
	void setNivel(cSprite* nivel);
	void delNivel();

	void cargaTextura(int id, const char* ficheroTextura);
	int idTextura(int id) const;
	void tamanoTextura(int id, int &width, int &height) const;

	// esto para el game loop
	void procesaTeclas(unsigned char *keys);
	void logica();
	void pinta();

};



#define COLISION_ARRIBA 1
#define COLISION_ABAJO 2
#define COLISION_IZQ 4
#define COLISION_DER 8

#define COLISION_TILE 1
#define COLISION_PANTALLA 2


/**
 * Un sprite es un coso que se tiene que implementar
 */
class cSprite {
protected:

	int _x, _y;			// coordenadas
	cSistema* _sis;		// sistema donde vive
	bool _muerto;		// esta muerto?


	int _state;
	int _seq;
	int _delay;


public:
	cSprite(cSistema* sis);
	cSprite(cSistema* sis, int x, int y);

	virtual ~cSprite() {};		// las subclases deben implementar el destructor


	void getPosicion(int &x, int &y) const;
	void setPosicion(int x, int y);
	virtual void offset(int x, int y);


	bool muerto() const;		// dice si el sprite esta definitivamente muerto
	virtual void muerete() {};	// manda al sprite que se muera

	// que cada sprite calcula su caja como le de la gana
	virtual void caja(cRect &caja) const {};
	
	// por donde colisiona
	virtual void colision(cRect &caja, int &colMask) const {};

	// esta mierda que la implementen las subclases
	virtual void logica() {};
	virtual void pinta() const {};

};

