#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <list>

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
	Joystick(int jsID) : jsID(jsID), pseudo(false) {}
	Joystick() : jsID(-1), pseudo(true) {
		for (int i = 0; i < 14; i++)
		{
			this->buttons[i] = false;
		}
		for (int i = 0; i < 6; i++)
		{
			this->analogs[i] = false;
		}
		analogs[XBOX_L_TRIGGER] = -1.f;
		analogs[XBOX_R_TRIGGER] = -1.f;
	}

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

	bool setButtons(const unsigned char *buttons);

	bool setAnalogs(const float *analogs);

	bool setInputs(const unsigned char *buttons, const float *analogs);

	bool updateButtons() {return ( (!pseudo) ? (updateButtons(this->jsID)) :  false); }
	bool updateAnalogs() { return ((!pseudo) ? (updateAnalogs(this->jsID)) : false); }
	bool updateInputs() { return ((!pseudo) ? (updateInputs(this->jsID)) : false); }

	bool getButton(int buttonIndex) { return buttons[buttonIndex]; }
	float getAnalog(int analogIndex) { return analogs[analogIndex]; }
	int getID() { return jsID; }

	const float *getAnalogs() { return analogs; }
	const unsigned char *getButtons() { return buttons; };

	// For printing button names, for testing
	std::string buttonList();
	std::string triggerList();
	std::string axisList();

	/**
	 * @brief Update all inputs (buttons and analog axes) for the controller
	 * 				Should only be called oncer per controller per main game loop.
	 *
	 * @return true
	 * @return false
	 */
	bool updateAllInputs();


	void pressW() { if(pseudo) analogs[XBOX_R_TRIGGER] = 1.f; }
	void releaseW() { if (pseudo) analogs[XBOX_R_TRIGGER] = -1.f; }
	void pressS() { if (pseudo)  analogs[XBOX_L_TRIGGER] = 1.f; }
	void releaseS() { if (pseudo) analogs[XBOX_L_TRIGGER] = -1.f; }
	void pressA() { if (pseudo)  analogs[XBOX_L_XAXIS] = analogs[XBOX_L_XAXIS] - 1.f; }
	void releaseA() { if (pseudo) analogs[XBOX_L_XAXIS] = analogs[XBOX_L_XAXIS] + 1.f; }
	void pressD() { if (pseudo)  analogs[XBOX_L_XAXIS] = analogs[XBOX_L_XAXIS] + 1.f; }
	void releaseD() { if (pseudo)  analogs[XBOX_L_XAXIS] = analogs[XBOX_L_XAXIS] - 1.f; }
	void pressLeftShift() { if (pseudo)  buttons[XBOX_LB] = 1; }
	void releaseLeftShift() { if (pseudo)  buttons[XBOX_LB] = 0; }
	void pressEnter() { if (pseudo) buttons[XBOX_A] = 1; }
	void releaseEnter() { if (pseudo) buttons[XBOX_A] = 0; }


	bool isPseudo() { return pseudo; }

	void makePseudo() { 
		pseudo = true; 
	jsID = -1; }

	void makeReal(int jsID) { this->jsID = jsID; pseudo = false; }

protected:
	unsigned char buttons[14];
	float analogs[6];
	int jsID;
	bool pseudo;

	// for Testing
	bool updateButtons(int jsID);
	bool updateAnalogs(int jsID);
	bool updateInputs(int jsID);
};


class JoystickHandler
{
public:
	JoystickHandler() {}

	static bool addJS(int jsID);
	static bool addJS(Joystick& js);


	static bool removeJS(int jsID);

	static std::map<int, Joystick> getJSMap();
	static void printJSMap();
	static Joystick getJoystick(int jsID);
	static Joystick& getFirstJS() { return jsMap.begin()->second; }

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


