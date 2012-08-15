#ifndef _constants_h
#define _constants_h

const float GRAVITY_ACC = 700.00f;
const float MAX_VX = 700;
const float MAX_VY = 700.00f;
const float ZAX = 2000.00f;
const float ZAY = 700.00f;

const float FRIC_X = 700;
const float LEFTWALL = 50;
const float RIGHTWALL = 800;

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