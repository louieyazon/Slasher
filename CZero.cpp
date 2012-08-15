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
	position.x		= STARTING_X;
	position.y		= STARTING_Y;
	vy				= 0;
	vx				= 0;
	vx_max			= MAX_VX;
	vy_max			= MAX_VY;
	ay				= ZAY;
	ax				= ZAX;
	jumpmax			= JUMP_FUEL_MAX;
	jumpfuel		= jumpmax;
	falling			= true;
	previousTime	= SDL_GetTicks() * 0.01f;

	zeroState		= STATE_STANDING;
	animZeroState	= AS_STANDING;

	/*logicTimeBetween = 1.0000/50.0000;
	logicTimeLast = -logicTimeBetween;*/


	//animation
	curFrame		= 0;
	drawFrame		= 0;
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
	readState();
	Animate();
	lastZeroState = zeroState;
}
void CZero::Draw() {
	GD4N::TVector2<int> drawPosition;
	drawPosition.x = position.x - ZEROSPRITEOFFSET_X;		//Zero object's position is located at the feet of the sprite.
	drawPosition.y = position.y - ZEROSPRITEOFFSET_Y;
	sVideo->Draw(zeroTexture, drawPosition);
}

void CZero::ReactToInput(){	
	if(sInput->GetKey(KEYBIND_RIGHT))				{		accelerate_right();		}
	if(sInput->GetKey(KEYBIND_LEFT))				{		accelerate_left();		}
	if(sInput->GetKey(KEYBIND_JUMP))				{		jump();					}
	else if(sInput->GetKeyUp(KEYBIND_JUMP))			{		breakjump();			}

	if(sInput->GetKey(KEYBIND_ATTACK))				{		attack();				}
	
	//prevent hopping (by holding jump key)
	if(!sInput->GetKey(KEYBIND_JUMP) && !falling)	{	jumpfuel = jumpmax;			}	//re-enable jumping only when jump key is released and player has landed on the ground
}


// TIMER
void CZero::timer() {
	float nowTime = SDL_GetTicks() * 0.01f;		// i think it's better if we make an independent timer object and dt, a global variable so it can be universal.
	dt = nowTime - previousTime;
	previousTime = nowTime;	
}

// PHYSICS
void CZero::Physics() {
	friction();
	move();
	bound();
}
void CZero::move() {
	position.x += vx * dt;
	position.y += vy * dt;
}
void CZero::bound() {
	boundme(&position.x, LEFTWALL, RIGHTWALL);		//	left-right boundaries
	
	if(position.y > FLOORLEVEL) {					//	floor boundary
		position.y = FLOORLEVEL;
		vy = 0;
		zeroState = STATE_STANDING;
		animZeroState = AS_LANDING;	//animation modularity fail. let's try to figure this out later
		falling = false;
	}
}
void CZero::friction() {
	grind(&vx);

	//if( (int)(vx * dt * 100) == 0 ) vx = 0;

	if(vy < 0) grind(&vy);
	gravity();
}

void CZero::grind(float* v) {
	float vel = *v;
	float newMagnitude = (  abs(vel) - (FRIC_X * dt)  );
	if(newMagnitude < 0) newMagnitude = 0;
	*v = signof(vel) * newMagnitude;
}
void CZero::gravity() {
	if(falling) {
		vy += (dt * GRAVITY_ACC);
	}
}
void CZero::boundme (int* val, int min, int max) {
	if(*val > max) *val = max;
	if(*val < min) *val = min;
	if(*val < min || *val > max) { vx = 0.0f; }
}

// ANIMATION
void CZero::readState() {
	
	if(!falling){	//on the ground
		int nvx = (vx * dt);
		if(nvx == 0) {	zeroState = STATE_STANDING;	}		// not moving
		else		 {	zeroState = STATE_RUNNING;	}		// moving		
	}
	
	
}
void CZero::Animate() {
	setAnimationState();
	forwardFrame();
	decideFrame();
}

void CZero::setAnimationState() {
	if(zeroState == STATE_STANDING) {	// vx = 0
		if(animZeroState == AS_RUNNING) animZeroState = AS_STOPRUN;
		if(animZeroState != AS_LANDING && animZeroState != AS_STOPRUN) animZeroState = AS_STANDING;
	}
	if(zeroState == STATE_RUNNING) {	// vx != 0
		if(animZeroState == AS_STANDING) animZeroState = AS_STARTRUN;
		if(animZeroState != AS_STARTRUN) animZeroState = AS_RUNNING;
	}
	if(vy < 0) {			//going up
		if (animZeroState == AS_STANDING) animZeroState = AS_JUMPOFF;
		if (animZeroState != AS_JUMPOFF) animZeroState = AS_RISING;
	}
	if( abs((int)vy) < 10 && falling && animZeroState == AS_RISING) animZeroState = AS_JUMPTRANS;
	if(vy > 0) {			//going down
		if (animZeroState != AS_JUMPTRANS) animZeroState = AS_FALLING;
	}
}

void CZero::forwardFrame() {
	spriteTimeBetween = aCycle[animZeroState].delay;				// set sprite update rate from array
	if(spriteTimeLast + spriteTimeBetween < sTime->GetTime()){
		spriteTimeLast = sTime->GetTime();
		curFrame++;
	}
}
void CZero::decideFrame() {
	if(zeroState != lastZeroState) { curFrame = 0; }			// reset curFrame to show first frame if animation state changed
	int nMaxFrame = aCycle[animZeroState].numberOfFrames;		// 
	if(curFrame >= nMaxFrame) nextAnimState();					// when the last frame is reached, play the next animation
	drawFrame = aCycle[animZeroState].spriteFrame[curFrame];
	zeroTexture->SetCurrentFrame(drawFrame);
}

void CZero::nextAnimState() {
	curFrame = 0;
	animZeroState = aCycle[animZeroState].nextAnimState;
}

// MOVES
void CZero::accelerate_up()    {	vy -= ay * dt; minmaxf(&vy, -vy_max, vy_max); }
void CZero::accelerate_down()  {	vy += ay * dt; minmaxf(&vy, -vy_max, vy_max); }

void CZero::accelerate_left()  {	vx -= ax * dt; minmaxf(&vx, -vx_max, vx_max); }		//first statement accelerates vx; second statement limits maximum vx;
void CZero::accelerate_right() {	vx += ax * dt; minmaxf(&vx, -vx_max, vx_max); }

void CZero::jump()	{
	if(jumpfuel > 0) {
		if(!falling) {						//jump from ground
			zeroState = STATE_JUMPING;
			falling = true;
			accelerate_up();
		}
		//continue upward acceleration
		accelerate_up();
		jumpfuel -= JUMPCONSUMPTION * dt;
	}
}
void CZero::breakjump() {
	jumpfuel = 0;
}

void CZero::attack() {
	// TODO
	// set state to attacking/slashing
	// separate ground attack from air attack
	// detect the COMBO TIME WINDOW
}