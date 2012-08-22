#ifndef _CZUILifebar_h
#define _CZUILifebar_h

#include "TVector2.h"
#include "CGameObject.h"
#include "CSurfaceSheet.h"

class CZUILifebar: public GD4N::CGameObject {
protected:
	
	void Update();
	void equalize();

	void Draw();
	void drawUI();
	void drawLogo();
	void drawPoints();
	void drawHealth();
	void drawComboBar();


	int maxwidth;
	int points;
	bool logo;

	float comboTargetPercent;
	float lifeTargetPercent;
	int comboMultiplier;
	

	//effects
	int flashing;
	float updateDelay;

	//data sources
	float* UIlifesource;
	int* UIpointsource;
	float* UIcombosource;

	//score
	GD4N::CSurfaceSheet *UINumbersTexture;
	GD4N::CSurfaceSheet *UIHighscoreNumbersTexture;
	GD4N::CSurfaceSheet *UIComboNumbersTexture;


	//points
	void showPointsNumber();
	void showHighScoreNumber();

	//number drawing
	void drawNumber(const int numberToPrint, const int x, const int y, int charwidth, int digitsToShow, bool addSpacing, bool useKerning, GD4N::CSurfaceSheet* numberSprite);
	int numberOfDigits(int number);
	
public:
	CZUILifebar();
	~CZUILifebar();
	int width;
	int height;

	void setTargetPercent(float x);
	void decreaseTargetPercent(float x);
	void setLifeSource(float* lifesource);
	void setPointsSource(int* pointsource);

	//general UI position
	GD4N::TVector2<int> logoposition;
	GD4N::TVector2<int> positionContainer;
	GD4N::TVector2<int> positionPortrait;

	//health position
	GD4N::TVector2<int> position;
	GD4N::TVector2<int> targetsrcposition;
	GD4N::TVector2<int> srcposition;
	GD4N::TVector2<int> srcposition_lag;
	
	//combo bar position
	GD4N::TVector2<int> comboposition;
	GD4N::TVector2<int> combosrcposition;
	GD4N::TVector2<int> combotargetsrcposition;

	//highscore position
	GD4N::TVector2<int> posHighscorelabel;
	GD4N::TVector2<int> posHighscoreposition;
	


};

#endif