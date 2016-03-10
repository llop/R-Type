#pragma once

#pragma comment(lib,"corona")

#include "Globals.h"

class cTexture
{
public:
	cTexture(void);
	~cTexture(void);

	bool Load(const char *filename,int type = GL_RGBA,int wraps = GL_REPEAT,int wrapt = GL_REPEAT,
			  int magf = GL_NEAREST,int minf = GL_NEAREST,bool mipmap = false);
	int  GetID() const;
	void GetSize(int *w,int *h) const;

private:
	GLuint id;
	int width;
	int height;
};
