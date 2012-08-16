#ifndef _constants_h
#define _constants_h

#define ZEROSPRITEOFFSET_X 88
#define ZEROSPRITEOFFSET_Y 93


//CONTROLS
const SDLKey KEYBIND_LEFT	=	SDLK_LEFT;
const SDLKey KEYBIND_RIGHT	=	SDLK_RIGHT;
const SDLKey KEYBIND_UP		=	SDLK_UP;
const SDLKey KEYBIND_DOWN	=	SDLK_DOWN;
const SDLKey KEYBIND_JUMP	=	SDLK_SPACE;
const SDLKey KEYBIND_ATTACK	=	SDLK_z;

//PHYSICS CONSTANTS

//horizontal movement
const float FRIC_X			=	30.0f;		
const float MAX_VX			=	40.0f;		//max speed
const float ZAX				=	100.0f;		//acceleration

//vertical movement
const float GRAVITY_ACC		=	20.0f;		
const float MAX_VY			=	600.0f;		//max speed
const float ZAY				=	200.0f;		//acceleration
const float ZJUMPY			=	1000.0f;
const float JUMP_FUEL_MAX	=	120;
const float JUMPCONSUMPTION	=	60;

const float STARTING_X		=	150;
const float STARTING_Y		=	150;

//asteroid constants
const float MAX_AVX = 60.0f;
const float MIN_AVX = 30.0f;
const float ASPAWN_X = 800;
const float HITPOS_X = -110;

//PLATFORMS
const int PLATFROM_HEIGHT	=	20;
const int PLATFORM_WIDTH	=	80;

const float PLATFORM_1X		=	200;
const float PLATFORM_1Y		=	400;
const float PLATFORM_2X		=	400;
const float PLATFORM_2Y		=	300;
const float PLATFORM_3X		=	100;
const float PLATFORM_3Y		=	200;
const float PLATFORM_4X		=	200;
const float PLATFORM_4Y		=	50;

// STAGE BOUNDS
const float LEFTWALL		=	50;
const float RIGHTWALL		=	750;
const float FLOORLEVEL		=	500;

enum SFX_IDS {
};

enum MUSIC_IDS {
};

enum SURFACE_IDS {
	SURFID_ZERO,
	SURFID_BKGD,
	SURFID_PLATFORM,
	SURFID_ASTEROID,
};

enum direction_t {
	DIR_UP = 0,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

enum GAMEOBJECT_TYPES {
	TYPE_ZERO = 0,
	TYPE_PLATFORM,
	TYPE_DESTRUCTABLE,
	TYPE_GROUND,
};


enum ZERO_STATES {
	STATE_STANDING,
	STATE_JUMPING,
	STATE_RUNNING,
};


extern float previousTime;				// time of last update
extern float dt;

#endif