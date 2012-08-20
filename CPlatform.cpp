#include "CPlatform.h"
#include "SVideoManager.h"
#include "constants.h"

CPlatform::CPlatform(int x, int y) : CGameObject() {
	type = TYPE_PLATFORM;
	height = PLATFORM_HEIGHT;
	width = PLATFORM_WIDTH;
	
	position.x		= x + (width/2);
	position.y		= y - (height/2);
	
	
}

CPlatform::~CPlatform() {
}

void CPlatform::Draw() {
	GD4N::TVector2<int> drawposition;
	drawposition.x = position.x - (width/2);
	drawposition.y = position.y;
	sVideo->Draw(SURFID_PLATFORM, drawposition);
}

int CPlatform::GetHeight() {
	return height;
}

int CPlatform::GetWidth() {
	return width;
}