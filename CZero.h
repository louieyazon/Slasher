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
	bool IsCollidingWith(GD4N::CGameObject* other);
    void CollidesWith(GD4N::CGameObject* other);
	GD4N::TVector2<int> position;
	GD4N::CSurfaceSheet *zeroTexture;
	GD4N::CSurfaceSheet *zeroTextureL;

	void DrawDebug();
	void debugNumber(const int x, const int y, const int digits, const int* number);
	void drawSlashAfter(const int x, const int y);
	void debugNumber(const int x, const int y, const int digits, const float* number);
	GD4N::CSurfaceSheet *UINumbersTexture;

	
	//float logicTimeLast;
	//float logicTimeBetween;


	// animation
	float spriteTimeLast;
	float spriteTimeBetween;
	int curFrame;
	int drawFrame;
	void setAnimationState();
	void forwardFrame();
	void decideFrame();
	void nextAnimState();

	
	int animZeroState;
	int animLastZeroState;

	// nonposition object states
	void readState();
	
	bool falling;					// falling tracks when to apply gravity (when you're jumping or not on the ground)
	bool dashing;
	bool facingRight;

	int zeroState;
	int lastZeroState;				// probably won't need this anymore
	float jumpfuel;	float jumpmax;	// jump limiter, also allows jumping of different heights
	float dashfuel; float dashmax;	// dash limiter, also allows dashing of different distances

	// physics
	float vy;	float vx;	float vx_max;	float vy_max;
	float ay;	float ax;
	void move();
	void bound();
	void friction();
	void gravity();
	void grind(float* v);
	void boundme (int* val, int min, int max);
	

	// moves
	void accelerate_up();
	void accelerate_down(); 
	void accelerate_left(); 
	void accelerate_right();
	void dashccelerate();
	void undash();

	void dash();
	void jump();
	void breakjump();
	void attack();

	//control logic
	void disableJump();
	void reenableJump();
	void land();

	void disableDash();
	void reenableDash();



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
	AS_AIRSLASH		= 13,
	AS_INTODASH		= 14,
	AS_DASHING		= 15,
	AS_BREAKING		= 16
};

const int sfZeroStand[]		 =	{	20,20,20,21,22,22,22,21						};
const int sfZeroRun[]	=		{	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13	};
const int sfZeroIntoRun[] =		{	15,14										};
const int sfZeroStopRun[] =		{	14,15										};

const int sfZeroSlash1[] =		{	30,31,32,33,34,35,36,37,38,39,40,41,42		};
const int sfZeroSlash2[] =		{	43,44,45,46,47,48,49,50,51,52,53			};
const int sfZeroSlash3[] =		{	54,55,56,57,58,59,60,61,62,63,64,65,66,67	};
const int sfZeroKS[] =			{	25,26,27,28									};

const int sfZeroJump[] =		{	70,71										};
const int sfZeroRise[] =		{	72,73										};
const int sfZeroJT[] =			{	74,75,76,77,78							 	};
const int sfZeroFall[] =		{	79,80,81									};
const int sfZeroLand[] =		{	82,83,84,85									};
const int sfZeroJSlash[] =		{	86,87,88,89,90,91,92,93,94,81				};


const int sfZeroIntoDash[] =	{	100, 101, 102, 103							};
const int sfZeroDashing[] =		{	104, 105, 106								};
const int sfZeroBreak[]	=		{	107, 108, 109, 110, 110, 110				};

const int sfZeroDSlash[] =		{	111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121};



typedef struct animationsegment {
	int numberOfFrames;
	float delay;
	bool loops;
	int* spriteFrame;
	int nextAnimState;
} animPart;

const animPart aCycle[] = {	
	//  { numberofFrames					,	delay,		loops,		sprite frame array		, next animation state}
		{ sizeof(sfZeroStand)/sizeof(int)	,	0.33f,		true	,	(int*)&sfZeroStand		, AS_STANDING	}, //zero standing
		{ sizeof(sfZeroIntoRun)/sizeof(int)	,	0.033f,		false	,	(int*)&sfZeroIntoRun	, AS_RUNNING	}, //zero into running
		{ sizeof(sfZeroRun)/sizeof(int)		,	0.033f,		true	,	(int*)&sfZeroRun		, AS_RUNNING	}, //zero running
		{ sizeof(sfZeroStopRun)/sizeof(int)	,	0.033f,		false	,	(int*)&sfZeroStopRun	, AS_STANDING	}, //zero stop running
		{ sizeof(sfZeroJump)/sizeof(int)	,	0.067f,		false	,	(int*)&sfZeroJump		, AS_RISING		}, //zero jumpoff
		{ sizeof(sfZeroRise)/sizeof(int)	,	0.033f,		true	,	(int*)&sfZeroRise		, AS_RISING		}, //zero rising
		{ sizeof(sfZeroJT)/sizeof(int)		,	0.040f,		false	,	(int*)&sfZeroJT			, AS_FALLING	}, //zero jump transition
		{ sizeof(sfZeroFall)/sizeof(int)	,	0.033f,		true	,	(int*)&sfZeroFall		, AS_FALLING	}, //zero falling
		{ sizeof(sfZeroLand)/sizeof(int)	,	0.067f,		false	,	(int*)&sfZeroLand		, AS_STANDING	}, //zero landing
		{ sizeof(sfZeroSlash1)/sizeof(int)	,	0.02f,		false	,	(int*)&sfZeroSlash1		, AS_KEEPSABER	}, //zero slash1
		{ sizeof(sfZeroSlash2)/sizeof(int)	,	0.02f,		false	,	(int*)&sfZeroSlash2		, AS_KEEPSABER	}, //zero slash2
		{ sizeof(sfZeroSlash3)/sizeof(int)	,	0.02f,		false	,	(int*)&sfZeroSlash3		, AS_KEEPSABER	}, //zero slash3
		{ sizeof(sfZeroKS)/sizeof(int)		,	0.033f,		false	,	(int*)&sfZeroKS			, AS_STANDING	}, //zero keepsaber
		{ sizeof(sfZeroJSlash)/sizeof(int)	,	0.02f,		false	,	(int*)&sfZeroJSlash		, AS_FALLING	}, //zero airslash
		{ sizeof(sfZeroIntoDash)/sizeof(int),	0.033f,		false	,	(int*)&sfZeroIntoDash	, AS_DASHING	}, //zero into dash
		{ sizeof(sfZeroDashing)/sizeof(int)	,	0.033f,		false	,	(int*)&sfZeroDashing	, AS_DASHING	}, //zero dashing
		{ sizeof(sfZeroBreak)/sizeof(int)	,	0.040f,		false	,	(int*)&sfZeroBreak		, AS_STANDING	}, //zero breaking
		{ sizeof(sfZeroDSlash)/sizeof(int)	,	0.033f,		false	,	(int*)&sfZeroDSlash		, AS_STANDING	} //zero dashing slash
};

#endif