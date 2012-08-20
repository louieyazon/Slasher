#ifndef _constants_h
#define _constants_h

#define ZEROSPRITEOFFSET_X 88
#define ZEROSPRITEOFFSET_Y 93


const bool SHOW_DEBUG_NUMBERS = true;
const bool SHOW_SPRITE_NOTES = false;

//POINTS SYSTEM and PACING
const float TIME_POINT_RATE	= 0.350f;
const float ASTEROID_DAMAGE = 5.0f;
const float SECOND_ASTEROID_SPAWNDELAY = 60.0f;
const float THIRD_ASTEROID_SPAWNDELAY = 140.0f;
const float FOURTH_ASTEROID_SPAWNDELAY = 360.0f;


//CONTROLS
const SDLKey KEYBIND_LEFT	=	SDLK_LEFT;
const SDLKey KEYBIND_RIGHT	=	SDLK_RIGHT;
const SDLKey KEYBIND_UP		=	SDLK_UP;
const SDLKey KEYBIND_DOWN	=	SDLK_DOWN;
const SDLKey KEYBIND_JUMP	=	SDLK_SPACE;
const SDLKey KEYBIND_DASH	=	SDLK_c;
const SDLKey KEYBIND_ATTACK	=	SDLK_z;

const SDLKey KEYBIND_LEFT2		=	SDLK_KP4;
const SDLKey KEYBIND_RIGHT2		=	SDLK_KP6;
const SDLKey KEYBIND_UP2		=	SDLK_KP8;
const SDLKey KEYBIND_DOWN2		=	SDLK_KP5;
const SDLKey KEYBIND_JUMP2		=	SDLK_END;
const SDLKey KEYBIND_DASH2		=	SDLK_HOME;
const SDLKey KEYBIND_ATTACK2	=	SDLK_DELETE;

//SPLASH LOGO
const float LOGO_DURATION	= 4.00f;
const int LOGOPOS_X			= 100;
const int LOGOPOS_Y			= 80;



//PHYSICS CONSTANTS
//horizontal movement
const float FRIC_X			=	17.0f;		
const float MAX_VX			=	28.0f;		//max speed
const float ZAX				=	40.0f;		//acceleration

//vertical movement
const float GRAVITY_ACC		=	20.0f;		
const float MAX_VY			=	60.0f;		//max speed
const float ZAY				=	40.0f;		//acceleration

const float ZJUMPY			=	1200.0f;	//this seems to be too big on slow computers, i wonder how to fix it.
const float JUMP_FUEL_MAX	=	900;
const float JUMPCONSUMPTION	=	450;

const float ZDASH			=	100.0f;
const float DASH_VX			=	45.0f;
const float DASH_FUEL_MAX	=	260;
const float DASHCONSUMPTION	=	60;

const float STARTING_X		=	150;
const float STARTING_Y		=	-100;

//asteroid constants
const float MAX_AVX			= 45.0f;
const float MIN_AVX			= 30.0f;
const float ASPAWN_X		= 900.0;
const float CUTEFACTORY_X	= 60.0;
const float ASTEROID_RADIUS = 60.0;

//PLATFORMS
const int PLATFORM_WIDTH	=	110;		const int PLATFORM_HEIGHT	=	35;

const float PLATFORM_1X		=	200;		const float PLATFORM_1Y		=	400;
const float PLATFORM_2X		=	400;		const float PLATFORM_2Y		=	300;
const float PLATFORM_3X		=	100;		const float PLATFORM_3Y		=	250;
const float PLATFORM_4X		=	300;		const float PLATFORM_4Y		=	150;

// STAGE BOUNDS
#define LEFTWALL	50.0
#define RIGHTWALL	750.0
#define FLOORLEVEL	490.0

//UI POSITIONS	
const int LIFEBAR_X				= 150;
const int LIFEBAR_Y				= 50;
const int LIFEBAR_SPRITEHEIGHT	= 12;
const int LIFEBAR_MAXSPRITEY	= 500 - LIFEBAR_SPRITEHEIGHT;
const float LIFEBAR_UPDATERATE	= 0.033f;
const int LIFEBAR_MAXWIDTH		= 300;




enum SFX_IDS {
	SFXID_ZJUMP,
	SFXID_ZJUMP2,
	SFXID_ZLAND,
	SFXID_ZSLASH1,
	SFXID_ZSLASH2,
	SFXID_ZSLASH3,
	SFXID_ZSLASHAIR,
	SFXID_ZDASH,
	SFXID_ZSKID,
	SFXID_ZSLASHHIT,
	SFXID_HURT,
	SFXID_READY,
	SFXID_STEP1,
	SFXID_STEP2,
	SFXID_EXPLODE,
};

enum MUSIC_IDS {
	MUSICID_01,
};

enum SURFACE_IDS {
	SURFID_ZERO,
	SURFID_ZEROL,
	SURFID_BKGD,
	SURFID_PLATFORM,
	SURFID_ASTEROID,
	SURFID_EXPLODE,
	
	//UI STUFF
	SURFID_LIFEBAR,
	SURFID_ARROW,
	SURFID_LIFEBARDIFF,
	SURFID_LIFEBARWHITE,
	SURFID_LIFECONTAINER,
	SURFID_ZEROPORTRAIT,
	SURFID_UINUMBERS,
	SURFID_UILARGENUMBERS,
	SURFID_SLASHERLOGO
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
extern bool gameOn;

#endif