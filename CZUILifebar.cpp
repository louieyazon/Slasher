#include "CZUILifebar.h"
#include "SVideoManager.h"
#include "constants.h"

CZUILifebar::CZUILifebar() : CGameObject() {
	position.x				= LIFEBAR_X;
	position.y				= LIFEBAR_Y;

	srcposition.x			= 0;
	srcposition.y			= 0;			//max is LIFEBAR_MAXSPRITEY

	srcposition_lag.x		= 0;
	srcposition_lag.y		= 0;

	maxwidth				= LIFEBAR_MAXWIDTH;

	//LIFE UPDATE PROBLEM
	//we may need to pass this constructor a pointer to the hitpoints variable of another object.
	//it seems like the only other way for objects to communicate is through the collision engine. (which is kinda weird)
	//the objects in the scene are anonymous, and i'm not sure how to access them if we gave them variable names.
	lifeTargetPercent		= 100.0;

	
	updateDelay				= LIFEBAR_UPDATERATE;

	height					= LIFEBAR_SPRITEHEIGHT;
	width					= LIFEBAR_MAXWIDTH;
}

CZUILifebar::~CZUILifebar() {
}

//sVideo->Draw(SURFID_LIFEBAR, position); base statement
void CZUILifebar::Draw() {
	sVideo->Draw(SURFID_LIFEBARDIFF, position, srcposition_lag, width, height);
	sVideo->Draw(SURFID_LIFEBAR, position, srcposition, width, height);
}

void CZUILifebar::Update() {
	if(	(rand() % 200)	==	1) {		//take random damage at random intervals for testing
		int dmg = rand() % 10;
		decreaseTargetPercent(dmg);	
	}

	if(srcposition.y != srcposition_lag.y) srcposition_lag.y++;	//TODO change this to be time dependent
	srcposition.y = 0 + (int)((100 - lifeTargetPercent) / 100 * LIFEBAR_MAXSPRITEY);
}

void CZUILifebar::setTargetPercent(float x) {
	lifeTargetPercent = x;
	//if (lifeTargetPercent <= 0) lifeTargetPercent = 0;		real code
	if (lifeTargetPercent <= 0) {lifeTargetPercent = 100;	srcposition_lag.y = 0;}			//DEBUG CODE
}

void CZUILifebar::decreaseTargetPercent(float d) {
	setTargetPercent(lifeTargetPercent - d);
}