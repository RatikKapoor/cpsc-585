#pragma once


enum class XboxButton
{
	XBOX_A = 0,
	XBOX_B = 1,
	XBOX_X = 2,
	XBOX_Y = 3,
	XBOX_LB = 4,
	XBOX_RB = 5,
	XBOX_VIEW = 6,
	XBOX_MENU = 7,
	XBOX_MAIN = 8, // The 'X' button on the center of the controller
	XBOX_JSLB = 9,
	XBOX_JSRB = 10,
	XBOX_DUP = 11,
	XBOX_DRIGHT = 12,
	XBOX_DDOWN = 13,
	XBOX_DLEFT = 14
};

class Joystick {
public:
	Joystick(int jsID) :jsID(jsID);
	Joystick(const Joystick& js) {
		this->jsID = js.jsID;
		return;
	}

	Joystick

private:
	int jsID; 
};