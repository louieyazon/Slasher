#ifndef _CAsteroid_h
#define _CAsteroid_h

#include "TVector2.h"
#include "CGameObject.h"
#include "CSurfaceSheet.h"
#include "CGameTimer.h"
#include "constants.h"

typedef struct oneExplode {
	GD4N::CSurfaceSheet* explodesheet;
	int frame;
	GD4N::TVector2<int> position;
	GD4N::TVector2<int> initoffset;
	GD4N::TVector2<int> v;
	bool isExploding;
	int framesAfterPrevious;
} asteroidexplosion;



class CAsteroid: public GD4N::CGameObject {
protected:
	void Draw();
	void Update();
	void Respawn();
	void Animate();
	void moveExplosion(int r);

	void bound();
	void drawExplode();
	//physics
	float vy;	float vx;	float vx_max;	float vy_max;
	int next_y;
	int arrow_y;
	bool asteroidOn;
	float asteroidspawndelay;

	//animation
	GD4N::CSurfaceSheet* explodesheet;
	GD4N::CSurfaceSheet* explodesheet2;
	GD4N::CSurfaceSheet* explodesheet3;
	
	asteroidexplosion explosion[ EXPLODE_SPRITES_PER_ASTEROID ];
	void setExplodeInitPositions();
	void resetExplodes();

	int explodeframe;
	int explodeframe2;
	int explodeframe3;
	float spriteTimeLast;
	float spriteTimeBetween;

	void initExplodes();
	void cleanupExplodeSheets();

	float* zlifetarget;
	void generateNextY();
	float sceneStartTime;
	
public:
	CAsteroid(float spawndelay);
	~CAsteroid();

	void explode();
	void takeDamage(int dmg);
	bool exploded;
	int hitpoints;

	void setLifeTarget(float* lifetarget);
	GD4N::TVector2<int> position;
	GD4N::TVector2<int> explosition;
	float radius;
};




#endif