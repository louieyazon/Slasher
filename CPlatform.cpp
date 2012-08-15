#include "CPlatform.h"
#include "SVideoManager.h"
#include "constants.h"

CPlatform::CPlatform() : CGameObject() {
	type = TYPE_PLATFORM;
}

CPlatform::~CPlatform() {
}

void CPlatform::Draw() {
	sVideo->Draw(SURFID_PLATFORM);
}

int CPlatform::GetHeight() {
	return height;
}

int CPlatform::GetWidth() {
	return width;
}