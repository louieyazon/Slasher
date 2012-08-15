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
	void Move();
	//bool IsCollidingWith(GD4N::CGameObject* other);
    //void CollidesWith(GD4N::CGameObject* other);

	float spriteTimeLast;
	float spriteTimeBetween;
	float logicTimeLast;
	float logicTimeBetween;

	int curFrame;
	bool frameForward;

	int zeroState;

	float fYSpeed;
	float fXSpeed;

	GD4N::TVector2<int> position;

	GD4N::CSurfaceSheet *zeroTexture;

public:
	CZero();
	~CZero();
	const GD4N::TVector2<int> & GetPosition() { return position; };
};

#endif