#include "SInputManager.h"
#include "SVideoManager.h"
#include "STimeManager.h"
#include "SAudioManager.h"

#include "CBGround.h"
#include "constants.h"
#include "CSlasherGameManager.h"
#include "CZero.h"
#include "SlasherUtil.h"
#include "CPlatform.h"
#include <cmath>

CZero::CZero() : CGameObject() {
	//PHYSICS
	position.x		= STARTING_X;
	position.y		= STARTING_Y;
	facingRight		= true;
	vy				= 0;
	vx				= 0;
	vx_max			= MAX_VX;
	vy_max			= MAX_VY;
	ay				= ZAY;
	ax				= ZAX;

	jumpmax			= JUMP_FUEL_MAX;
	jumpfuel		= jumpmax;

	dashmax			= DASH_FUEL_MAX;
	dashfuel		= dashmax;

	falling			= true;
	dashing			= false;
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
	zeroTexture->SetSpriteDimensions(14,10);

	zeroTextureL = new GD4N::CSurfaceSheet(SURFID_ZEROL);
	zeroTextureL->SetSpriteDimensions(14,10);

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

	if(facingRight)	sVideo->Draw(zeroTexture, drawPosition);
	else sVideo->Draw(zeroTextureL, drawPosition);
}

void CZero::ReactToInput(){	
	if(!dashing || (falling && dashing)) {
		if(sInput->GetKey(KEYBIND_RIGHT))			{		accelerate_right();		}
		if(sInput->GetKey(KEYBIND_LEFT))			{		accelerate_left();		}
	}

	if(sInput->GetKey(KEYBIND_JUMP))				{		jump();					}
	else if(sInput->GetKeyUp(KEYBIND_JUMP))			{		breakjump();			}
	
	if(!falling) {																		//ground-only controls
		if(sInput->GetKey(KEYBIND_DASH))			{		dash();					}
	}
	if(sInput->GetKeyUp(KEYBIND_DASH))			{		undash();				}

	

	if(sInput->GetKey(KEYBIND_ATTACK))				{		attack();				}
	
	//prevent hopping (by holding jump key)
	if(!sInput->GetKey(KEYBIND_JUMP) && !falling)	{		reenableJump();			}	//re-enable jumping only when jump key is released and player has landed on the ground
	if(!sInput->GetKey(KEYBIND_DASH) && !dashing)	{		reenableDash();			}	//re-enable jumping only when jump key is released and player has landed on the ground
}
void CZero::disableJump() {
	jumpfuel = 0;
}
void CZero::reenableJump() {
	jumpfuel = jumpmax;
}
void CZero::disableDash() {
	dashfuel = 0;
}
void CZero::reenableDash() {
	dashfuel = dashmax;
	dashing = false;
}


void CZero::readState() {
	// this funnction's usefulness needs reevaluation
	if(!falling){	//on the ground
		int nvx = (vx * dt);
		if(nvx == 0)			{	zeroState = STATE_STANDING;	}		// not moving
		else if(!dashing)		{	zeroState = STATE_RUNNING;	}		// moving		
		else					{	zeroState = STATE_DASHING;	}
		if(dashfuel <= 0)		{	dashing = false;			}
	}

	if(dashing) {		vx_max = DASH_VX;		}
	else		{		vx_max = MAX_VX;		}

}

// PHYSICS
void CZero::Physics() {
	friction();
	move();
	bound();
}
void CZero::move() {
	minmaxf(&vx, -vx_max, vx_max);
	position.x += (vx * dt) + 0.5;			//+0.5 is to compensate for int-casting's fraction elimination
	position.y += (vy * dt) + 0.5;			
}
void CZero::bound() {
	boundme(&position.x, LEFTWALL, RIGHTWALL);		//	left-right boundaries
	
	if(position.y > FLOORLEVEL) {					//	floor boundary
		position.y = FLOORLEVEL;
		land();										// stops gravity, removes vy, plays landing animation
	}
}
void CZero::friction() {
	grind(&vx);

	//if( (int)(vx * dt * 100) == 0 ) vx = 0;
	//if(vy < 0) grind(&vy);
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
void CZero::land() {
	vy = 0;
	zeroState = STATE_STANDING;
	animZeroState = AS_LANDING;	
	falling = false;
}

// ANIMATION
void CZero::Animate() {
	setAnimationState();
	forwardFrame();
	decideFrame();
}
void CZero::setAnimationState() {
	if(zeroState == STATE_STANDING) {	// vx = 0
		if(animZeroState == AS_RUNNING) animZeroState = AS_STOPRUN;
		if(
			!(
			animZeroState == AS_LANDING ||
			animZeroState == AS_STOPRUN ||
			animZeroState == AS_SLASH1 ||
			animZeroState == AS_SLASH2 ||
			animZeroState == AS_SLASH3 ||
			animZeroState == AS_KEEPSABER ||
			animZeroState == AS_BREAKING
			)
			
			) animZeroState = AS_STANDING;
		
	}
	if(dashing) {
		if(animZeroState != AS_DASHING && !falling) animZeroState = AS_INTODASH;
	}
	if(!dashing && animZeroState == AS_DASHING) {
		animZeroState = AS_BREAKING;
	}

	if(zeroState == STATE_RUNNING && animZeroState != AS_BREAKING) {	// vx != 0
		if(animZeroState == AS_STANDING) animZeroState = AS_STARTRUN;
		if(animZeroState != AS_STARTRUN) animZeroState = AS_RUNNING;
	}
	if(animZeroState != AS_AIRSLASH) {
		if(vy < 0) {						//going up
			if (animZeroState == AS_STANDING) animZeroState = AS_JUMPOFF;
			if (animZeroState != AS_JUMPOFF) animZeroState = AS_RISING;
		}
		if( abs((int)vy) < 10 && falling && animZeroState == AS_RISING) animZeroState = AS_JUMPTRANS;
		if(vy > 0) {						//going down
			if (animZeroState != AS_JUMPTRANS) animZeroState = AS_FALLING;
		}
	}
}
void CZero::forwardFrame() {
	spriteTimeBetween = aCycle[animZeroState].delay;				// set sprite update rate from array
	float timeGap = sTime->GetTime() - spriteTimeLast;

	if(timeGap > spriteTimeBetween){
		spriteTimeLast = sTime->GetTime();
		curFrame++;
		if(timeGap > spriteTimeBetween * 2) curFrame++;			// frameskip 1
	}
}
void CZero::decideFrame() {
	if(zeroState != lastZeroState) { curFrame = 0; }			// reset curFrame to show first frame if animation state changed
	int nMaxFrame = aCycle[animZeroState].numberOfFrames;		// 
	if(curFrame >= nMaxFrame) nextAnimState();					// when the last frame is reached, play the next animation
	drawFrame = aCycle[animZeroState].spriteFrame[curFrame];

	zeroTexture->SetCurrentFrame(drawFrame);
	zeroTextureL->SetCurrentFrame(drawFrame);
}
void CZero::nextAnimState() {
	curFrame = 0;
	animZeroState = aCycle[animZeroState].nextAnimState;
}

// MOVES
void CZero::accelerate_up()    {	
									vy = -ZJUMPY * dt;
									//vy -= ay * dt;
									minmaxf(&vy, -vy_max, vy_max);
}
void CZero::accelerate_down()	{	vy += ay * dt; minmaxf(&vy, -vy_max, vy_max);							}
void CZero::accelerate_left()	{	vx -= ax * dt; facingRight = false;	}		//first statement accelerates vx; second statement limits maximum vx;
void CZero::accelerate_right()	{	vx += ax * dt; facingRight = true;		}
void CZero::dashccelerate()		{	vx += ZDASH * dt * ((facingRight? 1 : -1));								}

void CZero::jump()	{
	if(jumpfuel > 0) {
		if(!falling) {						//jump from ground
			zeroState = STATE_JUMPING;
			falling = true;
			//accelerate_up();
		}
		//continue upward acceleration
		accelerate_up();
		jumpfuel -= JUMPCONSUMPTION * dt;
	}
}
void CZero::breakjump() {
	disableJump();
}
void CZero::dash() {
	// TODO implement dash
	// dash gives zero a short burst of forward acceleration in the direction he's facing
	// (like a horizontal jump)
	// the Physics > friction > grind function should probably take care of braking.

	if(dashfuel > 0) {
		if(!dashing) {						//jump from ground
			zeroState = STATE_DASHING;
			dashing = true;
		}
		//continue upward acceleration
		dashccelerate();
		dashfuel -= DASHCONSUMPTION * dt;
	}
}
void CZero::undash() {
	disableDash();
}

void CZero::attack() {
	// TODO
	// set state to attacking/slashing
	// separate ground attack from air attack
	// detect the COMBO TIME WINDOW
	
	
	// ATTACK CONTROL LOGIC:
	//
	// (1) don't allow the triggering of successive attacks by holding the attack button. see jump-hopping prevention code for reference.
	// (2) the next attack should only be allowed to trigger a short while after the current attack is triggered (COMBO TIME WINDOW)
	// (3) zero should not be allowed to move sideways in the middle of an attack.
	// (4) zero may jump at any point of the ground attack. this cancels the rest of the slash and allows him to do a normal jump.
	// (5) zero may dash at any point of the ground attack.
	// (6) 
	// 
	// ANIMATION-CENTRIC DUMMY ATTACK CONTROL LOGIC CODE. DO NOT USE AS IS.
	if(!falling &&
		(animZeroState == AS_STANDING ||		//these are the conditions when zero should be allowed to do a primary slash
		animZeroState == AS_KEEPSABER ||
		animZeroState == AS_LANDING ||
		animZeroState == AS_RUNNING ||
		animZeroState == AS_STARTRUN ||
		animZeroState == AS_STOPRUN
		)
		) {
			animZeroState = AS_SLASH1;
	}
	if (falling &&
		animZeroState != AS_AIRSLASH) {
			animZeroState = AS_AIRSLASH;
	}
	if(animZeroState == AS_SLASH1 && curFrame > 6) {
		animZeroState = AS_SLASH2;
		curFrame = 0;
	}
	if(animZeroState == AS_SLASH2 && curFrame > 6) {
		animZeroState = AS_SLASH3;
		curFrame = 0;
	}
	

}

//COLLISIONS
bool CZero::IsCollidingWith(GD4N::CGameObject* other){
	switch (other->GetType()) {
		case TYPE_PLATFORM:
			CPlatform* platform = dynamic_cast<CPlatform*>(other);
			if(vy > 0 &&														//Going down
				position.y > platform->position.y &&							//Under platform top
				position.y < (platform->position.y + platform->GetHeight()) &&  //Over platform bottom
				position.x > platform->position.x &&							//Within platfrom x1, x2
				position.x < (platform->position.x + platform->GetWidth()) ) { 
				return true;
			} else if (
				position.y+2 > platform->position.y &&							//Under platform top
				position.y+2 < (platform->position.y + platform->GetHeight()) &&  //Over platform bottom
				!(position.x > platform->position.x &&							//Within platfrom x1, x2
				position.x < (platform->position.x + platform->GetWidth()))	) {
				falling = true;
				disableJump();
				return false;
			} 
			break;
	}
	return false;
}

void CZero::CollidesWith(GD4N::CGameObject* other){
	switch (other->GetType()) {
		case TYPE_PLATFORM:
			CPlatform* platform = dynamic_cast<CPlatform*>(other);
			position.y = platform->position.y;
			land();
			break;    
    };
}