#include "CAsteroid.h"
#include "SVideoManager.h"
#include "STimeManager.h"
#include "constants.h"
#include <ctime>
#include "SlasherUtil.h"
#include "SAudioManager.h"
#include "CGameTimer.h"

CAsteroid::CAsteroid(float spawndelay) : CGameObject() {
	//initial spawn behavior
	sceneStartTime			= sTime->GetTime();
	asteroidspawndelay		= spawndelay;
	asteroidOn				= false;

	//subsequent spawn behavior
	generateNextY();
	Respawn();
	arrow_y					= next_y;
	
	//collision 
	type					=	TYPE_ASTEROID;
	radius					= ASTEROID_RADIUS;
	hitpoints				= ASTEROIDMAXHP;

	//animation stuff
	spriteTimeBetween		= 0.02;
	spriteTimeLast			= sTime->GetTime();

	initExplodes();
}
void CAsteroid::initExplodes() {
	for(int r = 0; r < EXPLODE_SPRITES_PER_ASTEROID; r++){
		//init sheet
		explosion[r].explodesheet = new GD4N::CSurfaceSheet(SURFID_EXPLODE);
		(explosion[r].explodesheet)->SetSpriteDimensions(2,9);
		(explosion[r].explodesheet)->SetCurrentFrame(explosion[r].frame);
		
		explosion[r].v.x = -7 + (rand() % 7);
		explosion[r].v.y = (rand() % 7) - (rand() % 7);
	}
	resetExplodes();

	explosion[0].initoffset.Assign(-15, 0);
	explosion[1].initoffset.Assign(-40, -40);
	explosion[2].initoffset.Assign(-35, 10);
}


CAsteroid::~CAsteroid() {
	cleanupExplodeSheets();	//delete sheets from explode array
}
void CAsteroid::cleanupExplodeSheets() {
	for(int r = 0; r < EXPLODE_SPRITES_PER_ASTEROID; r++){
		if(explosion[r].explodesheet != NULL) { delete explosion[r].explodesheet; }
		explosion[r].explodesheet = 0;
	}
}




void CAsteroid::Draw() {
	GD4N::TVector2<int> drawposition;
	if(exploded) {
		Animate();
		drawExplode();
	} else {
		drawposition.x = position.x - 60;
		drawposition.y = position.y - 60;
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
	for(int r = 0; r < EXPLODE_SPRITES_PER_ASTEROID; r++) {
		if (explosion[r].isExploding) sVideo->Draw(explosion[r].explodesheet, explosion[r].position);
	}
}
void CAsteroid::Animate() {
	float timeGap = sTime->GetTime() - spriteTimeLast;
	
	if(timeGap > spriteTimeBetween){
		spriteTimeLast = sTime->GetTime();
		for(int r = 0; r < EXPLODE_SPRITES_PER_ASTEROID; r++) {
			//implement delays between explosions



			if(r > 0 && explosion[r].isExploding == false && explosion[r-1].frame >= explosion[r].framesAfterPrevious) {
				explosion[r].isExploding = true;
				explosion[r].frame = -1;			//so the next line starts at 0
			}

			if(explosion[r].isExploding) {		//apply frame animation to exploding explosion
				//if(timeGap > spriteTimeBetween * 2) explosion[r].explodeframe++;			// frameskip 1
				explosion[r].frame++;
				explosion[r].explodesheet->SetCurrentFrame(explosion[r].frame);
				moveExplosion(r);
			}

		}

		if(explosion[ EXPLODE_SPRITES_PER_ASTEROID / 2].frame == 1) sAudio->PlaySound(SFXID_EXPLODE);		//play sound
	}
}
void CAsteroid::moveExplosion(int r){
	explosion[r].position.x = explosion[r].position.x + (explosion[r].v.x);
	explosion[r].position.y = explosion[r].position.y + (explosion[r].v.y);
}

void CAsteroid::Update() {
	if(!asteroidOn) {
		if(sTime->GetTime() > (asteroidspawndelay + sceneStartTime))
			asteroidOn = true;
	}

	if(gameOn && asteroidOn){
		if (!exploded) position.x -= dt * vx;
		bound();
	}
}
void CAsteroid::bound() {
	if (exploded && explosion[ EXPLODE_SPRITES_PER_ASTEROID - 1 ].frame >= 17) Respawn();

	if(position.x < CUTEFACTORY_X && !exploded) {
		explode();
		*zlifetarget -= ASTEROID_DAMAGE;
	}

}


void CAsteroid::Respawn() {
	
	exploded		= false;
	explodeframe	= -1;
	explodeframe3	= 0;
	position.x		= ASPAWN_X;
	position.y		= next_y;
	vx				= randoValue(MIN_AVX, MAX_AVX);
	generateNextY();
	resetExplodes();
}
void CAsteroid::resetExplodes() {
	//init values
	for(int r = 0; r < EXPLODE_SPRITES_PER_ASTEROID; r++){
		explosion[r].frame = -1;
		explosion[r].isExploding = false;
		explosion[r].position.Assign(0,0);
		explosion[r].framesAfterPrevious = 4 + (rand() % 4);
		int rx = -20 - (rand() % 30);
		int ry = (rand() % 50);
		int ry2 = (rand() % 50);
		explosion[r].initoffset.Assign(rx, ry - ry2);
	}
}
void CAsteroid::generateNextY() {
	next_y = randoValue(ASPAWN_YMIN, ASPAWN_YMAX);
}

void CAsteroid::explode() {
	setExplodeInitPositions();
	explosion[0].isExploding = true;
	exploded = true;
	sAudio->PlaySound(SFXID_EXPLODE);
}
void CAsteroid::setExplodeInitPositions(){
	for(int r = 0; r < EXPLODE_SPRITES_PER_ASTEROID; r++) {
		explosion[r].position.Assign (  position.x + explosion[r].initoffset.x		,		position.y + explosion[r].initoffset.y  );
	}
}


void CAsteroid::takeDamage(int dmg) {
	explode();
}
void CAsteroid::setLifeTarget(float* lifetarget) {
	zlifetarget = lifetarget;
}