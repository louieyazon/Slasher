#include "CZUILifebar.h"
#include "STimeManager.h"
#include "SVideoManager.h"
#include "constants.h"

CZUILifebar::CZUILifebar() : CGameObject() {
	position.x				= LIFEBAR_X;	// all other positions depend on this
	position.y				= LIFEBAR_Y;
	logo					= true;

	logoposition.x			= LOGOPOS_X;
	logoposition.y			= LOGOPOS_Y;
	UIpoints_lag			= 0;

	srcposition.x			= 0;			// length of the health bar
	srcposition.y			= 0;			//max is LIFEBAR_MAXSPRITEY
	srcposition_lag.x		= 0;			// length of the lagging health bar
	srcposition_lag.y		= 0;
	targetsrcposition.x		= 0;			//target length of the bar
	targetsrcposition.y		= 0;
	
	//General UI Positions
	positionContainer.x		= position.x - 5;
	positionContainer.y		= position.y - 4;
	positionPortrait.x		= position.x - 100;
	positionPortrait.y		= position.y - 55;

	//Combo Bar Positions
	comboposition.x				= position.x + COMBOBAR_OFFSETX;
	comboposition.y				= position.y + COMBOBAR_OFFSETY;

	combosrcposition.x			= 0;
	combosrcposition.y			= COMBOBAR_MAXSPRITEY;
	combotargetsrcposition.x	= 0;
	combotargetsrcposition.y	= COMBOBAR_MAXSPRITEY;

	posHighscorelabel.x			= position.x + HIGHSCORE_LABEL_OFFSETX;
	posHighscorelabel.y			= position.y + HIGHSCORE_LABEL_OFFSETY;

	maxwidth				= LIFEBAR_MAXWIDTH;
	flashing				= 0;

	lifeTargetPercent		= 100.0;

	
	updateDelay				= LIFEBAR_UPDATERATE;

	height					= LIFEBAR_SPRITEHEIGHT;
	width					= LIFEBAR_MAXWIDTH;


	UINumbersTexture = new GD4N::CSurfaceSheet(SURFID_UILARGENUMBERS);
	UINumbersTexture->SetSpriteDimensions(1,10);

	UIHighscoreNumbersTexture = new GD4N::CSurfaceSheet(SURFID_UIHIGHSCORENUMBERS);
	UIHighscoreNumbersTexture->SetSpriteDimensions(1,10);
}
CZUILifebar::~CZUILifebar() {
	delete UINumbersTexture;
}

//sVideo->Draw(SURFID_LIFEBAR, position); base statement
void CZUILifebar::Draw() {
	drawUI();
	drawPoints();
	if(!gameOn) drawLogo();
}
void CZUILifebar::drawUI(){
	drawComboBar();
	
	sVideo->Draw(SURFID_LIFECONTAINER, positionContainer);		// LIFEBAR HOLDER
	sVideo->Draw(SURFID_HIGHSCORELABEL, posHighscorelabel);		// HIGHSCORE LABEL
	drawHealth();
	sVideo->Draw(SURFID_ZEROPORTRAIT, positionPortrait);		// ZERO PORTRAIT
}
void CZUILifebar::drawPoints(){
	showPointsNumber();
	showHighScoreNumber();
	showMultiplierNumber();
}
void CZUILifebar::drawHealth(){
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
}
void CZUILifebar::drawComboBar(){
	sVideo->Draw(SURFID_COMBOBAR, comboposition, combosrcposition, COMBOBAR_MAXWIDTH, COMBOBAR_SPRITEHEIGHT);						// COMBO BUILDUP BAR
}
void CZUILifebar::drawLogo() {
	if(logo) sVideo->Draw(SURFID_SLASHERLOGO, logoposition);

	if(sTime->GetTime() > LOGO_DURATION)	 { logo = false; }
	if(sTime->GetTime() > LOGO_DURATION + 1) { gameOn = true; }
}
void CZUILifebar::showPointsNumber() {
	drawNumber(UIpoints_lag, 340, 10, 21, 8, false, false, UINumbersTexture);
}
void CZUILifebar::showHighScoreNumber() {
	drawNumber(*UIhighscoresource, posHighscorelabel.x + 45, posHighscorelabel.y + 9, 8, NULL, true, true, UIHighscoreNumbersTexture);
}
void CZUILifebar::showMultiplierNumber() {
	drawNumber(*UImultipliersource, 415, 68, 8, NULL, false, true, UIHighscoreNumbersTexture);
}

void CZUILifebar::Update() {
	/*if(	(rand() % 200)	==	1) {		//take random damage at random intervals for testing
		comboTargetPercent += (rand() % 10) + 5;
		if (comboTargetPercent >= 100) comboTargetPercent = 0;
	}*/	

	setTargetPercents();
	equalize();
	targetsrcposition.y = 0 + (int)((100 - lifeTargetPercent) / 100 * LIFEBAR_MAXSPRITEY);
	combotargetsrcposition.y = 0 + (int)((100 - comboTargetPercent) / 100 * COMBOBAR_MAXSPRITEY);
}
void CZUILifebar::equalize() {
	int lagdiff = targetsrcposition.y - srcposition_lag.y;
	int maindiff = targetsrcposition.y - srcposition.y;

	

	if(lagdiff > 0) {
		srcposition_lag.y++;						//TODO change this to be time dependent
		if(lagdiff > 20) srcposition_lag.y += lagdiff / 30;
	}
	if(lagdiff < 0) srcposition_lag.y += lagdiff / 4;

	if(maindiff > 0) srcposition.y += maindiff / 4;	//TODO change this to be time dependent
	if(maindiff < 0) 
	{	
		srcposition.y--;
		if(maindiff < -10) srcposition.y += maindiff / 20;
	}

	int combodiff = combotargetsrcposition.y - combosrcposition.y;
	if(combodiff > 0) combosrcposition.y += combodiff / 4;	//TODO change this to be time dependent
	if(combodiff < 0) 
	{	
		combosrcposition.y--;
		if(combodiff < -10) combosrcposition.y += combodiff / 20;
	}

	int pointsdiff = *UIpointsource - UIpoints_lag;
	if(pointsdiff != 0) UIpoints_lag += (pointsdiff + 0.5)/10.0;



}

void CZUILifebar::setLifeSource(float* lifesource){
	UIlifesource = lifesource;
}
void CZUILifebar::setPointsSource(int* pointsource){
	UIpointsource = pointsource;
}
void CZUILifebar::setComboSource(float* combosource){
	UIcombosource = combosource;
}
void CZUILifebar::setHighScoreSource(int* highscoresource){
	UIhighscoresource = highscoresource;
}

void CZUILifebar::setMultiplierSource(int* multipliersource){
	UImultipliersource = multipliersource;
}
void CZUILifebar::setComboThresholdSource(float* threshold){
	UIcombothresholdsource = threshold;
}

void CZUILifebar::setTargetPercents() {
	lifeTargetPercent = *UIlifesource;
	comboTargetPercent = (*UIcombosource / *UIcombothresholdsource) * 100.0;

	if (lifeTargetPercent <= 0)		lifeTargetPercent = 0;
	if (comboTargetPercent >= 100)	comboTargetPercent = 100;
}


void CZUILifebar::drawNumber(const int numberToPrint, const int x, const int y, int charwidth, int digitsToShow, bool addSpacing, bool useKerning, GD4N::CSurfaceSheet* numberSprite) {
	GD4N::TVector2<int> digitpos;
	digitpos.x = x;
	digitpos.y = y;

	int digitVal = 0;
	int powdigit = 1;
	int digitValRight = 0;
	int kern = (charwidth + 0.5)/8.0;
	int spacer = (addSpacing)? kern*3 : 0;
	
	int digits = numberOfDigits(numberToPrint);
	if (digitsToShow > digits) digits = digitsToShow;		//passed parameter gets priority if actual number of digits is smaller than it
	

	for(int dig = 0; dig < digits; dig++) {
		digitValRight = digitVal;
		digitVal = (int)(numberToPrint / powdigit) % 10;
		digitpos.x -= charwidth;

		//kerning
		if(useKerning){
			if(digitVal == 1) digitpos.x += kern;
			if(digitValRight == 1) digitpos.x += kern;
			if(digitVal == 7 && digitValRight != 7) digitpos.x += kern;
			if(digitVal == 1 && digitValRight == 1) digitpos.x += kern;
		}

			if(dig != 0 && dig % 3 == 0) digitpos.x -= spacer;			//3 digit grouping

		numberSprite->SetCurrentFrame(digitVal);
		sVideo->Draw(numberSprite, digitpos);
		powdigit *= 10;
	}

}
int CZUILifebar::numberOfDigits(int number) {
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}