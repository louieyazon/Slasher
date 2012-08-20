#ifndef _CZUILifebar_h
#define _CZUILifebar_h

#include "TVector2.h"
#include "CGameObject.h"
#include "CSurfaceSheet.h"

class CZUILifebar: public GD4N::CGameObject {
protected:
	void Draw();
	void Update();

	void equalize();

	int maxwidth;
	int points;
	float lifeTargetPercent;
	
	//effects
	int flashing;
	float updateDelay;
	float* UIlifesource;
	int* UIpointsource;

	//score
	GD4N::CSurfaceSheet *UINumbersTexture;

	//points
	void showPointsNumber(const int x, const int y, const int digits, const int outpoints);
	
public:
	CZUILifebar();
	~CZUILifebar();
	int width;
	int height;

	void setTargetPercent(float x);
	void decreaseTargetPercent(float x);
	void setLifeSource(float* lifesource);
	void setPointsSource(int* pointsource);

	GD4N::TVector2<int> position;
	GD4N::TVector2<int> positionContainer;
	GD4N::TVector2<int> positionPortrait;
	GD4N::TVector2<int> targetsrcposition;
	GD4N::TVector2<int> srcposition;
	GD4N::TVector2<int> srcposition_lag;
};

#endif