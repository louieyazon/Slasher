#include "CBGround.h"
#include "SVideoManager.h"
#include "constants.h"

CBGround::CBGround() : CGameObject() {
	type = TYPE_GROUND;
}

CBGround::~CBGround() {
}

void CBGround::Draw() {
	sVideo->Draw(SURFID_BKGD);
}

int CBGround::GetHeight() {
	return height;
}

int CBGround::GetWidth() {
	return width;
}