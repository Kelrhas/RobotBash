#ifndef WIN32_MAIN
#define WIN32_MAIN

#include "Utils/Debug.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_OFFSET_X 20
#define WINDOW_OFFSET_Y 20
/** /
// NOTE load de fonction externes sans avoir link la lib, avec des fonctions dummy
// XInputGetState
#ifndef X_INPUT_GET_STATE
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateDummy)
{
return 0;
}
static x_input_get_state *XInputGetState_ = XInputGetStateDummy;
#define XInputGetState XInputGetState_
#endif

// XInputSetState
#ifndef X_INPUT_SET_STATE
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateDummy)
{
return 0;
}
static x_input_set_state *XInputSetState_ = XInputSetStateDummy;
#define XInputSetState XInputSetState_
#endif
/**/
class Game;

typedef struct Color
{
	union
	{
		struct // beware of endianness
		{
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t a;
		};
		uint32_t argb = 0;
	};

	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}


	Color(uint32_t col = 0)
	{
		this->argb = col;
	}

	Color& operator=(const uint32_t &col)
	{
		this->argb = col;
		return *this;
	}
}Color;

typedef struct GameButtonState
{
	bool isDown = false;
	bool wasDown = false;
}GameButtonState;

typedef struct GameControllerInput
{
	bool isConnected = false;
	float verticalLeft = 0;
	float horizontalLeft = 0;
	float verticalRight = 0;
	float horizontalRight = 0;
	float triggerLeft = 0;
	float triggerRight = 0;

	GameButtonState a; // croix		speed
	GameButtonState b; // rond
	GameButtonState x; // carré		shoot
	GameButtonState y; // triangle
	GameButtonState leftShoulder;
	GameButtonState rightShoulder;
}GameControllerInput;

typedef struct GameInput
{
	GameControllerInput controllers[4];
	GameControllerInput keyboard;
}GameInput;

#endif