#ifndef _CPlatform_h
#define _CPlatform_h

#include "CGameObject.h"

class CPlatform: public GD4N::CGameObject {
protected:
	void Draw();
	int width;
	int height;

public:
	CPlatform();
	~CPlatform();
	int GetHeight();
	int	GetWidth();
};

#endif