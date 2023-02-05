#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <map>
#include <memory>

#define XBOX_A 0
#define XBOX_B 1
#define XBOX_X 2
#define XBOX_Y 3
#define XBOX_LB 4
#define XBOX_RB 5
#define XBOX_VIEW 6
#define XBOX_MENU 7
#define XBOX_LJS 8
#define XBOX_RJS 9
#define XBOX_UP 10
#define XBOX_RIGHT 11
#define XBOX_DOWN 12
#define XBOX_LEFT 13

#define XBOX_L_XAXIS 0
#define XBOX_L_YAXIS 1
#define XBOX_R_XAXIS 2
#define XBOX_R_YAXIS 3
#define XBOX_L_TRIGGER 4
#define XBOX_R_TRIGGER 5

class Joystick
{
public:
	Joystick() {}
	Joystick(const Joystick &js)
	{
		for (int i = 0; i < 14; i++)
		{
			this->buttons[i] = js.buttons[i];
		}
		for (int i = 0; i < 6; i++)
		{
			this->analogs[i] = js.analogs[i];
		}
	}

	Joystick &operator=(const Joystick &js)
	{
		if (this != &js)
		{
			for (int i = 0; i < 14; i++)
			{
				this->buttons[i] = js.buttons[i];
			}
			for (int i = 0; i < 6; i++)
			{
				this->analogs[i] = js.analogs[i];
			}
		}
		return *this;
	}

	~Joystick() {}

	static std::string buttonToStr(int button);
	static std::string analogToStr(int analog);

	bool setButtons(const unsigned char *buttons);
	bool updateButtons(int jsID);

	bool setAnalogs(const float *analogs);
	bool updateAnalogs(int jsID);

	bool setInputs(const unsigned char *buttons, const float *analogs);
	bool setInputs(int jsID);

	bool getButton(int buttonIndex);
	bool getAnalog(int analogIndex);

	std::string buttonList();
	std::string triggerList();
	std::string axisList();

private:
	unsigned char buttons[14];
	float analogs[6];
};

class JSHandler
{
public:
	// Adds one controller by default
	JSHandler()
	{
		if (jsMap.size() == 0 && glfwJoystickPresent(GLFW_JOYSTICK_1))
		{
			JSHandler::jsMap[GLFW_JOYSTICK_1] = Joystick();
		}
	}

private:
	static std::map<int, Joystick> jsMap;
};
