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
	position.x = 1;
	position.y = 1;
	fYSpeed = 0;
	fXSpeed = 0;

	zeroState = STATE_STANDING;

	curFrame = 20;
	frameForward = true;
	spriteTimeBetween = 1.0000/60.0000;
	spriteTimeLast = -spriteTimeBetween;

	logicTimeBetween = 1.0000/50.0000;
	logicTimeLast = -logicTimeBetween;

	zeroTexture = new GD4N::CSurfaceSheet(SURFID_ZERO);
	zeroTexture->SetSpriteDimensions(10,10);
	zeroTexture->SetCurrentFrame(curFrame);
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
	sVideo->Draw(zeroTexture, position);
}

void CZero::Animate() {
	switch(zeroState){
		case STATE_STANDING:
			spriteTimeBetween = 0.5f;
			break;
	}

	if(spriteTimeLast + spriteTimeBetween < sTime->GetTime()) {
		spriteTimeLast = sTime->GetTime();
		if(frameForward){
			curFrame++;
			if(curFrame > 21){
				frameForward = false;
			}
		} else {
			curFrame--;
			if(curFrame < 21) {
				frameForward = true;
			}
		}

		zeroTexture->SetCurrentFrame(curFrame);
	}

}

void CZero::Move() {
	
	position.x = position.x + logicTimeBetween * fXSpeed;

	//GRAVITY
	fYSpeed = fYSpeed + (logicTimeBetween*GRAVITY_ACC);
	position.y = position.y + (logicTimeBetween*fYSpeed);
	
	if(position.y > 500) {
		position.y = 500;
		fYSpeed = 0;
	}
}

void CZero::ReactToInput(){
	if(sInput->GetKeyDown(SDLK_RIGHT)) {
		zeroState = STATE_WALKING_RIGHT;
		fXSpeed = fXSpeed + 100;
	} else if (sInput->GetKeyUp(SDLK_RIGHT)) {
		zeroState = STATE_WALKING_RIGHT;
		fXSpeed = fXSpeed - 100;
	}

	if(sInput->GetKeyDown(SDLK_LEFT)) {
		zeroState = STATE_WALKING_LEFT;
		fXSpeed = fXSpeed -100;
	} else if(sInput->GetKeyUp(SDLK_LEFT)) {
		zeroState = STATE_WALKING_LEFT;
		fXSpeed = fXSpeed + 100;
	}

	if(sInput->GetKeyUp(SDLK_UP)) {
		zeroState = STATE_JUMPING;
		fYSpeed = -100;
	}
}

