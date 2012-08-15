#ifndef _CBGround_h
#define _CBGround_h

#include "CGameObject.h"

class CBGround: public GD4N::CGameObject {
protected:
	void Draw();
	int width;
	int height;

public:
	CBGround();
	~CBGround();
	int GetHeight();
	int	GetWidth();
};

#endif