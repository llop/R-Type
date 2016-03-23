
#include "cTexture.h"
#include "corona.h"
#include "Globals.h"

cTexture::cTexture(void) {
	loaded = false;
}

cTexture::~cTexture(void) {
}

bool cTexture::load(const char *filename,int type,int wraps,int wrapt,int magf,int minf,bool mipmap) {
	corona::Image* img;
	int components;

	img = corona::OpenImage(filename);
	if(type==GL_RGB) {
		//img = corona::OpenImage(filename,corona::PF_R8G8B8);
		components = 3;
	} else if(type==GL_RGBA) {
		//img = corona::OpenImage(filename,corona::PF_R8G8B8A8);
		components = 4;
	} else return loaded = false;

	if (img == NULL) return loaded = false;

	width  = img->getWidth();
	height = img->getHeight();

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D,id);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wraps);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapt);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magf);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minf);

	if(!mipmap) {
		glTexImage2D(GL_TEXTURE_2D,0,components,width,height,0,type,
					 GL_UNSIGNED_BYTE,img->getPixels());
	} else {
		gluBuild2DMipmaps(GL_TEXTURE_2D,components,width,height,type,
						  GL_UNSIGNED_BYTE,img->getPixels());
	}

	return loaded = true;
}
int cTexture::getID() const {
	return id;
}
void cTexture::getSize(int* w,int* h) const {
	*w = width;
	*h = height;
}

bool cTexture::isLoaded() const {
	return loaded;
}