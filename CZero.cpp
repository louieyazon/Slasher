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
#include "CAsteroid.h"
#include <cmath>

CZero::CZero(CSlasherGameManager* gameManager) : CGameObject() {
	
	highScorePtr = &gameManager->highScore;
	gameManagerPtr = gameManager;

	hitpoints		= MAX_HP;
	points			= 0;
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
	isDropping		= false;

	dashslashing	= false;
	attacknum		= 0;
	previousTime	= SDL_GetTicks() * 0.01f;

	zeroState		= STATE_STANDING;
	animZeroState	= AS_STANDING;

	//animation
	curFrame		= 0;
	drawFrame		= 0;
	spriteTimeBetween = 1.0000/60.0000;
	spriteTimeLast = sTime->GetTime();

	lastKillTime = 0;
	comboCount = 0;

	timeSinceLastPoint = sTime->GetTime();


	UINumbersTexture = new GD4N::CSurfaceSheet(SURFID_UINUMBERS);
	UINumbersTexture->SetSpriteDimensions(1,12);

	zeroTexture = new GD4N::CSurfaceSheet(SURFID_ZERO);
	zeroTexture->SetSpriteDimensions(14,10);

	zeroTextureL = new GD4N::CSurfaceSheet(SURFID_ZEROL);
	zeroTextureL->SetSpriteDimensions(14,10);

	zeroTexture->SetCurrentFrame(drawFrame);

	sAudio->PlaySound(SFXID_READY);
	sAudio->SetAndPlayMusic(MUSICID_01);
}
CZero::~CZero() {
	delete zeroTexture;
	delete zeroTextureL;
	delete UINumbersTexture;
	zeroTexture = 0;
	zeroTextureL = 0;
}

void CZero::Update() {
	/*if(logicTimeLast + logicTimeBetween < sTime->GetTime()) {
		logicTimeLast = sTime->GetTime();
	}*/
	earnTimePoints();
	ReactToInput();
	Physics();
	readState();
	Animate();
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

	if((!dashing && !dashslashing) || (falling && dashing)) {
		if(attacknum == 0 || falling) {
			if(sInput->GetKey(KEYBIND_RIGHT) || sInput->GetKey(KEYBIND_RIGHT2))			{		accelerate_right();		}
			if(sInput->GetKey(KEYBIND_LEFT) || sInput->GetKey(KEYBIND_LEFT2))			{		accelerate_left();		}
		}
	}

	if(sInput->GetKey(KEYBIND_JUMP) || sInput->GetKey(KEYBIND_JUMP2))					{		jump();					}
	else if(sInput->GetKeyUp(KEYBIND_JUMP) || sInput->GetKeyUp(KEYBIND_JUMP2))			{		breakjump();			}
	
	if(!falling && !dashslashing) {																		//ground-only controls
		if(sInput->GetKey(KEYBIND_DASH) || sInput->GetKey(KEYBIND_DASH2))				{		dash();					}
	}
	if(sInput->GetKeyUp(KEYBIND_DASH) || sInput->GetKeyUp(KEYBIND_DASH2))				{		undash();				}
	if(sInput->GetKey(KEYBIND_ATTACK) || sInput->GetKey(KEYBIND_ATTACK2))				{		attack();				}
	
	//prevent hopping (by holding jump key)
	if(!sInput->GetKey(KEYBIND_JUMP) && !sInput->GetKey(KEYBIND_JUMP2) && !falling)	{		reenableJump();			}	//re-enable jumping only when jump key is released and player has landed on the ground
	if(!sInput->GetKey(KEYBIND_DASH) && !sInput->GetKey(KEYBIND_DASH2) && !dashing)	{		reenableDash();			}	//re-enable jumping only when jump key is released and player has landed on the ground
	
	if(sInput->GetKeyDown(KEYBIND_DOWN) && position.y != FLOORLEVEL) { isDropping = true; }
	if(sInput->GetKeyUp(KEYBIND_DOWN)) { isDropping = false; }
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
		if(nvx == 0)			{	zeroState = STATE_STANDING; dashslashing = false;}		// not moving
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

	lastZeroState = zeroState;
	// DEBUG DEBUG DEBUG
	if (hitpoints <= 0){ 
	gameManagerPtr->setHighScore(points);
	sGameManager->ChangeScene(0);}
}

// POINTS
void CZero::earnTimePoints() {
	if(sTime->GetTime() - timeSinceLastPoint >= TIME_POINT_RATE) {
		timeSinceLastPoint = sTime->GetTime();
		points++;
	}
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
		isDropping = false;
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
	attacknum = 0;
	falling = false;
	dashing = false;
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
	if(!falling && attacknum == 0){		//ground animations
		if(zeroState == STATE_STANDING) {	// vx = 0
			if(animZeroState == AS_RUNNING) animZeroState = AS_STOPRUN;
		}

		if(zeroState == STATE_RUNNING && animZeroState != AS_BREAKING && animZeroState != AS_DSLASHING && !dashing) {	// vx != 0
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
	if(animZeroState != AS_RUNNING) attacknum = 0;
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
		if(!dashing && !dashslashing) {						//jump from ground
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
	if(!falling) {
		if(	attacknum == 0 &&
			(animZeroState == AS_STANDING ||		//these are the conditions when zero should be allowed to do a primary slash
			animZeroState == AS_KEEPSABER ||
			animZeroState == AS_LANDING ||
			animZeroState == AS_RUNNING ||
			animZeroState == AS_STARTRUN ||
			animZeroState == AS_STOPRUN)
		)									{	slash(1);	}	//slash 1
		if (attacknum == 1 && curFrame > 5)	{	slash(2);	}	//slash 2
		if (attacknum == 2 && curFrame > 5)	{	slash(3);	}	//slash 3

		if(dashing && !dashslashing)		{	slash(17);	}	//dashing slash (code 17)
	}

	if (falling && attacknum == 0)			{	slash(13);	}		// air slash (code 13)
	
}

void CZero::slash(int slashnum){
	switch(slashnum) {
		case 0: break;
		case 1: animZeroState = AS_SLASH1; sAudio->PlaySound(SFXID_ZSLASH1); break;
		case 2: animZeroState = AS_SLASH2; sAudio->PlaySound(SFXID_ZSLASH2); break;
		case 3: animZeroState = AS_SLASH3; sAudio->PlaySound(SFXID_ZSLASH3); break;
		case 13: animZeroState = AS_AIRSLASH; sAudio->PlaySound(SFXID_ZSLASHAIR); break;
		case 17:	animZeroState = AS_DSLASHING;
					sAudio->PlaySound(SFXID_ZSLASHAIR);
					dashing = false;
					dashslashing = true;
					undash();
					dashccelerate();
					break;
	}
	
	attacknum = slashnum;
	curFrame = 0;
	spriteTimeLast = sTime->GetTime();
}

//COLLISIONS
bool CZero::IsCollidingWith(GD4N::CGameObject* other){
	switch (other->GetType()) {
		case TYPE_PLATFORM:
			{
				CPlatform* platform = dynamic_cast<CPlatform*>(other);

				int diffpos_x = position.x - platform->position.x;						// + if position.value is greater
				int diffpos_y = position.y - platform->position.y;						// - if position.value is less
				int diffpos_yUnder = (position.y + 2) - platform->position.y;

				bool isYCollide			=	abs(diffpos_y) < platform->GetHeight()/2;		// y within platform rectangle
				bool isXCollide			=	abs(diffpos_x) < platform->GetWidth()/2;		// within width of platform
				bool isYCollideUnder	=	abs(diffpos_yUnder) < platform->GetHeight()/2;
				
				if(vy > 0	&&	isYCollide	&& isXCollide && !isDropping) {	// going down & position.xy is within platform rectangle
					return true;
				} else if ((isYCollideUnder && !isXCollide) || isDropping) {	// position.x not within platform width & position.y is right above platform rectangle.
					falling = true;
					return false;
				}
			}
			break;
		case TYPE_ASTEROID:
			CAsteroid* asteroid = dynamic_cast<CAsteroid*>(other);
			if(!asteroid->exploded){
				return attackCheck(attacknum, other);
			}
			break;			
	};
	return false;
}
void CZero::CollidesWith(GD4N::CGameObject* other){
	switch (other->GetType()) {
		case TYPE_PLATFORM:
			{
				CPlatform* platform = dynamic_cast<CPlatform*>(other);
				position.y = platform->position.y;
				land();
			}
			break;
		case TYPE_ASTEROID:
			CAsteroid* asteroid = dynamic_cast<CAsteroid*>(other);
			asteroid->explode();
			
			if(sTime->GetTime() - lastKillTime <= COMBO_WINDOW){
				comboCount++;
				if(comboCount == 50) hitpoints = MAX_HP;
			} else {
				comboCount = 1;
			}
			
			lastKillTime = sTime->GetTime();
			points = points + KILLSCORE*((comboCount/5) + 1);
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
	
	debugNumber(200, 570, 2, &attacknum);

	debugNumber(700, 10, 3, &comboCount);
	debugNumber(700, 570, 10, highScorePtr);

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

bool CZero::attackCheck(int attacknum, GD4N::CGameObject* other) {
	CAsteroid* asteroid = dynamic_cast<CAsteroid*>(other);

	slashCircle thisSlash;
	switch(attacknum) {
		case 0: return false;
		case 1: thisSlash = slashCircles[SLASH_GROUND1];	break;
		case 2: thisSlash = slashCircles[SLASH_GROUND2];	break;
		case 3: thisSlash = slashCircles[SLASH_GROUND3];	break;
		case 13: thisSlash = slashCircles[SLASH_AIR];	break;
		case 17: thisSlash = slashCircles[SLASH_DASH];	break;
	};
	
	float slashX = position.x + thisSlash.x_offset;
	float slashY = position.y + thisSlash.y_offset;
	float slashRadius = thisSlash.radius;
	
	if(!facingRight){
	slashX = position.x - thisSlash.x_offset * 2;
	}

	bool isAHit = AreCirclesIntersecting(slashX, slashY, slashRadius, asteroid->position.x, asteroid->position.y, asteroid->radius);
	if(attacknum == 2){
	}

	return isAHit;
}

bool CZero::AreCirclesIntersecting(float posAX, float posAY, float radiusA, float posBX, float posBY, float radiusB) {
	float distanceSq = (posAX - posBX) * (posAX - posBX) + (posAY - posBY) * (posAY - posBY);
	return ( (radiusA + radiusB) * (radiusA + radiusB) > distanceSq );
}