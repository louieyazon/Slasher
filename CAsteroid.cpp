#include "CAsteroid.h"
#include "SVideoManager.h"
#include "STimeManager.h"
#include "constants.h"
#include <ctime>
#include "SlasherUtil.h"
#include "SAudioManager.h"

CAsteroid::CAsteroid(float spawndelay) : CGameObject() {
	asteroidspawndelay = spawndelay;
	asteroidOn = false;
	radius = ASTEROID_RADIUS;
	spriteTimeBetween = 0.02;
	spriteTimeLast = sTime->GetTime();
	type = TYPE_ASTEROID;


	generateNextY();
	Respawn();
	arrow_y = next_y;

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
	delete explodesheet;
	delete explodesheet2;
	delete explodesheet3;
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

	
	if(exploded || position.x < 300) {
		GD4N::TVector2<int> arrowposition;
		if(arrow_y != next_y) arrow_y += (next_y - arrow_y) / 20;

		arrowposition.x = 750;
		arrowposition.y = arrow_y - 10;
		sVideo->Draw(SURFID_ARROW, arrowposition);
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
	if(!asteroidOn) {
		if(sTime->GetTime() > asteroidspawndelay) asteroidOn = true;
	}

	if(gameOn && asteroidOn){
		if (!exploded) position.x -= dt * vx;
		bound();
	}
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
	if (exploded && explodeframe3 > 17) Respawn();

	if(position.x < CUTEFACTORY_X && !exploded) {
		explode();
		*zlifetarget -= ASTEROID_DAMAGE;
	}

}

void CAsteroid::Respawn() {
	exploded		= false;
	explodeframe	= -1;
	position.x		= ASPAWN_X;
	position.y		= next_y;
	vx				= randoValue(MIN_AVX, MAX_AVX);
	generateNextY();
}

void CAsteroid::generateNextY() {
	next_y = randoValue(0, (int)FLOORLEVEL);
}

void CAsteroid::setLifeTarget(float* lifetarget) {
	zlifetarget = lifetarget;
}