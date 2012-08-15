#include "SInputManager.h"
#include "SVideoManager.h"
#include "STimeManager.h"
#include "SAudioManager.h"

#include "CBGround.h"
#include "constants.h"
#include "CSlasherGameManager.h"
#include "CZero.h"

CZero::CZero() : CGameObject() {
	
	//PHYSICS
	position.x = 150;
	position.y = 100;
	fYSpeed = 0;
	fXSpeed = 0;
	jumpmax = 3;
	jumpfuel = jumpmax;
	falling = true;

	zeroState = STATE_STANDING;
	animZeroState = AS_STANDING;

	logicTimeBetween = 1.0000/50.0000;
	logicTimeLast = -logicTimeBetween;


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
	//if(logicTimeLast + logicTimeBetween < sTime->GetTime()) {
		//logicTimeLast = sTime->GetTime();
	ReactToInput();
	Move();
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
	


	//FORWARD FRAME BASED ON FRAME DELAY
	spriteTimeBetween = cycleFPS[animZeroState];				// set sprite update rate from array
	if(spriteTimeLast + spriteTimeBetween < sTime->GetTime()){
		spriteTimeLast = sTime->GetTime();
		curFrame++;
	}

	//DECIDE FRAME
	if(zeroState != lastZeroState) { curFrame = 0; } // reset curFrame to show first frame if animation state changed
	int nMaxFrame = cycleFN[animZeroState];			 // 

	if(curFrame >= nMaxFrame) curFrame = 0;			// return to first frame if max frame is reached

	if(animZeroState == AS_STANDING) drawFrame = sfZeroStand[curFrame];
	if(animZeroState == AS_RUNNING) drawFrame = sfZeroRun[curFrame];
	zeroTexture->SetCurrentFrame(drawFrame);
	
	lastZeroState = zeroState;
	
}

void CZero::Move() {
	
	position.x = position.x + logicTimeBetween * fXSpeed;

	//GRAVITY
	if(falling) {
		fYSpeed = fYSpeed + (logicTimeBetween*GRAVITY_ACC);
		position.y = position.y + (logicTimeBetween*fYSpeed);
	}
	
	if(position.y > 500) {
		position.y = 500;
		fYSpeed = 0;
		zeroState = STATE_STANDING;
		jumpfuel = jumpmax;
		falling = false;
	}
}
void CZero::ReactToInput(){
	zeroState = STATE_STANDING;
	
	if(sInput->GetKey(SDLK_RIGHT)) {
		zeroState = STATE_RUNNING;
		fXSpeed = 150;
	} else if (sInput->GetKeyUp(SDLK_RIGHT)) {
		zeroState = STATE_RUNNING;
		fXSpeed = 0;
	}

	if(sInput->GetKey(SDLK_LEFT)) {
		zeroState = STATE_RUNNING;
		fXSpeed = -100;
	} else if(sInput->GetKeyUp(SDLK_LEFT)) {
		zeroState = STATE_RUNNING;
		fXSpeed = 0;
	}

	if(sInput->GetKey(SDLK_UP) && jumpfuel > 0) {
		zeroState = STATE_JUMPING;
		fYSpeed -= 200;
		jumpfuel--;
		falling = true;
	} 
	if(sInput->GetKeyUp(SDLK_UP)) {
		fYSpeed = 0;
		jumpfuel = 0;
	}
}


void CZero::newAnimateState() {


}