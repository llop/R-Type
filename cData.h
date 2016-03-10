#pragma once

#include "cTexture.h"
#include "Globals.h"

//Image array size
#define NUM_IMG	3

//Image identifiers
#define TEX_NIVEL1 0
#define TEX_NAVE 1
#define TEX_ENE1 2


class cData {
public:
	cData(void);
	~cData(void);

	int  GetID(int img) const;
	void GetSize(int img,int *w,int *h) const;
	bool LoadImage(int img,const char *filename,int type = GL_RGBA);

private:
	cTexture texture[NUM_IMG];
};
