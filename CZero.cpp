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
	fricx			= FRIC_X;

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

	UINumbersTexture = new GD4N::CSurfaceSheet(SURFID_UINUMBERS);
	UINumbersTexture->SetSpriteDimensions(1,12);

	zeroTexture = new GD4N::CSurfaceSheet(SURFID_ZERO);
	zeroTexture->SetSpriteDimensions(14,10);

	zeroTextureL = new GD4N::CSurfaceSheet(SURFID_ZEROL);
	zeroTextureL->SetSpriteDimensions(14,10);

	zeroTexture->SetCurrentFrame(drawFrame);

	sAudio->PlaySound(SFXID_READY);


}
CZero::~CZero() {
	delete zeroTexture;
	delete zeroTextureL;
	zeroTexture = 0;
	zeroTextureL = 0;
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

	if(SHOW_DEBUG_NUMBERS) DrawDebug();						//draw debug numbers;
}

void CZero::ReactToInput(){	

	if(!dashing || (falling && dashing)) {
		if(animZeroState != AS_SLASH1 && animZeroState != AS_SLASH2 && animZeroState != AS_SLASH3) {		//DUMMY CODE: remove this after animation-centric code is removed
			if(sInput->GetKey(KEYBIND_RIGHT))			{		accelerate_right();		}
			if(sInput->GetKey(KEYBIND_LEFT))			{		accelerate_left();		}
		}

	}

	if(sInput->GetKey(KEYBIND_JUMP))				{		jump();					}
	else if(sInput->GetKeyUp(KEYBIND_JUMP))			{		breakjump();			}
	
	if(!falling) {																		//ground-only controls
		if(sInput->GetKey(KEYBIND_DASH))			{		dash();					}
	}
	if(sInput->GetKeyUp(KEYBIND_DASH))				{		undash();				}

	

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
	if(!falling){	//on the ground
		int nvx = (vx * dt);
		if(nvx == 0)			{	zeroState = STATE_STANDING;	}		// not moving
		else if(!dashing)		{	zeroState = STATE_RUNNING;	}		// moving		
		else					{	zeroState = STATE_DASHING;	}

		if(dashfuel <= 0)		{	dashing = false;			}
	}

	
	if(falling) { // in air
		if(vy > 0) disableJump();
	}

	//falling-landing independent
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
	if (falling) fricx = FRIC_X/3;
	else fricx = FRIC_X;
		
	grind(&vx);

	//if( (int)(vx * dt * 100) == 0 ) vx = 0;
	//if(vy < 0) grind(&vy);
	gravity();
}

void CZero::grind(float* v) {
	float vel = *v;
	float newMagnitude = (  abs(vel) - (fricx * dt)  );
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
	sAudio->PlaySound(SFXID_ZLAND);
}

// ANIMATION
void CZero::Animate() {
	setAnimationState();
	forwardFrame();
	decideFrame();
	
}
void CZero::frameSound() {
	if(animZeroState == AS_RUNNING) {
		if(curFrame == 2)	sAudio->PlaySound(SFXID_STEP1);
		else if(curFrame == 9)	sAudio->PlaySound(SFXID_STEP2);
	}

	if(curFrame == 0) {			//sounds played at start of animation

	}
}
void CZero::setAnimationState() {
	if(!falling){		//ground animations
		if(zeroState == STATE_STANDING) {	// vx = 0
			if(animZeroState == AS_RUNNING) animZeroState = AS_STOPRUN;
		}

		if(zeroState == STATE_RUNNING && animZeroState != AS_BREAKING && !dashing) {	// vx != 0
			if(animZeroState == AS_STANDING) animZeroState = AS_STARTRUN;
			if(animZeroState != AS_STARTRUN) animZeroState = AS_RUNNING;
		}
		
		if(dashing) {
			if(animZeroState != AS_DASHING) animZeroState = AS_INTODASH;
		} else if (!dashing && animZeroState == AS_DASHING) {
			animZeroState = AS_BREAKING;
			sAudio->PlaySound(SFXID_ZSKID);
		}

		
	}

	if(falling){		//air animations
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
}
void CZero::forwardFrame() {
	spriteTimeBetween = aCycle[animZeroState].delay;				// set sprite update rate from array
	float timeGap = sTime->GetTime() - spriteTimeLast;
	
	if(timeGap > spriteTimeBetween){
		spriteTimeLast = sTime->GetTime();
		curFrame++;
		if(timeGap > spriteTimeBetween * 2) curFrame++;			// frameskip 1
		frameSound();
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
void CZero::accelerate_up()    {	// jump
									vy = -ZJUMPY * dt;
									//vy -= ay * dt;
									minmaxf(&vy, -vy_max, vy_max);
}
void CZero::accelerate_down()	{	vy += ay * dt; minmaxf(&vy, -vy_max, vy_max);		}	//unused
void CZero::accelerate_left()	{	vx -= ax * dt; facingRight = false;					}
void CZero::accelerate_right()	{	vx += ax * dt; facingRight = true;					}
void CZero::dashccelerate()		{	vx += ZDASH * dt * ((facingRight? 1 : -1));			}

void CZero::jump()	{
	if(jumpfuel > 0) {
		if(!falling) {						//jump from ground
			zeroState = STATE_JUMPING;
			falling = true;
			undash();
			if(rand() % 2 == 1) sAudio->PlaySound(SFXID_ZJUMP);
			else sAudio->PlaySound(SFXID_ZJUMP2);
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
			sAudio->PlaySound(SFXID_ZDASH);
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
	// (4) you may trigger an attack while walking, but you should not be able to walk again while in the middle of an attack.
	// (5) zero may jump at any point of the ground attack. this cancels the rest of the slash and allows him to do a normal jump.
	// (6) zero may dash at any point of the ground attack.
	
	// 
	// ANIMATION-CENTRIC DUMMY ATTACK CONTROL LOGIC CODE. DO NOT USE AS IS BECAUSE THIS IS REALLY BUGGY.
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
			sAudio->PlaySound(SFXID_ZSLASH1);
			spriteTimeLast = sTime->GetTime();
	}
	if (falling &&
		animZeroState != AS_AIRSLASH) {
			animZeroState = AS_AIRSLASH;
			curFrame = 0;
			sAudio->PlaySound(SFXID_ZSLASHAIR);
	}
	if(animZeroState == AS_SLASH1 && curFrame > 7) {
		animZeroState = AS_SLASH2;
		sAudio->PlaySound(SFXID_ZSLASH2);
		curFrame = 0;
		spriteTimeLast = sTime->GetTime();
	}
	if(animZeroState == AS_SLASH2 && curFrame > 7) {
		animZeroState = AS_SLASH3;
		sAudio->PlaySound(SFXID_ZSLASH3);
		curFrame = 0;
		spriteTimeLast = sTime->GetTime();
	}
	

}

//COLLISIONS
bool CZero::IsCollidingWith(GD4N::CGameObject* other){
	switch (other->GetType()) {
		case TYPE_PLATFORM:
			CPlatform* platform = dynamic_cast<CPlatform*>(other);

			int diffpos_x = position.x - platform->position.x;						// + if position.value is greater
			int diffpos_y = position.y - platform->position.y;						// - if position.value is less
			int diffpos_yUnder = (position.y + 2) - platform->position.y;

			bool isYCollide			=	abs(diffpos_y) < platform->GetHeight()/2;		// y within platform rectangle
			bool isXCollide			=	abs(diffpos_x) < platform->GetWidth()/2;		// within width of platform
			bool isYCollideUnder	=	abs(diffpos_yUnder) < platform->GetHeight()/2;
				
			if(vy > 0	&&	isYCollide	&& isXCollide) {	// going down & position.xy is within platform rectangle
				return true;
			} else if (isYCollideUnder && !isXCollide) {	// position.x not within platform width & position.y is right above platform rectangle.
				falling = true;
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

// DEBUG NUMBERS
void CZero::DrawDebug(){

	debugNumber(250, 550, 3, &jumpfuel);
	debugNumber(290, 550, 3, &dashfuel);

	//speed + position debug
	debugNumber(350, 550, 3, &position.x);
	debugNumber(390, 550, 3, &position.y);
	debugNumber(350, 570, 3, &vx);
	debugNumber(390, 570, 3, &vy);

	//sprite animator debug
	debugNumber(230, 570, 2, &animZeroState);
	drawSlashAfter(230, 570);
	debugNumber(250, 570, 2, &curFrame);
	int checkspritetime = spriteTimeBetween * 1000;
	debugNumber(290, 570, 3, &checkspritetime);

	

	//timer debug
	int checktime = sTime->GetTime();
	debugNumber(450,550, 3, &checktime);
	int checkdt = dt * 1000;
	debugNumber(500,550, 4, &checkdt);



}
void CZero::debugNumber(const int x, const int y, const int digits, const int* number){
	GD4N::TVector2<int> digitpos;
	digitpos.x = x;
	digitpos.y = y;
	int digitVal = 1;
	int powdigit = 1;

	int numtoshow = abs(*number);

	for(int dig = 0; dig < digits; dig++) {
		digitVal = (int)(numtoshow / powdigit) % 10;
		digitpos.x = x - (dig * 7);
		UINumbersTexture->SetCurrentFrame(digitVal);
		sVideo->Draw(UINumbersTexture, digitpos);
		powdigit *= 10;
	}
}
void CZero::drawSlashAfter(const int x, const int y) {
	GD4N::TVector2<int> digitpos;
	digitpos.x = x + 6;
	digitpos.y = y;
	UINumbersTexture->SetCurrentFrame(10);
	sVideo->Draw(UINumbersTexture, digitpos);
}
void CZero::debugNumber(const int x, const int y, const int digits, const float* number){
	int debugfloat = *number;
	debugNumber(x, y, digits, &debugfloat);
}
