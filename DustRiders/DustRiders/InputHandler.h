#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <list>

// #define XBOX_A 0
// #define XBOX_B 1
// #define XBOX_X 2
// #define XBOX_Y 3
// #define XBOX_LB 4
// #define XBOX_RB 5
// #define XBOX_VIEW 6
// #define XBOX_MENU 7
// #define XBOX_LJS 8
// #define XBOX_RJS 9
// #define XBOX_UP 10
// #define XBOX_RIGHT 11
// #define XBOX_DOWN 12
// #define XBOX_LEFT 13

// #define XBOX_L_XAXIS 0
// #define XBOX_L_YAXIS 1
// #define XBOX_R_XAXIS 2
// #define XBOX_R_YAXIS 3
// #define XBOX_L_TRIGGER 4
// #define XBOX_R_TRIGGER 5

class Xbox
{

public:
	class Button
	{

	public:
		enum Index
		{
			UNKNOWN = -1,
			XBOX_A = 0,
			XBOX_B = 1,
			XBOX_X = 2,
			XBOX_Y = 3,
			XBOX_LB = 4,
			XBOX_RB = 5,
			XBOX_VIEW = 6,
			XBOX_MENU = 7,
			XBOX_LJS = 8,
			XBOX_RJS = 9,
			XBOX_UP = 10,
			XBOX_RIGHT = 11,
			XBOX_DOWN = 12,
			XBOX_LEFT = 13
		};

		int toInt(Index ind) { return static_cast<int>(ind); }
		Index toIndex(int value)
		{
			Index indVal = (value > 13 || value < 0) ? Index::UNKNOWN : static_cast<Index>(value);
			return indVal;
		}
	};

	class Analog
	{
	public:
		enum Index
		{
			UNKNOWN = -1,
			XBOX_L_XAXIS = 0,
			XBOX_L_YAXIS = 1,
			XBOX_R_XAXIS = 2,
			XBOX_R_YAXIS = 3,
			XBOX_L_TRIGGER = 4,
			XBOX_R_TRIGGER = 5,
		};

		int toInt(Index ind) { return static_cast<int>(ind); }
		Index toIndex(int value)
		{
			Index indVal = (value > 5 || value < 0) ? Index::UNKNOWN : static_cast<Index>(value);
			return indVal;
		}
	};
};

class Joystick
{
public:
	Joystick(int jsID) : jsID(jsID), pseudo(false) {}
	Joystick() : jsID(-1), pseudo(true)
	{
		for (int i = 0; i < 14; i++)
		{
			this->buttons[i] = false;
			this->wasPressed[i] = false;
		}
		for (int i = 0; i < 6; i++)
		{
			this->analogs[i] = false;
		}
		analogs[Xbox::Analog::XBOX_L_TRIGGER] = -1.f;
		analogs[Xbox::Analog::XBOX_R_TRIGGER] = -1.f;
	}

	Joystick(const Joystick &js)
	{
		for (int i = 0; i < 14; i++)
		{
			this->buttons[i] = js.buttons[i];
			this->wasPressed[i] = js.wasPressed[i];
		}
		for (int i = 0; i < 6; i++)
		{
			this->analogs[i] = js.analogs[i];
		}
		this->jsID = js.jsID;
		this->pseudo = js.pseudo;
	}

	Joystick &operator=(const Joystick &js)
	{
		if (this != &js)
		{
			for (int i = 0; i < 14; i++)
			{
				this->buttons[i] = js.buttons[i];
				this->wasPressed[i] = js.wasPressed[i];
			}
			for (int i = 0; i < 6; i++)
			{
				this->analogs[i] = js.analogs[i];
			}
			this->jsID = js.jsID;
			this->pseudo = js.pseudo;
		}
		return *this;
	}

	~Joystick() {}

	static std::string buttonToStr(int button);
	static std::string analogToStr(int analog);

	bool setInputs(const unsigned char *buttons, const float *analogs);

	bool getButton(int buttonIndex) { return wasPressed[buttonIndex]; }
	bool getButtonRaw(int buttonIndex) { return buttons[buttonIndex]; }
	float getAnalog(int analogIndex) { return analogs[analogIndex]; }
	int getID() { return jsID; }

	const float *getAnalogs() { return analogs; }

	/**
	 * @brief Get the list of buttons currently pressed. Ignores previous values.
	 *
	 * @return const unsigned char*
	 */
	const unsigned char *getButtonsRaw() { return buttons; };

	/**
	 * @brief Get the list of buttons have been pressed since the list of buttons was last updated.
	 *
	 * @return const unsigned char*
	 */
	const unsigned char *getButtons() { return wasPressed; }

	// For printing button names, for testing
	std::string buttonList();
	std::string triggerList();
	std::string axisList();
	std::string buttonListRaw();

	/**
	 * @brief Update all inputs (buttons and analog axes) for the controller
	 * 				Should only be called oncer per controller per main game loop.
	 *
	 * @return true
	 * @return false
	 */
	bool updateAllInputs();

#pragma region Keyboard_Interface
	void pressW()
	{
		if (pseudo)
			analogs[Xbox::Analog::XBOX_R_TRIGGER] = 0.5f;
	}
	void releaseW()
	{
		if (pseudo)
			analogs[Xbox::Analog::XBOX_R_TRIGGER] = -1.f;
	}
	void pressS()
	{
		if (pseudo)
			analogs[Xbox::Analog::XBOX_L_TRIGGER] = 1.f;
	}
	void releaseS()
	{
		if (pseudo)
			analogs[Xbox::Analog::XBOX_L_TRIGGER] = -1.f;
	}
	void pressA()
	{
		if (pseudo)
			analogs[Xbox::Analog::XBOX_L_XAXIS] = analogs[Xbox::Analog::XBOX_L_XAXIS] - 1.f;
	}
	void releaseA()
	{
		if (pseudo)
			analogs[Xbox::Analog::XBOX_L_XAXIS] = analogs[Xbox::Analog::XBOX_L_XAXIS] + 1.f;
	}
	void pressD()
	{
		if (pseudo)
			analogs[Xbox::Analog::XBOX_L_XAXIS] = analogs[Xbox::Analog::XBOX_L_XAXIS] + 1.f;
	}
	void releaseD()
	{
		if (pseudo)
			analogs[Xbox::Analog::XBOX_L_XAXIS] = analogs[Xbox::Analog::XBOX_L_XAXIS] - 1.f;
	}
	void pressLeftShift()
	{
		if (pseudo)
			buttons[Xbox::Button::XBOX_LB] = 1;
	}
	void releaseLeftShift()
	{
		if (pseudo)
			buttons[Xbox::Button::XBOX_LB] = 0;
	}
	void pressEnter()
	{
		if (pseudo)
		{
			if (!buttons[Xbox::Button::XBOX_RIGHT])
			{
				wasPressed[Xbox::Button::XBOX_RIGHT] = 1;
			}
			else
			{
				wasPressed[Xbox::Button::XBOX_RIGHT] = 0;
			}
			buttons[Xbox::Button::XBOX_RIGHT] = 1;
		}
	}
	void releaseEnter()
	{

		buttons[Xbox::Button::XBOX_RIGHT] = 0;
		wasPressed[Xbox::Button::XBOX_RIGHT] = 0;
	}

	void pressSpace()
	{
		if (pseudo)
		{
			if (!buttons[Xbox::Button::XBOX_A])
			{
				wasPressed[Xbox::Button::XBOX_A] = 1;
			}
			else
			{
				wasPressed[Xbox::Button::XBOX_A] = 0;
			}
			buttons[Xbox::Button::XBOX_A] = 1;
		}
	}
	void releaseSpace()
	{

		buttons[Xbox::Button::XBOX_A] = 0;
		wasPressed[Xbox::Button::XBOX_A] = 0;
	}

	void pressEsc()
	{
		if (pseudo)
		{
			if (!buttons[Xbox::Button::XBOX_MENU])
			{
				wasPressed[Xbox::Button::XBOX_MENU] = 1;
			}
			else
			{
				wasPressed[Xbox::Button::XBOX_MENU] = 0;
			}
			buttons[Xbox::Button::XBOX_MENU] = 1;
		}
	}

	void releaseEsc()
	{
		if (pseudo)
		{
			buttons[Xbox::Button::XBOX_MENU] = 0;
			wasPressed[Xbox::Button::XBOX_MENU] = 0;
		}
	}

	void pressX()
	{
		if (pseudo)
		{
			wasPressed[Xbox::Button::XBOX_X] = 1;
		}
	}

	void releaseX()
	{
		if (pseudo)
		{
			wasPressed[Xbox::Button::XBOX_X] = 0;
			buttons[Xbox::Button::XBOX_X] = 0;
		}
	}

	void pressR()
	{
		if (pseudo)
		{
			if (!buttons[Xbox::Button::XBOX_UP])
			{
				wasPressed[Xbox::Button::XBOX_UP] = 1;
			}
			else
			{
				wasPressed[Xbox::Button::XBOX_UP] = 0;
			}
			buttons[Xbox::Button::XBOX_UP] = 1;
		}
	}

	void releaseR()
	{
		if (pseudo)
		{
			buttons[Xbox::Button::XBOX_UP] = 0;
			wasPressed[Xbox::Button::XBOX_UP] = 0;
		}
	}

	void pressBackspace()
	{
		if (pseudo)
		{
			if (!buttons[Xbox::Button::XBOX_Y])
			{
				wasPressed[Xbox::Button::XBOX_Y] = 1;
			}
			else
			{
				wasPressed[Xbox::Button::XBOX_Y] = 0;
			}
			buttons[Xbox::Button::XBOX_Y] = 1;
		}
	}

	void releaseBackspace()
	{
		if (pseudo)
		{
			buttons[Xbox::Button::XBOX_Y] = 0;
			wasPressed[Xbox::Button::XBOX_Y] = 0;
		}
	}

	bool isPseudo() { return pseudo; }

	void makePseudo()
	{
		pseudo = true;
		jsID = -1;
	}

	void makeReal(int jsID)
	{
		this->jsID = jsID;
		pseudo = false;
	}

#pragma endregion Inputs for converting keyboard values into a pseudo controller

protected:
	unsigned char buttons[14];
	unsigned char wasPressed[14];
	float analogs[6];
	int jsID;
	bool pseudo;
};

class JoystickHandler
{
public:
	JoystickHandler() {}

	static bool addJS(int jsID);
	static bool addJS(Joystick &js);

	static bool removeJS(int jsID);

	static std::map<int, Joystick> getJSMap();
	static void printJSMap();
	static Joystick getJoystick(int jsID);
	static Joystick &getFirstJS() { return jsMap.begin()->second; }

	/**
	 * @brief Update the inputs for all the joysticks in the map.
	 *
	 * @return true
	 * @return false
	 */
	static void updateAll();
	/**
	 * @brief Checks if all expected joysticks are still present.
	 *
	 *
	 * @return true if all expected joysticks are present
	 * @return false if an expected joystick is not present
	 */
	static bool checkPresent();

private:
	static std::map<int, Joystick> jsMap;
};
