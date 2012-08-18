#include "CZUILifebar.h"
#include "SVideoManager.h"
#include "constants.h"

CZUILifebar::CZUILifebar() : CGameObject() {
	position.x				= LIFEBAR_X;
	position.y				= LIFEBAR_Y;

	srcposition.x			= 0;
	srcposition.y			= 0;			//max is LIFEBAR_MAXSPRITEY

	maxwidth				= LIFEBAR_MAXWIDTH;
	lifeTargetPercent		= 100.0;
	updateDelay				= LIFEBAR_UPDATERATE;

	height					= LIFEBAR_SPRITEHEIGHT;
	width					= LIFEBAR_MAXWIDTH;
}

CZUILifebar::~CZUILifebar() {
}

//sVideo->Draw(SURFID_LIFEBAR, position); base statement
void CZUILifebar::Draw() {
	sVideo->Draw(SURFID_LIFEBAR, position, srcposition, width, height);
}

int CZUILifebar::GetHeight() {
	return height;
}

int CZUILifebar::GetWidth() {
	return width;
}

void CZUILifebar::Update() {
	//take constant damage for testing
	//decreaseTargetPercent(0.00001);
	//srcposition.y = 0 + (int)(lifeTargetPercent / 100) * LIFEBAR_MAXSPRITEY;
}

void CZUILifebar::setTargetPercent(float x) {
	lifeTargetPercent = x;
	if (lifeTargetPercent <= 0) lifeTargetPercent = 0;
}

void CZUILifebar::decreaseTargetPercent(float d) {
	setTargetPercent(lifeTargetPercent - d);
}