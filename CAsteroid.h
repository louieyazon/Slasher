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

typedef struct oneBit {
	GD4N::TVector2<int> position;
	GD4N::TVector2<int> initoffset;
	GD4N::TVector2<int> v;
} asteroidbit;



class CAsteroid: public GD4N::CGameObject {
protected:
	void Draw();
	void Update();
	void Respawn();
	void Animate();
	

	void bound();
	void drawExplode();
	void drawBits();
	//physics
	float vy;	float vx;	float vx_max;	float vy_max;
	float ay;
	int next_y;
	int arrow_y;
	bool asteroidOn;
	float asteroidspawndelay;

	//animation
	asteroidexplosion explosion[ EXPLODE_SPRITES_PER_ASTEROID ];
	asteroidbit flyingbits[ 4 ];
	GD4N::CSurfaceSheet* asteroidbitssheet;
	void moveExplosion(int r);
	void moveBits(int r);

	float spriteTimeLast;
	float spriteTimeBetween;

	//respawn
	void setExplodeInitPositions();
	void resetAsteroidBits();
	void resetExplodes();
	
	//inits
	void initExplodes();
	void initAsteroidBits();
	void randAsteroidBitspeed();
	void inheritAsteroidBitSpeed(float ivx, float ivy);
	

	void cleanupSheets();

	float* zlifetarget;
	void generateNextY();
	float sceneStartTime;
	
public:
	CAsteroid(float spawndelay);
	~CAsteroid();

	void explode();
	bool takeDamage(int dmg, float vx, float vy);
	bool exploded;
	int hitpoints;

	void setLifeTarget(float* lifetarget);
	GD4N::TVector2<int> position;
	GD4N::TVector2<int> explosition;
	float radius;
};




#endif