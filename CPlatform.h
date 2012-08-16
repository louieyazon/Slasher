#ifndef _CPlatform_h
#define _CPlatform_h

#include "TVector2.h"
#include "CGameObject.h"

class CPlatform: public GD4N::CGameObject {
protected:
	void Draw();
	int width;
	int height;
	
public:
	CPlatform(int x, int y);
	~CPlatform();
	int GetHeight();
	int	GetWidth();
	GD4N::TVector2<int> position;
};

#endif