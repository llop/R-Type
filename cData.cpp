#include "cData.h"

cData::cData(void) {}
cData::~cData(void){}

int cData::getID(int img) const {
	return texture[img].getID();
}

void cData::getSize(int img, int* w, int* h) const {
	texture[img].getSize(w, h);
}

bool cData::loadImage(int img, const char* filename, int type) {
	if (!texture[img].isLoaded()) texture[img].load(filename,type);
	return texture[img].isLoaded();
}