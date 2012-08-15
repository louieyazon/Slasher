#ifndef _constants_h
#define _constants_h


//CONTROLS
const SDLKey KEYBIND_LEFT	= SDLK_LEFT;
const SDLKey KEYBIND_RIGHT	= SDLK_RIGHT;
const SDLKey KEYBIND_UP		= SDLK_UP;
const SDLKey KEYBIND_DOWN	= SDLK_DOWN;
const SDLKey KEYBIND_JUMP	= SDLK_SPACE;

//PHYSICS CONSTANTS
const float GRAVITY_ACC		= 200.0f;
const float MAX_VX			= 45.0f;
const float MAX_VY			= 300.0f;
const float ZAX				= 100.0f;
const float ZAY				= 200.0f;
const float FRIC_X			= 30.0f;

const float STARTING_X		= 150;
const float STARTING_Y		= 150;
const float JUMP_FUEL_MAX	= 50;
const float JUMPCONSUMPTION	= 100;

const float LEFTWALL		= 50;
const float RIGHTWALL		= 750;
const float FLOORLEVEL		= 500;


enum SFX_IDS {
};

enum MUSIC_IDS {
};

enum SURFACE_IDS {
	SURFID_ZERO,
	SURFID_BKGD,
	SURFID_PLATFORM,
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

enum ZERO_ANIMSTATES {
	AS_STANDING,
	AS_STARTRUN,
	AS_RUNNING,
	AS_JUMPING
};


#endif