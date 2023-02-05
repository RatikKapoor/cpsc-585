#include "InputHandler.h"
#include <string>

std::vector<int> Joystick::jsPresent;

int Joystick::add(int jsID)
{
  if (Joystick::jsPresent.size() >= 16)
  {
    return -1;
  }
  if (searchJS(jsID) == -1)
  {
    Joystick::jsPresent.push_back(jsID);
    return 0;
  }
  else
  {
    return 1;
  }
}

int Joystick::remove(int jsID)
{
  if (Joystick::jsPresent.empty())
  {
    return -1;
  }
  int searchInd = searchJS(jsID);
  if (searchInd == -1)
  {
    return 1;
  }
  else
  {
    Joystick::jsPresent.erase(Joystick::jsPresent.begin() + searchInd);
    return 0;
  }
}

int Joystick::status(int jsID)
{
  return (searchJS(jsID) >= 0);
}

int Joystick::removeAll()
{
  Joystick::jsPresent.clear();
  return (Joystick::jsPresent.empty());
}

int Joystick::searchJS(int jsID)
{
  // We will have a max of 16 controllers, so this should be fine for performance
  if (Joystick::jsPresent.empty())
  {
    return -1;
  }
  int index = 0;
  for (auto js : Joystick::jsPresent)
  {
    if (js == jsID)
    {
      return index;
    }
    index++;
  }
  return -1;
}

std::vector<int> Joystick::getAll() const
{
  std::vector<int> jsList = Joystick::jsPresent;
  return jsList;
}

std::string Joystick::getButtonsString(int jsID)
{
  // No butt
  std::string buttonString = "JStick ";
  buttonString.append(std::to_string(jsID));
  if (!glfwJoystickPresent(jsID))
  {
    buttonString.append(" not present.");
    return buttonString;
  }

  int count;
  const unsigned char *buttons = glfwGetJoystickButtons(jsID, &count);
  buttonString.append(" buttons: ");
  buttonString.append(std::to_string(count));
  for (int i = 0; i < count; i++)
  {

    // buttonString.append(std::to_string(i));
    // buttonString.append("(");
    // buttonString.append(std::to_string(buttons[i]));
    // buttonString.append(")");
    if (buttons[i])
    {
      buttonString.append(getButtonName(static_cast<XboxButton>(i)));
      buttonString.append(", ");
    }
  }
  buttonString.resize(buttonString.size() - 2);
  return buttonString;
}