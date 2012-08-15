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
	frameForward	= true;
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
	if(sInput->GetKey(KEYBIND_RIGHT))		{		accelerate_right();		}
	if(sInput->GetKey(KEYBIND_LEFT))		{		accelerate_left();		}
	if(sInput->GetKey(KEYBIND_JUMP))		{		jump();					}
	else if(sInput->GetKeyUp(KEYBIND_JUMP)) {		breakjump();			}

	if(sInput->GetKey(KEYBIND_ATTACK))		{		attack();				}
	
	if(!sInput->GetKey(KEYBIND_JUMP) && !falling)	{	jumpfuel = jumpmax;			}
}


// TIMER
void CZero::timer() {
	float nowTime = SDL_GetTicks() * 0.01f;
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
	
	if(position.y > FLOORLEVEL) {							//	floor boundary
		position.y = FLOORLEVEL;
		vy = 0;
		zeroState = STATE_STANDING;
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

// ANIMATION
void CZero::readState() {
	
	if(!falling){	//on the ground
		int nvx = (vx * dt);
		if(nvx == 0) {	zeroState = STATE_STANDING;	}		// not moving
		else		 {	zeroState = STATE_RUNNING;	}		// moving
			
	}
	
	
}
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