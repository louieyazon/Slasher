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
	ay						= GRAVITY_ACC * 2;
	spriteTimeBetween		= 0.02;
	spriteTimeLast			= sTime->GetTime();

	//asteroidbits stuff
	asteroidbitssheet = new GD4N::CSurfaceSheet(SURFID_ASTEROIDBITS);
	asteroidbitssheet->SetSpriteDimensions(2,2);
	asteroidbitssheet->SetCurrentFrame(0);

	initExplodes();
	initAsteroidBits();
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

	/*explosion[0].initoffset.Assign(-15, 0);
	explosion[1].initoffset.Assign(-40, -40);
	explosion[2].initoffset.Assign(-35, 10);*/
}
void CAsteroid::initAsteroidBits() {
	randAsteroidBitspeed();
}


CAsteroid::~CAsteroid() {
	cleanupSheets();	//delete sheets from explode array and bits array
}
void CAsteroid::cleanupSheets() {
	delete asteroidbitssheet;
	
	for(int r = 0; r < EXPLODE_SPRITES_PER_ASTEROID; r++){
		if(explosion[r].explodesheet != NULL) { delete explosion[r].explodesheet; }
		explosion[r].explodesheet = 0;
	}
}


void CAsteroid::Draw() {
	GD4N::TVector2<int> drawposition;
	if(exploded) {
		Animate();
		drawBits();
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
void CAsteroid::drawBits(){
	for(int r = 0; r < 4; r++) {
		asteroidbitssheet->SetCurrentFrame(r);
		sVideo->Draw(asteroidbitssheet, flyingbits[r].position);
	}
}
void CAsteroid::Animate() {
	float timeGap = sTime->GetTime() - spriteTimeLast;

	if(exploded) {
		for(int r = 0; r < 4; r++) {
			moveBits(r);
		}
	}
	
	if(timeGap > spriteTimeBetween){			// frame dependent
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
void CAsteroid::moveBits(int r) {
	flyingbits[r].position.x = flyingbits[r].position.x + (flyingbits[r].v.x * dt);
	flyingbits[r].position.y = flyingbits[r].position.y + (flyingbits[r].v.y * dt);
	flyingbits[r].v.y += ay * dt;
}

void CAsteroid::Respawn() {
	exploded		= false;
	hitpoints		= ASTEROIDMAXHP;
	position.Assign(ASPAWN_X, next_y); 
	vx				= randoValue(MIN_AVX, MAX_AVX);
	generateNextY();
	resetExplodes();
	resetAsteroidBits();
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

void CAsteroid::resetAsteroidBits(){
	for(int r = 0; r < 4; r++){
		flyingbits[r].position.Assign(0,0);
		int rx = (rand() % 50) - 20;
		int ry = 60;
		int ry2 = (rand() % 30);
		flyingbits[r].initoffset.Assign(rx, ry - ry2);
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
		
		if(r < 4) flyingbits[r].position.Assign (	position.x + flyingbits[r].initoffset.x		,		position.y + flyingbits[r].initoffset.y	);
		explosion[r].position.Assign			(  position.x + explosion[r].initoffset.x		,		position.y + explosion[r].initoffset.y  );
	}
	randAsteroidBitspeed();
}
void CAsteroid::randAsteroidBitspeed() {
	for(int r = 0; r < 4; r++){
		flyingbits[r].v.x = vx * 2;
		flyingbits[r].v.y = -(rand() % 50);
	}
}


bool CAsteroid::takeDamage(int dmg) {
	hitpoints -= dmg;
	if(hitpoints < 0) { 
		explode();
		return true;
	}
	return false;
}
void CAsteroid::setLifeTarget(float* lifetarget) {
	zlifetarget = lifetarget;
}