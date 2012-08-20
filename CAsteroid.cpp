#include "CAsteroid.h"
#include "SVideoManager.h"
#include "STimeManager.h"
#include "constants.h"
#include <ctime>
#include "SlasherUtil.h"
#include "SAudioManager.h"

CAsteroid::CAsteroid() : CGameObject() {
	radius = ASTEROID_RADIUS;
	spriteTimeBetween = 0.02;
	spriteTimeLast = sTime->GetTime();
	
	Respawn();
	
	explodesheet =  new GD4N::CSurfaceSheet(SURFID_EXPLODE);
	explodesheet->SetSpriteDimensions(2,9);
	explodesheet->SetCurrentFrame(explodeframe);

	explodesheet2 =  new GD4N::CSurfaceSheet(SURFID_EXPLODE);
	explodesheet2->SetSpriteDimensions(2,9);

	explodesheet3 =  new GD4N::CSurfaceSheet(SURFID_EXPLODE);
	explodesheet3->SetSpriteDimensions(2,9);

	explodesheet->SetCurrentFrame(explodeframe);
	


}

CAsteroid::~CAsteroid() {

}

void CAsteroid::explode() {
	exploded = true;
	sAudio->PlaySound(SFXID_EXPLODE);
}



void CAsteroid::Draw() {
	GD4N::TVector2<int> drawposition;
	if(exploded) {
		Animate();
		drawExplode();
	} else {
		drawposition.x = position.x - 40;
		drawposition.y = position.y - 40;
		sVideo->Draw(SURFID_ASTEROID, drawposition);
	}
}


void CAsteroid::drawExplode(){
	GD4N::TVector2<int> drawexposition;
	drawexposition.x = position.x - 50;
	drawexposition.y = position.y;
	sVideo->Draw(explodesheet, drawexposition);

	if(explodeframe2 >= 0) {
		drawexposition.x = position.x - 40;
		drawexposition.y = position.y - 40;
		sVideo->Draw(explodesheet2, drawexposition);
	}
	if(explodeframe3 >= 0) {
		drawexposition.x = position.x - 35;
		drawexposition.y = position.y + 10;
		sVideo->Draw(explodesheet3, drawexposition);
	}
}

void CAsteroid::Update() {
	if (exploded && explodeframe3 > 17) Respawn();
	if (!exploded) position.x -= dt * vx;
	bound();
}

void CAsteroid::Animate() {
	float timeGap = sTime->GetTime() - spriteTimeLast;
	
	if(timeGap > spriteTimeBetween){
		spriteTimeLast = sTime->GetTime();
		explodeframe++;
		if(explodeframe2 == 1) sAudio->PlaySound(SFXID_EXPLODE);
		//if(timeGap > spriteTimeBetween * 2) explodeframe++;			// frameskip 1
	}

	explodeframe2 = explodeframe - 5;
	explodeframe3 = explodeframe2 - 8;

	explodesheet->SetCurrentFrame(explodeframe);
	if(explodeframe2 >= 0) explodesheet2->SetCurrentFrame(explodeframe2);
	if(explodeframe3 >= 0) explodesheet3->SetCurrentFrame(explodeframe3);

	
}

void CAsteroid::bound() {
	if(position.x < 50 && !exploded) {
		explode();
		*zlifetarget -= 10;
	}
	
	/*if (position.x < HITPOS_X) {
		Respawn();
	}*/
}

void CAsteroid::Respawn() {
	exploded		= false;
	explodeframe	= -1;
	position.x		= ASPAWN_X;
	position.y		= randoValue(0, (int)FLOORLEVEL);
	vx				= randoValue(MIN_AVX, MAX_AVX); 
}

void CAsteroid::setLifeTarget(float* lifetarget) {
	zlifetarget = lifetarget;
}