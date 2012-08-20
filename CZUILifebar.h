#ifndef _CZUILifebar_h
#define _CZUILifebar_h

#include "TVector2.h"
#include "CGameObject.h"

class CZUILifebar: public GD4N::CGameObject {
protected:
	void Draw();
	void Update();

	void equalize();

	int maxwidth;
	float lifeTargetPercent;
	
	//effects
	int flashing;
	float updateDelay;
	
public:
	CZUILifebar();
	~CZUILifebar();
	int width;
	int height;

	void setTargetPercent(float x);
	void decreaseTargetPercent(float x);

	GD4N::TVector2<int> position;
	GD4N::TVector2<int> positionContainer;
	GD4N::TVector2<int> positionPortrait;
	GD4N::TVector2<int> targetsrcposition;
	GD4N::TVector2<int> srcposition;
	GD4N::TVector2<int> srcposition_lag;
};

#endif