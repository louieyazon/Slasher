#include "SInputManager.h"
#include "SVideoManager.h"
#include "STimeManager.h"
#include "SAudioManager.h"

#include "CBGround.h"
#include "constants.h"
#include "CSlasherGameManager.h"
#include "CZero.h"
#include "SlasherUtil.h"
#include <cmath>

CZero::CZero() : CGameObject() {
	//PHYSICS
	position.x = 150;
	position.y = 100;
	vy = 0;
	vx = 0;
	vx_max = 400.0f;
	ay = ZAY;
	ax = ZAX;
	jumpmax = 3;
	jumpfuel = jumpmax;
	falling = true;

	zeroState = STATE_STANDING;
	animZeroState = AS_STANDING;

	/*logicTimeBetween = 1.0000/50.0000;
	logicTimeLast = -logicTimeBetween;*/


	//animation
	curFrame = 0;
	drawFrame = 0;
	frameForward = true;
	spriteTimeBetween = 1.0000/60.0000;
	spriteTimeLast = sTime->GetTime();



	zeroTexture = new GD4N::CSurfaceSheet(SURFID_ZERO);
	zeroTexture->SetSpriteDimensions(12,10);
	zeroTexture->SetCurrentFrame(drawFrame);


}
CZero::~CZero() {
	delete zeroTexture;
	zeroTexture = 0;
}

void CZero::Update() {
	/*if(logicTimeLast + logicTimeBetween < sTime->GetTime()) {
		logicTimeLast = sTime->GetTime();
	}*/
	timer();
	ReactToInput();
	Physics();
	Animate();
}
void CZero::Draw() {
	GD4N::TVector2<int> drawPosition;
	drawPosition.x = position.x - 88;
	drawPosition.y = position.y - 93;
	sVideo->Draw(zeroTexture, drawPosition);
}
void CZero::Animate() {
	
	if(zeroState == STATE_STANDING) animZeroState = AS_STANDING;
	if(zeroState == STATE_RUNNING) animZeroState = AS_RUNNING;
	
	forwardFrame();
	decideFrame();
	
	lastZeroState = zeroState;
	
}

void CZero::ReactToInput(){
	zeroState = STATE_STANDING;
	
	if(sInput->GetKey(SDLK_RIGHT)) {
		zeroState = STATE_RUNNING;
		accelerate_right();
	}	//else if (sInput->GetKeyUp(SDLK_RIGHT)) {	zeroState = STATE_RUNNING;	}

	if(sInput->GetKey(SDLK_LEFT)) {
		zeroState = STATE_RUNNING;
		accelerate_left();
	}	//else if(sInput->GetKeyUp(SDLK_LEFT)) {		zeroState = STATE_RUNNING;	ax = 0;	}

	if(sInput->GetKey(SDLK_SPACE) && jumpfuel > 0) {
		jump();
	} 
	else if(sInput->GetKeyUp(SDLK_SPACE)) {
		ay = 0;
		jumpfuel = 0;
	}
}


// TIMER
void CZero::timer() {
	float nowTime = SDL_GetTicks() * 0.001f;
	dt = nowTime - previousTime;
	previousTime = nowTime;	
}

// PHYSICS
void CZero::Physics() {
	move();
	bound();
	friction();
}
void CZero::move() {
	position.x += vx * dt;
	position.y += vy * dt;
}
void CZero::bound() {
	boundme(&position.x, LEFTWALL, RIGHTWALL);
	if(position.y > 500) {
		position.y = 500;
		vy = 0;
		zeroState = STATE_STANDING;
		jumpfuel = jumpmax;
		falling = false;
	}
}
void CZero::friction() {
	grind(&vx);
	gravity();
}

void CZero::grind(float* v) {
	int vel = *v;
	int newMagnitude = (  abs(vel) - (FRIC_X * dt)  );
	if(newMagnitude < 0) newMagnitude = 0;
	*v = signof(vel) * newMagnitude;
}
void CZero::gravity() {
	if(falling) {
		vy += (dt * GRAVITY_ACC);
	}
}

// ANIMATION
void CZero::forwardFrame() {
	spriteTimeBetween = cycleFPS[animZeroState];				// set sprite update rate from array
	if(spriteTimeLast + spriteTimeBetween < sTime->GetTime()){
		spriteTimeLast = sTime->GetTime();
		curFrame++;
	}
}
void CZero::decideFrame() {
	if(zeroState != lastZeroState) { curFrame = 0; } // reset curFrame to show first frame if animation state changed
	int nMaxFrame = cycleFN[animZeroState];			 // 

	if(curFrame >= nMaxFrame) curFrame = 0;			// return to first frame if max frame is reached

	if(animZeroState == AS_STANDING) drawFrame = sfZeroStand[curFrame];
	if(animZeroState == AS_RUNNING) drawFrame = sfZeroRun[curFrame];
	zeroTexture->SetCurrentFrame(drawFrame);
}

// MOVEMENT
void CZero::accelerate_up()    {	vy -= ay * dt; minmaxf(&vy, -vx_max, vx_max); }
void CZero::accelerate_down()  {	vy += ay * dt; minmaxf(&vy, -vx_max, vx_max); }
void CZero::accelerate_left()  {	vx -= ax * dt; minmaxf(&vx, -vx_max, vx_max); }
void CZero::accelerate_right() {	vx += ax * dt; minmaxf(&vx, -vx_max, vx_max); }
void CZero::jump()	{
	zeroState = STATE_JUMPING;
	ay -= 200;
	jumpfuel--;
	falling = true;
}