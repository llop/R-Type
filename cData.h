#pragma once

#include "cTexture.h"
#include "Globals.h"


//----------------------------------------------------------
// texture array size
//----------------------------------------------------------

#define NUM_IMG	27


//----------------------------------------------------------
// texture identifiers
//----------------------------------------------------------

// niveles
#define TEX_NIVEL1 0
#define TEX_NIVEL2 1
#define TEX_NIVEL3 2
#define TEX_NIVEL4 3
#define TEX_FONDO1 4
#define TEX_FONDO2 5
#define TEX_FONDO3 6
#define TEX_FONDO4 7
#define TEX_HUD 8
#define TEX_MENU 9

// nave y demás
#define TEX_NAVE1 10
#define TEX_NAVE2 11
#define TEX_ESCUDO 12

// items
#define TEX_ITEMS 13

// explosiones
#define TEX_EXPLO1 14
#define TEX_EXPLO2 15

// enemigos
#define TEX_ENE1 16
#define TEX_ENE2 17
#define TEX_ENE3 18
#define TEX_ENE_ITEM 19
#define TEX_JEFE1 20
#define TEX_JEFE2 21
#define TEX_JEFE3 22
#define TEX_MINIJEFE3 23
#define TEX_ENE4 24
#define TEX_ENE5 25
#define TEX_JEFE4 26



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
