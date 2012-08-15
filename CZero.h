#ifndef _CZero_h
#define _CZero_h

#include "CGameObject.h"
#include "TVector2.h"
#include "CSurfaceSheet.h"
#include "constants.h"


const float cycleFPS[10]	=	{	0.3f,		//zero standing
									0.05f,
									0.05f,
									0.1f, 
									0.1f,
									0.1f,
									0.1f,
									0.1f,
									0.1f,
									0.1f
								};

const int cycleFN[10]		=	{
									8,		//stand								
									14,		//run
									13,		//slash
									11,		//slash2
									14,		//slash3
									4,		//keepsaber
								};

const int sfZeroStand[8] =		{	20,20,20,21,22,22,22,21						};
const int sfZeroRun[14] =		{	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13	};
const int sfZeroSlash1[13] =	{	30,31,32,33,34,35,36,37,38,39,40,41,42		};
const int sfZeroSlash2[11] =	{	43,44,45,46,47,48,49,50,51,52,53			};
const int sfZeroSlash3[14] =	{	54,55,56,57,58,59,60,61,62,63,64,65,66,67	};
const int sfZeroKS[4] =			{	25,26,27,28									};




class CZero: public GD4N::CGameObject {
protected:
	void Update();
	void Draw();
	void Animate();
	void ReactToInput();
	void Physics();
	//bool IsCollidingWith(GD4N::CGameObject* other);
    //void CollidesWith(GD4N::CGameObject* other);
	GD4N::TVector2<int> position;
	GD4N::CSurfaceSheet *zeroTexture;
	
	//float logicTimeLast;
	//float logicTimeBetween;


	// animation
	float spriteTimeLast;
	float spriteTimeBetween;
	void forwardFrame();
	void decideFrame();
	int curFrame;
	int drawFrame;
	bool frameForward;
	void readState();

	
	bool falling;
	int zeroState;
	int lastZeroState;
	float jumpfuel;	float jumpmax;

	int animZeroState;
	int animLastZeroState;
	
	// timing
	void timer();			// this function tracks time between updates
	float previousTime;		// time of last update
	float dt;				// difference in time between updates

	// physics
	float vy;	float vx;	float vx_max;	float vy_max;
	float ay;	float ax;
	
	
	void move();
	void bound();
	void friction();
	void gravity();
	void grind(float* v);

	// moves
	void accelerate_up();
	void accelerate_down(); 
	void accelerate_left(); 
	void accelerate_right();
	void jump();
	void breakjump();

public:
	CZero();
	~CZero();
	const GD4N::TVector2<int> & GetPosition() { return position; };
};

#endif