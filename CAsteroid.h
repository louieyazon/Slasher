#ifndef _CAsteroid_h
#define _CAsteroid_h

#include "TVector2.h"
#include "CGameObject.h"

class CAsteroid: public GD4N::CGameObject {
protected:
	void Draw();
	void Update();
	void Respawn();
	//physics
	float vy;	float vx;	float vx_max;	float vy_max;
public:
	CAsteroid();
	~CAsteroid();

	GD4N::TVector2<int> position;
	int radius;
};

#endif