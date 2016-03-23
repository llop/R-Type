#pragma once

#include "cTexture.h"
#include "Globals.h"


//----------------------------------------------------------
// texture array size
//----------------------------------------------------------

#define NUM_IMG	14


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

// nave y demás
#define TEX_NAVE1 7
#define TEX_NAVE2 8
#define TEX_ESCUDO 9

// items
#define TEX_ITEMS 10

// enemigos
#define TEX_ENE1 11
#define TEX_ENE2 12
#define TEX_ENE_ITEM 13



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
