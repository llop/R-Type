#include "cData.h"

cData::cData(void) {}
cData::~cData(void){}

int cData::GetID(int img) const {
	return texture[img].GetID();
}

void cData::GetSize(int img, int *w, int *h) const {
	texture[img].GetSize(w,h);
}

bool cData::LoadImage(int img, const char *filename, int type) {
	int res;

	res = texture[img].Load(filename,type);
	if(!res) return false;

	return true;
}