#ifndef _constants_h
#define _constants_h

#define ZEROSPRITEOFFSET_X 88
#define ZEROSPRITEOFFSET_Y 93


//CONTROLS
const SDLKey KEYBIND_LEFT	=	SDLK_LEFT;
const SDLKey KEYBIND_RIGHT	=	SDLK_RIGHT;
const SDLKey KEYBIND_UP		=	SDLK_UP;
const SDLKey KEYBIND_DOWN	=	SDLK_DOWN;
const SDLKey KEYBIND_JUMP	=	SDLK_s;
const SDLKey KEYBIND_DASH	=	SDLK_w;
const SDLKey KEYBIND_ATTACK	=	SDLK_a;

//PHYSICS CONSTANTS

//horizontal movement
const float FRIC_X			=	20.0f;		
const float MAX_VX			=	28.0f;		//max speed
const float ZAX				=	40.0f;		//acceleration

//vertical movement
const float GRAVITY_ACC		=	20.0f;		
const float MAX_VY			=	600.0f;		//max speed
const float ZAY				=	40.0f;		//acceleration

const float ZJUMPY			=	1200.0f;
const float JUMP_FUEL_MAX	=	100;
const float JUMPCONSUMPTION	=	60;

const float ZDASH			=	100.0f;
const float DASH_VX			=	40.0f;
const float DASH_FUEL_MAX	=	260;
const float DASHCONSUMPTION	=	60;

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

const float PLATFORM_1X		=	200;		const float PLATFORM_1Y		=	400;
const float PLATFORM_2X		=	400;		const float PLATFORM_2Y		=	300;
const float PLATFORM_3X		=	100;		const float PLATFORM_3Y		=	200;
const float PLATFORM_4X		=	200;		const float PLATFORM_4Y		=	50;

// STAGE BOUNDS
const float LEFTWALL		=	50;
const float RIGHTWALL		=	750;
const float FLOORLEVEL		=	500;

//UI POSITIONS	
const int LIFEBAR_X				= 150;
const int LIFEBAR_Y				= 510;
const int LIFEBAR_SPRITEHEIGHT	= 12;
const int LIFEBAR_MAXSPRITEY	= 500 - LIFEBAR_SPRITEHEIGHT;
const float LIFEBAR_UPDATERATE	= 0.033f;
const int LIFEBAR_MAXWIDTH		= 300;




enum SFX_IDS {
};

enum MUSIC_IDS {
};

enum SURFACE_IDS {
	SURFID_ZERO,
	SURFID_ZEROL,
	SURFID_BKGD,
	SURFID_PLATFORM,
	SURFID_ASTEROID,
	
	//UI STUFF
	SURFID_LIFEBAR,
	SUFTID_LIFECONTAINER
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
	STATE_DASHING,
	STATE_RUNNING,
};


extern float previousTime;				// time of last update
extern float dt;

#endif