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
	void Move();
	//bool IsCollidingWith(GD4N::CGameObject* other);
    //void CollidesWith(GD4N::CGameObject* other);

	float spriteTimeLast;
	float spriteTimeBetween;
	float logicTimeLast;
	float logicTimeBetween;

	int curFrame;
	int drawFrame;
	bool frameForward;

	// animation methods
	void newAnimateState();


	int zeroState;
	int lastZeroState;

	int animZeroState;
	int animLastZeroState;


	// physics
	float fYSpeed;
	float fXSpeed;
	bool falling;
	int jumpfuel;
	int jumpmax;

	GD4N::TVector2<int> position;

	GD4N::CSurfaceSheet *zeroTexture;

public:
	CZero();
	~CZero();
	const GD4N::TVector2<int> & GetPosition() { return position; };
};

#endif