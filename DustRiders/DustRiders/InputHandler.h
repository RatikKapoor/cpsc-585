#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>

// enum class XboxButton
// {
// 	XBOX_A = 0,
// 	XBOX_B = 1,
// 	XBOX_X = 2,
// 	XBOX_Y = 3,
// 	XBOX_LB = 4,
// 	XBOX_RB = 5,
// 	XBOX_VIEW = 6,
// 	XBOX_MENU = 7,
// 	XBOX_MAIN = 8,
// 	XBOX_JSRB = 9,	 // NEW JSLB
// 	XBOX_DUP = 10,	 // NEW JSRB
// 	XBOX_DDOWN = 11, // NEW DRIGHT
// 	XBOX_DLEFT = 14, //
// 	XBOX_JSLB = 12,
// 	XBOX_DRIGHT = 13, // NEW DUP
// };

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
	XBOX_JSLB = 8,
	XBOX_JSRB = 9,
	XBOX_DUP = 10,
	XBOX_DRIGHT = 11,
	XBOX_DDOWN = 12,
	XBOX_DLEFT = 13
};

constexpr const char *getButtonName(XboxButton button)
{
	switch (button)
	{
	case XboxButton::XBOX_A:
		return "A";
	case XboxButton::XBOX_B:
		return "B";
	case XboxButton::XBOX_X:
		return "X";
	case XboxButton::XBOX_Y:
		return "Y";
	case XboxButton::XBOX_LB:
		return "LB";
	case XboxButton::XBOX_RB:
		return "RB";
	case XboxButton::XBOX_VIEW:
		return "VW";
	case XboxButton::XBOX_MENU:
		return "MU";
	case XboxButton::XBOX_JSLB:
		return "JSL";
	case XboxButton::XBOX_JSRB:
		return "JSR";
	case XboxButton::XBOX_DUP:
		return "DUP";
	case XboxButton::XBOX_DRIGHT:
		return "DRT";
	case XboxButton::XBOX_DDOWN:
		return "DDN";
	case XboxButton::XBOX_DLEFT:
		return "DLT";
	default:
		return "Unknown button";
	}
}

class Joystick
{

public:
	Joystick() {}

	int add(int jsID);
	int status(int jsID);
	int remove(int jsID);
	int removeAll();
	std::vector<int> getAll() const;
	static std::string getButtonsString(int jsID);

private:
	static std::vector<int> jsPresent;
	int searchJS(int jsID);
};
