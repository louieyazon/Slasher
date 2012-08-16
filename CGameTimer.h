#ifndef _CGameTimer_h
#define _CGameTimer_h

#include "CGameObject.h"

class CGameTimer: public GD4N::CGameObject {
protected:
	void Update();
public:
	CGameTimer();
	~CGameTimer();
};

#endif