#ifndef _CZero_h
#define _CZero_h

#include "CGameObject.h"
#include "TVector2.h"
#include "CSurfaceSheet.h"
#include "constants.h"


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
	int curFrame;
	int drawFrame;
	bool cycleanim;
	void setAnimationState();
	void forwardFrame();
	void decideFrame();
	void nextAnimState();

	
	int animZeroState;
	int animLastZeroState;

	// nonposition object states
	void readState();
	bool falling;
	int zeroState;
	int lastZeroState;
	float jumpfuel;	float jumpmax;

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
	void attack();



public:
	CZero();
	~CZero();
	const GD4N::TVector2<int> & GetPosition() { return position; };
};

enum ZERO_ANIMSTATES {
	AS_STANDING		= 0,
	AS_STARTRUN		= 1,
	AS_RUNNING		= 2,
	AS_STOPRUN		= 3,
	AS_JUMPOFF		= 4,
	AS_RISING		= 5,
	AS_JUMPTRANS	= 6,
	AS_FALLING		= 7,
	AS_LANDING		= 8,
	AS_SLASH1		= 9,
	AS_SLASH2		= 10,
	AS_SLASH3		= 11,
	AS_KEEPSABER	= 12,
};

const int sfZeroStand[8] =		{	20,20,20,21,22,22,22,21						};
const int sfZeroRun[14] =		{	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13	};
const int sfZeroIntoRun[2]	=	{	15,14										};
const int sfZeroStopRun[2]	=	{	14,15										};

const int sfZeroSlash1[13] =	{	30,31,32,33,34,35,36,37,38,39,40,41,42		};
const int sfZeroSlash2[11] =	{	43,44,45,46,47,48,49,50,51,52,53			};
const int sfZeroSlash3[14] =	{	54,55,56,57,58,59,60,61,62,63,64,65,66,67	};
const int sfZeroKS[4] =			{	25,26,27,28									};

const int sfZeroJump[2] =		{	70,71										};
const int sfZeroRise[2] =		{	72,73										};
const int sfZeroJT[5] =			{	74,75,76,77,78							 	};
const int sfZeroFall[3] =		{	79,80,81									};
const int sfZeroLand[4] =		{	82,83,84,85									};


typedef struct animationsegment {
	int numberOfFrames;
	float delay;
	bool loops;
	int* spriteFrame;
	int nextAnimState;
} animPart;

const animPart aCycle[] = {	//  {numofFrames, delay, loops, sprite frame array, next animation state}
								{ 8,	0.33f,		true	,	(int*)&sfZeroStand		, AS_STANDING	}, //zero standing
								{ 2,	0.05f,		false	,	(int*)&sfZeroIntoRun	, AS_RUNNING	}, //zero into running
								{ 14,	0.033f,		true	,	(int*)&sfZeroRun		, AS_RUNNING	}, //zero running
								{ 2,	0.033f,		false	,	(int*)&sfZeroStopRun	, AS_STANDING	}, //zero stop running
								{ 2,	0.067f,		false	,	(int*)&sfZeroJump		, AS_RISING		}, //zero jumpoff
								{ 2,	0.033f,		true	,	(int*)&sfZeroRise		, AS_RISING		}, //zero rising
								{ 5,	0.033f,		false	,	(int*)&sfZeroJT			, AS_FALLING	}, //zero jump transition
								{ 3,	0.033f,		true	,	(int*)&sfZeroFall		, AS_FALLING	}, //zero falling
								{ 4,	0.067f,		false	,	(int*)&sfZeroLand		, AS_STANDING	}, //zero landing
								{ 8,	0.033f,		false	,	(int*)&sfZeroSlash1		, AS_KEEPSABER	}, //zero slash1
								{ 8,	0.033f,		false	,	(int*)&sfZeroSlash2		, AS_KEEPSABER	}, //zero slash2
								{ 8,	0.033f,		false	,	(int*)&sfZeroSlash3		, AS_KEEPSABER	}, //zero slash3
								{ 8,	0.033f,		false	,	(int*)&sfZeroKS			, AS_STANDING	} //zero keepsaber
};



#endif