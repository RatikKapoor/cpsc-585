#include "InputHandler.h"
#include <string>
#include <map>

std::map<int, Joystick> JSHandler::jsMap;

std::string Joystick::buttonToStr(int button)
{
  switch (button)
  {
  case XBOX_A:
    return "A";
    break;
  case XBOX_B:
    return "B";
    break;
  case XBOX_X:
    return "X";
    break;
  case XBOX_Y:
    return "Y";
    break;
  case XBOX_LB:
    return "LB";
    break;
  case XBOX_RB:
    return "RB";
    break;
  case XBOX_VIEW:
    return "VIEW";
    break;
  case XBOX_MENU:
    return "MENU";
    break;
  case XBOX_LJS:
    return "LJS";
    break;
  case XBOX_RJS:
    return "RJS";
    break;
  case XBOX_UP:
    return "UP";
    break;
  case XBOX_RIGHT:
    return "RIGHT";
    break;
  case XBOX_DOWN:
    return "DOWN";
    break;
  case XBOX_LEFT:
    return "LEFT";
    break;
  default:
    return "UNKNOWN";
  }
}

std::string Joystick::analogToStr(int analog)
{
  switch (analog)
  {
  case XBOX_L_XAXIS:
    return "L_XAXIS";
    break;
  case XBOX_L_YAXIS:
    return "L_YAXIS";
    break;
  case XBOX_R_XAXIS:
    return "R_XAXIS";
    break;
  case XBOX_R_YAXIS:
    return "R_YAXIS";
    break;
  case XBOX_L_TRIGGER:
    return "L_TRIGGER";
    break;
  case XBOX_R_TRIGGER:
    return "R_TRIGGER";
    break;
  default:
    return "UKNOWN";
  }
}

std::string Joystick::buttonList()
{
  std::string buttonList;
  for (int i = 0; i < 14; i++)
  {
    if (buttons[i])
    {
      buttonList.append(Joystick::buttonToStr(i));
      buttonList.append(", ");
    }
  }
  if (buttonList.size() == 0)
  {
    buttonList = "NONE";
  }
  else
  {
    buttonList.resize(buttonList.size() - 2);
  }
  return buttonList;
}

std::string Joystick::axisList()
{
  std::string axisList;
  axisList.append("Left JS: (");
  axisList.append(std::to_string(analogs[XBOX_L_XAXIS]) + ", " + std::to_string(analogs[XBOX_L_YAXIS]));
  axisList.append(")");
  axisList.append("\n\rRight JS:(");
  axisList.append(std::to_string(analogs[XBOX_R_XAXIS]) + ", " + std::to_string(analogs[XBOX_R_YAXIS]));
  axisList.append(")");
  return axisList;
}

std::string Joystick::triggerList()
{
  std::string triggerList;
  triggerList.append("Left trigger: " + std::to_string(analogs[XBOX_L_TRIGGER]));
  triggerList.append("\n\rRight trigger: " + std::to_string(analogs[XBOX_R_TRIGGER]));
  ;
  return triggerList;
}

bool Joystick::setButtons(const unsigned char *buttons)
{
  int i;
  for (i = 0; i < 14 && buttons + i != nullptr; i++)
  {
    this->buttons[i] = buttons[i];
  }

  if (i == 14)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Joystick::updateButtons(int jsID)
{
  if (!glfwJoystickPresent(jsID))
  {
    return false;
  }

  int count;
  return setButtons(glfwGetJoystickButtons(jsID, &count));
}

bool Joystick::setAnalogs(const float *analogs)
{
  int i;
  for (i = 0; i < 6 && analogs + i != nullptr; i++)
  {
    this->analogs[i] = analogs[i];
  }

  if (i == 6)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Joystick::updateAnalogs(int jsID)
{
  if (!glfwJoystickPresent(jsID))
  {
    return false;
  }

  int count;
  return setAnalogs(glfwGetJoystickAxes(jsID, &count));
}

bool Joystick::setInputs(const unsigned char *buttons, const float *analogs)
{
  return (setButtons(buttons) && setAnalogs(analogs));
}

bool Joystick::setInputs(int jsID)
{
  if (!glfwJoystickPresent(jsID))
  {
    return false;
  }

  int countB;
  int countA;
  return setInputs(glfwGetJoystickButtons(jsID, &countB), glfwGetJoystickAxes(jsID, &countA));
}

bool Joystick::getButton(int button)
{
  return buttons[button];
}

bool Joystick::getAnalog(int analog)
{
  return analogs[analog];
}
