#include <string>

#include "StateHandler.h"
#include "InputHandler.h"

void StateHandler::processJS(Joystick &js)
{
  const unsigned char *pressed = js.getButtons();

  if (gState == GameState::Exit)
  {
    return;
  }

  if (gState == GameState::Playing || gState == GameState::NotStarted)
  {
    if (pressed[Xbox::Button::XBOX_MENU])
    {
      setGState(GameState::PauseMenu);
      if (js.isPseudo()) js.releaseEsc();
      return;
    }
  }
  else if (gState == GameState::PauseMenu)
  {
    if (pressed[Xbox::Button::XBOX_MENU])
    {
      setGState(prevGState);
      if (js.isPseudo()) js.releaseEsc();
      return;
    }
    if (pressed[Xbox::Button::XBOX_X])
    {
      setGState(GameState::Exit);
      return;
    }
  }
  // To be used when the game first starts, with the overlay menu
  else if (gState == GameState::StartMenu)
  {
    if (pressed[Xbox::Button::XBOX_X])
    {
      setGState(GameState::Exit);
      return;
    }
    else if (pressed[Xbox::Button::XBOX_A])
    {
      setGState(GameState::Playing);
      if (js.isPseudo()) js.releaseEnter();

      return;
    }
  }
}

StateHandler::GameState StateHandler::getGState()
{
  return gState;
}

StateHandler::GameState StateHandler::getPrevGState()
{
  return prevGState;
}

void StateHandler::setGState(GameState gState)
{
  this->prevGState = this->gState;
  this->gState = gState;
}

StateHandler::GameState::operator std::string()
{
  std::string stateString = "Unknown";
  switch (this->state)
  {
  case Exit:
    stateString = "Exit";
    break;
  case NotStarted:
    stateString = "Not started";
    break;
  case Playing:
    stateString = "Playing";
    break;
  case StartMenu:
    stateString = "Start Menu";
    break;
  case PauseMenu:
    stateString = "Pause Menu";
    break;
  default:
    stateString = "Unknown";
  }
  return stateString;
}