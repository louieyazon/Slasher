#include "CAsteroid.h"
#include "SVideoManager.h"
#include "constants.h"
#include <ctime>
#include "SlasherUtil.h"

CAsteroid::CAsteroid() : CGameObject() {
	
	Respawn();

}

CAsteroid::~CAsteroid() {
}



void CAsteroid::Draw() {
	sVideo->Draw(SURFID_ASTEROID, position);
}

void CAsteroid::Update() {
	position.x -= dt * vx;
	if (position.x < HITPOS_X) {
		Respawn();
	}
}

void CAsteroid::Respawn() {
	position.x		= ASPAWN_X;
	position.y		= randoValue(0, (int)FLOORLEVEL);
	vx = randoValue(MIN_AVX, MAX_AVX); 
}