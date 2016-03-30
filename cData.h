#pragma once

#include "cTexture.h"
#include "Globals.h"


//----------------------------------------------------------
// texture array size
//----------------------------------------------------------

#define NUM_IMG 21


//----------------------------------------------------------
// texture identifiers
//----------------------------------------------------------

// niveles
#define TEX_NIVEL1 0
#define TEX_NIVEL2 1
#define TEX_NIVEL3 2
#define TEX_FONDO1 3
#define TEX_FONDO2 4
#define TEX_FONDO3 5
#define TEX_HUD 6
#define TEX_MENU 7

// nave y demás
#define TEX_NAVE1 8
#define TEX_NAVE2 9
#define TEX_ESCUDO 10

// items
#define TEX_ITEMS 11

// explosiones
#define TEX_EXPLO1 12
#define TEX_EXPLO2 13

// enemigos
#define TEX_ENE1 14
#define TEX_ENE2 15
#define TEX_ENE3 16
#define TEX_ENE_ITEM 17
#define TEX_JEFE1 18
#define TEX_JEFE2 19
#define TEX_JEFE3 20



class cData {
public:
	cData(void);
	~cData(void);

	int  getID(int img) const;
	void getSize(int img, int* w, int* h) const;
	bool loadImage(int img,const char* filename, int type = GL_RGBA);

private:
	cTexture texture[NUM_IMG];
};
