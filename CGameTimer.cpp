#include "CGameTimer.h"
#include "SVideoManager.h"
#include "constants.h"
#include "STimeManager.h"
#include <time.h>

float previousTime;				// time of last update
float dt;

CGameTimer::CGameTimer() : CGameObject() {
}

CGameTimer::~CGameTimer() {
}

void CGameTimer::Update() {
	float nowTime = SDL_GetTicks() * 0.01f;		// i think it's better if we make an independent timer object and dt, a global variable so it can be universal.
	dt = nowTime - previousTime;
	previousTime = nowTime;	
}
