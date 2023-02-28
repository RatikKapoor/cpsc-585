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
  case Xbox::Button::XBOX_A:
    return "A";
    break;
  case Xbox::Button::XBOX_B:
    return "B";
    break;
  case Xbox::Button::XBOX_X:
    return "X";
    break;
  case Xbox::Button::XBOX_Y:
    return "Y";
    break;
  case Xbox::Button::XBOX_LB:
    return "LB";
    break;
  case Xbox::Button::XBOX_RB:
    return "RB";
    break;
  case Xbox::Button::XBOX_VIEW:
    return "VIEW";
    break;
  case Xbox::Button::XBOX_MENU:
    return "MENU";
    break;
  case Xbox::Button::XBOX_LJS:
    return "LJS";
    break;
  case Xbox::Button::XBOX_RJS:
    return "RJS";
    break;
  case Xbox::Button::XBOX_UP:
    return "UP";
    break;
  case Xbox::Button::XBOX_RIGHT:
    return "RIGHT";
    break;
  case Xbox::Button::XBOX_DOWN:
    return "DOWN";
    break;
  case Xbox::Button::XBOX_LEFT:
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
  case Xbox::Analog::XBOX_L_XAXIS:
    return "L_XAXIS";
    break;
  case Xbox::Analog::XBOX_L_YAXIS:
    return "L_YAXIS";
    break;
  case Xbox::Analog::XBOX_R_XAXIS:
    return "R_XAXIS";
    break;
  case Xbox::Analog::XBOX_R_YAXIS:
    return "R_YAXIS";
    break;
  case Xbox::Analog::XBOX_L_TRIGGER:
    return "L_TRIGGER";
    break;
  case Xbox::Analog::XBOX_R_TRIGGER:
    return "R_TRIGGER";
    break;
  default:
    return "UKNOWN";
  }
}

std::string Joystick::buttonListRaw()
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

std::string Joystick::buttonList()
{
  std::string buttonList;
  for (int i = 0; i < 14; i++)
  {
    if (wasPressed[i])
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
  axisList.append(std::to_string(analogs[Xbox::Analog::XBOX_L_XAXIS]) + ", " + std::to_string(analogs[Xbox::Analog::XBOX_L_YAXIS]));
  axisList.append(")");
  axisList.append("\n\rRight JS:(");
  axisList.append(std::to_string(analogs[Xbox::Analog::XBOX_R_XAXIS]) + ", " + std::to_string(analogs[Xbox::Analog::XBOX_R_YAXIS]));
  axisList.append(")");
  return axisList;
}

std::string Joystick::triggerList()
{
  std::string triggerList;
  triggerList.append("Left trigger: " + std::to_string(analogs[Xbox::Analog::XBOX_L_TRIGGER]));
  triggerList.append("\n\rRight trigger: " + std::to_string(analogs[Xbox::Analog::XBOX_R_TRIGGER]));
  ;
  return triggerList;
}

bool Joystick::updateAllInputs()
{
  if (pseudo || !glfwJoystickPresent(jsID)) // Check that controller is connected and not a pseudo controller
  {
    return false; // If not connected, return false
  }

  int countButtons;
  const unsigned char *latestButtons = glfwGetJoystickButtons(jsID, &countButtons);

  for (int i = 0; i < countButtons; i++)
  {

    if ((!buttons[i]) && (latestButtons[i]))
    {
      wasPressed[i] = true;
    }
    else
    {
      wasPressed[i] = false;
    }
    buttons[i] = latestButtons[i];
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

bool JoystickHandler::addJS(Joystick &js)
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