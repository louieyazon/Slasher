#include "CZUILifebar.h"
#include "STimeManager.h"
#include "SVideoManager.h"
#include "constants.h"

CZUILifebar::CZUILifebar() : CGameObject() {
	position.x				= LIFEBAR_X;
	position.y				= LIFEBAR_Y;
	logo					= true;

	srcposition.x			= 0;
	srcposition.y			= 0;			//max is LIFEBAR_MAXSPRITEY

	srcposition_lag.x		= 0;
	srcposition_lag.y		= 0;

	targetsrcposition.x		= 0;
	targetsrcposition.y		= 0;

	logoposition.x			= LOGOPOS_X;
	logoposition.y			= LOGOPOS_Y;

	positionContainer.x		= position.x - 5;
	positionContainer.y		= position.y - 4;

	positionPortrait.x		= position.x - 100;
	positionPortrait.y		= position.y - 55;

	maxwidth				= LIFEBAR_MAXWIDTH;
	flashing				= 0;

	lifeTargetPercent		= 100.0;

	
	updateDelay				= LIFEBAR_UPDATERATE;

	height					= LIFEBAR_SPRITEHEIGHT;
	width					= LIFEBAR_MAXWIDTH;


	UINumbersTexture = new GD4N::CSurfaceSheet(SURFID_UILARGENUMBERS);
	UINumbersTexture->SetSpriteDimensions(1,10);
}

CZUILifebar::~CZUILifebar() {
	delete UINumbersTexture;
}

//sVideo->Draw(SURFID_LIFEBAR, position); base statement
void CZUILifebar::Draw() {
	sVideo->Draw(SURFID_LIFECONTAINER, positionContainer);									// LIFEBAR HOLDER
	sVideo->Draw(SURFID_LIFEBARDIFF, position, srcposition_lag, width, height);				// RED LAGGING LIFEBAR
	sVideo->Draw(SURFID_LIFEBAR, position, srcposition, width, height);						// ACTUAL LIFEBAR
	
	if(flashing == 5) flashing = 0;
	if(flashing) {
		if (flashing % 4 < 2) {
			sVideo->Draw(SURFID_LIFEBARWHITE, position, srcposition_lag, width, height);		//LIFEBAR EFFECTS
			flashing++;
		} else if (flashing % 4 > 1) {
			GD4N::TVector2<int> flashsrcpstn;
			flashsrcpstn.x = srcposition_lag.x;
			flashsrcpstn.y = srcposition_lag.y + 500;
			sVideo->Draw(SURFID_LIFEBARWHITE, position, flashsrcpstn, width, height);		//LIFEBAR EFFECTS
			flashing++;
		}
	}

	sVideo->Draw(SURFID_ZEROPORTRAIT, positionPortrait);									// ZERO PORTRAIT
	showPointsNumber(320, 10, 8, *UIpointsource);

	if(!gameOn) {
		if(logo) sVideo->Draw(SURFID_SLASHERLOGO, logoposition);

		if(sTime->GetTime() > LOGO_DURATION)	 { logo = false; }
		if(sTime->GetTime() > LOGO_DURATION + 1) { gameOn = true; }
	}
	
}

void CZUILifebar::Update() {
	/*if(	(rand() % 200)	==	1) {		//take random damage at random intervals for testing
		int dmg = (rand() % 20) + 10;
		decreaseTargetPercent(dmg);	
	}*/	

	setTargetPercent(*UIlifesource);
	equalize();
	targetsrcposition.y = 0 + (int)((100 - lifeTargetPercent) / 100 * LIFEBAR_MAXSPRITEY);
}

void CZUILifebar::equalize() {
	int lagdiff = targetsrcposition.y - srcposition_lag.y;
	int maindiff = targetsrcposition.y - srcposition.y;

	if(lagdiff > 0) {
		srcposition_lag.y++;	//TODO change this to be time dependent
		if(lagdiff > 20) srcposition_lag.y += lagdiff / 30;
	}
	if(lagdiff < 0) srcposition_lag.y += lagdiff / 4;

	if(maindiff > 0) srcposition.y += maindiff / 4;	//TODO change this to be time dependent
	if(maindiff < 0) 
	{	
		srcposition.y--;
		if(maindiff < -10) srcposition.y += maindiff / 20;
	}


}


void CZUILifebar::setLifeSource(float* lifesource){
	UIlifesource = lifesource;
}

void CZUILifebar::setPointsSource(int* pointsource){
	UIpointsource = pointsource;
}

void CZUILifebar::setTargetPercent(float x) {
	lifeTargetPercent = x;
	if (lifeTargetPercent <= 0) lifeTargetPercent = 0;
}

void CZUILifebar::decreaseTargetPercent(float d) {
	setTargetPercent(lifeTargetPercent - d);
	flashing = 1;
}

void CZUILifebar::showPointsNumber(const int x, const int y, const int digits, const int outpoints) {
	GD4N::TVector2<int> digitpos;
	digitpos.x = x;
	digitpos.y = y;
	int digitVal = 1;
	int powdigit = 1;

	for(int dig = 0; dig < digits; dig++) {
		digitVal = (int)(outpoints / powdigit) % 10;
		digitpos.x = x - (dig * 21);
		UINumbersTexture->SetCurrentFrame(digitVal);
		sVideo->Draw(UINumbersTexture, digitpos);
		powdigit *= 10;
	}

}