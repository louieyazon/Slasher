#include "CPlatform.h"
#include "SVideoManager.h"
#include "constants.h"

CPlatform::CPlatform(int x, int y) : CGameObject() {
	type = TYPE_PLATFORM;
	position.x		= x;
	position.y		= y;
	
	height = PLATFROM_HEIGHT;
	width = PLATFORM_WIDTH;
}

CPlatform::~CPlatform() {
}

void CPlatform::Draw() {
	sVideo->Draw(SURFID_PLATFORM, position);
}

int CPlatform::GetHeight() {
	return height;
}

int CPlatform::GetWidth() {
	return width;
}