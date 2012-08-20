#ifndef _CAsteroid_h
#define _CAsteroid_h

#include "TVector2.h"
#include "CGameObject.h"
#include "CSurfaceSheet.h"

class CAsteroid: public GD4N::CGameObject {
protected:
	void Draw();
	void Update();
	void Respawn();
	void Animate();

	void bound();
	void drawExplode();
	//physics
	float vy;	float vx;	float vx_max;	float vy_max;
	
	//animation
	GD4N::CSurfaceSheet *explodesheet;
	GD4N::CSurfaceSheet *explodesheet2;
	GD4N::CSurfaceSheet *explodesheet3;
	int explodeframe;
	int explodeframe2;
	int explodeframe3;
	float spriteTimeLast;
	float spriteTimeBetween;

	float* zlifetarget;
	
public:
	CAsteroid();
	~CAsteroid();

	void explode();
	bool exploded;

	void setLifeTarget(float* lifetarget);
	GD4N::TVector2<int> position;
	GD4N::TVector2<int> explosition;
	float radius;
};

#endif