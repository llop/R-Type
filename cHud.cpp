#include "cHud.h"
#include "cNaveEspacial.h"
#include <gl/glut.h>
#include <vector>
using namespace std;


int vidaHud[4] = { 5, 4, 6, 7 };
int magiaHud[2][4] = {
	{ 98, 5, 6, 7 },
	{ 106, 5, 6, 7 }
};
int barHud[4] = { 16, 4, 78, 8 };
int blueHud[4] = { 23, 13, 64, 6 };
int redHud[4] = { 90, 13, 64, 6 };
int numHud[10][4] = { 
	{ 6, 20, 8, 7 },
	{ 17, 20, 2, 7 },
	{ 23, 20, 8, 7 },
	{ 36, 20, 8, 7 },
	{ 49, 20, 8, 7 },
	{ 61, 20, 8, 7 },
	{ 73, 20, 8, 7 },
	{ 85, 20, 8, 7 },
	{ 98, 20, 8, 7 },
	{ 111, 20, 8, 7 }
};


cHud::cHud(cSistema* sis, int x, int y) : cSprite(sis, x, y) {
	_sis->cargaTextura(TEX_HUD, "img\\hud.png");
}

void cHud::logica() {
	++_tiempoVida;
}

void cHud::pintaFondo() const {
	// pintar un fondo negro
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0); 
	glVertex2i(_x, 0);
	glVertex2i(_x+GAME_WIDTH, 0);
	glVertex2i(_x+GAME_WIDTH, _y);
	glVertex2i(_x, _y);
	glColor3f(1, 1, 1); 
	glEnd();
}

void cHud::pintaVidas() const {
	// pintar las vidas
	cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
	int vidasNave = nave->vidas();
	if (!vidasNave) return;

	int wTex, hTex;
	_sis->tamanoTextura(TEX_HUD, wTex, hTex);
	int wPixVida = vidaHud[2];
	int hPixVida = vidaHud[3];
	int xPixVida = _x + 30;
	int yPixVida = _y - (8 + hPixVida);
	float xTexVida = vidaHud[0] / float(wTex);
	float yTexVida = vidaHud[1] / float(hTex);
	float wTexVida = vidaHud[2] / float(wTex);
	float hTexVida = vidaHud[3] / float(hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _sis->idTextura(TEX_HUD));
	glBegin(GL_QUADS);
	for (int i = 0; i<vidasNave; ++i) {
		glTexCoord2f(xTexVida, yTexVida + hTexVida);			glVertex2i(xPixVida, yPixVida);
		glTexCoord2f(xTexVida + wTexVida, yTexVida + hTexVida);	glVertex2i(xPixVida + wPixVida, yPixVida);
		glTexCoord2f(xTexVida + wTexVida, yTexVida);			glVertex2i(xPixVida + wPixVida, yPixVida + hPixVida);
		glTexCoord2f(xTexVida, yTexVida);						glVertex2i(xPixVida, yPixVida + hPixVida);
		xPixVida += vidaHud[2] + 3;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void cHud::pintaMagias() const {
	// pintar las vidas
	cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
	int magiasNave = nave->magias();
	if (!magiasNave) return;

	int seqMagia = !nave->puedeTirarMagia() && (_tiempoVida/24)%2 ? 1 : 0;
	int wTex, hTex;
	_sis->tamanoTextura(TEX_HUD, wTex, hTex);
	int wPixMagia = magiaHud[seqMagia][2];
	int hPixMagia = magiaHud[seqMagia][3];
	int xPixMagia = _x + 30;
	int yPixMagia = _y - (20 + hPixMagia);
	float xTexMagia = magiaHud[seqMagia][0] / float(wTex);
	float yTexMagia = magiaHud[seqMagia][1] / float(hTex);
	float wTexMagia = magiaHud[seqMagia][2] / float(wTex);
	float hTexMagia = magiaHud[seqMagia][3] / float(hTex);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _sis->idTextura(TEX_HUD));
	glBegin(GL_QUADS);
	for (int i = 0; i<magiasNave; ++i) {
		glTexCoord2f(xTexMagia, yTexMagia + hTexMagia);				glVertex2i(xPixMagia, yPixMagia);
		glTexCoord2f(xTexMagia + wTexMagia, yTexMagia + hTexMagia);	glVertex2i(xPixMagia + wPixMagia, yPixMagia);
		glTexCoord2f(xTexMagia + wTexMagia, yTexMagia);				glVertex2i(xPixMagia + wPixMagia, yPixMagia + hPixMagia);
		glTexCoord2f(xTexMagia, yTexMagia);							glVertex2i(xPixMagia, yPixMagia + hPixMagia);
		xPixMagia += magiaHud[seqMagia][2] + 3;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cHud::pintaBarra() const  {
	// pintar progreso carga tiro
	int wTex, hTex;
	_sis->tamanoTextura(TEX_HUD, wTex, hTex);
	int wPixBar = barHud[2];
	int hPixBar = barHud[3];
	int xPixBar = _x + 150;
	int yPixBar = _y - (8 + hPixBar);
	float xTexBar = barHud[0]/float(wTex);
	float yTexBar = barHud[1]/float(hTex);
	float wTexBar = barHud[2]/float(wTex);
	float hTexBar = barHud[3]/float(hTex);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _sis->idTextura(TEX_HUD));
	glBegin(GL_QUADS);
	glTexCoord2f(xTexBar, yTexBar + hTexBar);			glVertex2i(xPixBar, yPixBar);
	glTexCoord2f(xTexBar + wTexBar, yTexBar + hTexBar);	glVertex2i(xPixBar + wPixBar, yPixBar);
	glTexCoord2f(xTexBar + wTexBar, yTexBar);			glVertex2i(xPixBar + wPixBar, yPixBar + hPixBar);
	glTexCoord2f(xTexBar, yTexBar);						glVertex2i(xPixBar, yPixBar + hPixBar);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// int tamano = max(0, _carga_tiro/NAVE_TIRO_DELAY);
	// tamano = min(NAVE_MAX_CARGA_TIRO, tamano/NAVE_FACTOR_CARGA_TIRO);
	// => minimo = NAVE_TIRO_DELAY, maximo = NAVE_MAX_CARGA_TIRO*NAVE_TIRO_DELAY*NAVE_FACTOR_CARGA_TIRO
	cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
	int cargaMax = NAVE_MAX_CARGA_TIRO*NAVE_TIRO_DELAY*NAVE_FACTOR_CARGA_TIRO;
	int cargaTiro = max(0, min(cargaMax, nave->cargaDisparo()-NAVE_TIRO_DELAY));
	if (cargaTiro) {
		int mediaCargaMax = cargaMax>>1;
		if (cargaTiro>mediaCargaMax) {
			// pintarle el fondo azul
			int wPixBlu = blueHud[2];
			int hPixBlu = blueHud[3];
			int xPixBlu = _x + 157;
			int yPixBlu = _y - (9 + hPixBlu);
			float xTexBlu = blueHud[0]/float(wTex);
			float yTexBlu = blueHud[1]/float(hTex);
			float wTexBlu = blueHud[2]/float(wTex);
			float hTexBlu = blueHud[3]/float(hTex);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _sis->idTextura(TEX_HUD));
			glBegin(GL_QUADS);
			glTexCoord2f(xTexBlu, yTexBlu + hTexBlu);			glVertex2i(xPixBlu, yPixBlu);
			glTexCoord2f(xTexBlu + wTexBlu, yTexBlu + hTexBlu);	glVertex2i(xPixBlu + wPixBlu, yPixBlu);
			glTexCoord2f(xTexBlu + wTexBlu, yTexBlu);			glVertex2i(xPixBlu + wPixBlu, yPixBlu + hPixBlu);
			glTexCoord2f(xTexBlu, yTexBlu);						glVertex2i(xPixBlu, yPixBlu + hPixBlu);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		int* colBar = cargaTiro<=mediaCargaMax? blueHud : redHud;
		if (cargaTiro>mediaCargaMax) cargaTiro-=mediaCargaMax;
		float progreso = cargaTiro/float(mediaCargaMax);
		int wPixCol = int(progreso*colBar[2]);
		int hPixCol = colBar[3];
		int xPixCol = _x + 157;
		int yPixCol = _y - (9 + hPixCol);
		float xTexCol = colBar[0]/float(wTex);
		float yTexCol = colBar[1]/float(hTex);
		float wTexCol = wPixCol/float(wTex);
		float hTexCol = hPixCol/float(hTex);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _sis->idTextura(TEX_HUD));
		glBegin(GL_QUADS);
		glTexCoord2f(xTexCol, yTexCol + hTexCol);			glVertex2i(xPixCol, yPixCol);
		glTexCoord2f(xTexCol + wTexCol, yTexCol + hTexCol);	glVertex2i(xPixCol + wPixCol, yPixCol);
		glTexCoord2f(xTexCol + wTexCol, yTexCol);			glVertex2i(xPixCol + wPixCol, yPixCol + hPixCol);
		glTexCoord2f(xTexCol, yTexCol);						glVertex2i(xPixCol, yPixCol + hPixCol);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

void cHud::pintaPuntos() const {
	// pintar los puntos
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _sis->idTextura(TEX_HUD));
	glBegin(GL_QUADS);

	cNaveEspacial* nave = (cNaveEspacial*)_sis->naveEspacial();
	long long puntos = nave->puntos()*FACTOR_PUNTOS;
	vector<int> nums;
	if (puntos) {
		while (puntos) {
			nums.push_back(puntos%10);
			puntos /= 10;
		}
	} else nums.push_back(0);

	int wTex, hTex;
	_sis->tamanoTextura(TEX_HUD, wTex, hTex);
	int xPixPnt = _x + 150;
	int yPixPnt = _y - (20 + numHud[0][3]);
	for (int i=nums.size()-1; i>=0; --i) {
		int seq = nums[i];
		int wPixPnt = numHud[seq][2];
		int hPixPnt = numHud[seq][3];
		float xTexPnt = numHud[seq][0]/float(wTex);
		float yTexPnt = numHud[seq][1]/float(hTex);
		float wTexPnt = numHud[seq][2]/float(wTex);
		float hTexPnt = numHud[seq][3]/float(hTex);
		glTexCoord2f(xTexPnt, yTexPnt + hTexPnt);			glVertex2i(xPixPnt, yPixPnt);
		glTexCoord2f(xTexPnt + wTexPnt, yTexPnt + hTexPnt);	glVertex2i(xPixPnt + wPixPnt, yPixPnt);
		glTexCoord2f(xTexPnt + wTexPnt, yTexPnt);			glVertex2i(xPixPnt + wPixPnt, yPixPnt + hPixPnt);
		glTexCoord2f(xTexPnt, yTexPnt);						glVertex2i(xPixPnt, yPixPnt + hPixPnt);
		xPixPnt += numHud[seq][2]+2;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void cHud::pinta() const {
	pintaFondo();
	pintaVidas();
	pintaMagias();
	pintaBarra();
	pintaPuntos();
}
