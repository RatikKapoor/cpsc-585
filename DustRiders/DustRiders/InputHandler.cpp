#include "InputHandler.h"
#include <string>
#include <map>
#include <list>
#include <iostream>

std::map<int, Joystick> JoystickHandler::jsMap;

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

bool Joystick::updateInputs(int jsID)
{
  if (!glfwJoystickPresent(jsID))
  {
    return false;
  }

  int countB;
  int countA;
  return setInputs(glfwGetJoystickButtons(jsID, &countB), glfwGetJoystickAxes(jsID, &countA));
}

bool Joystick::updateAllInputs()
{
  if (pseudo || !glfwJoystickPresent(jsID)) // Check that controller is connected and not a pseudo controller
  {
    return false; // If not connected, return false
  }

  int countButtons;
  const unsigned char *tempButtons = glfwGetJoystickButtons(jsID, &countButtons);
  for (int i = 0; i < countButtons; i++)
  {
    buttons[i] = tempButtons[i];
  }

  int countAnalogs;
  const float *tempAnalogs = glfwGetJoystickAxes(jsID, &countAnalogs);
  for (int i = 0; i < countAnalogs; i++)
  {
    analogs[i] = tempAnalogs[i];
  }

  return true;
}

bool JoystickHandler::addJS(int jsID)
{
  if (glfwJoystickPresent(jsID))
  {
    auto jsIter = JoystickHandler::jsMap.find(jsID);
    if (jsIter == JoystickHandler::jsMap.end() || JoystickHandler::jsMap.size() == 0)
    {
      JoystickHandler::jsMap[jsID] = Joystick(jsID);
    }
    return true;
  }
  else
  {
    return false;
  }
}



bool JoystickHandler::addJS(Joystick& js)
{
    int jsID = js.getID();
    auto jsIter = JoystickHandler::jsMap.find(jsID);
    if (jsIter == JoystickHandler::jsMap.end() || JoystickHandler::jsMap.size() == 0)
    {
        JoystickHandler::jsMap[jsID] = js;
        return true;
    }
    else
    {
        return false;
    }
}

bool JoystickHandler::removeJS(int jsID)
{

  JoystickHandler::jsMap.erase(jsID);
  auto jsIter = JoystickHandler::jsMap.find(jsID);

  if (jsIter == JoystickHandler::jsMap.end() || JoystickHandler::jsMap.size() == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

std::map<int, Joystick> JoystickHandler::getJSMap()
{
  std::map<int, Joystick> varMap = JoystickHandler::jsMap;
  return varMap;
}

void JoystickHandler::printJSMap()
{
  std::cout << "Joysticks: " << std::endl;
  for (auto js : JoystickHandler::jsMap)
  {
    std::cout << js.first << std::endl;
  }
  std::cout << "LIST END" << std::endl;
}

void JoystickHandler::updateAll()
{
  auto jsIter = jsMap.begin();
  while (jsIter != jsMap.end())
  {
    jsIter->second.updateAllInputs();
    jsIter++;
  }
  return;
}

bool JoystickHandler::checkPresent()
{
  if (jsMap.size() == 0)
  {
    return true;
  }

  auto jsIter = jsMap.begin();

  while (jsIter != jsMap.end())
  {
    if (!glfwJoystickPresent(jsIter->first))
    {
      return false;
    }
    jsIter++;
  }
  return true;
}

Joystick JoystickHandler::getJoystick(int jsID)
{
  auto jsIter = jsMap.find(jsID);
  if (jsIter == jsMap.end())
  {
    return Joystick();
  }
  else
  {
    return jsIter->second;
  }
}