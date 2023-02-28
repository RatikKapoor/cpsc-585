#pragma once

#include <string>
#include "InputHandler.h"

class StateHandler
{
public:
  // Right now there's only a game state, but if
  // there are other states that need to be tracked
  // they can be put in here too.

  class GameState
  {
  public:
    enum State
    {
      Exit = -1,
      NotStarted = 0,
      Playing = 1,
      StartMenu = 2,
      PauseMenu = 3
    };

    operator int() { return static_cast<int>(this->state); }
    operator std::string();
    GameState(State state) : state(state) {}

    State state;
  };

  StateHandler() : gState(GameState::StartMenu), prevGState(GameState::StartMenu)
  {
  }

  void processJS(Joystick &js);

  GameState getGState();
  GameState getPrevGState();

  void setGState(GameState gState);

private:
  GameState gState;
  GameState prevGState;
};
