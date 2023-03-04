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
      PauseMenu = 3,
      GameWon = 4,
      GameLost = 5
    };

    operator int() { return static_cast<int>(this->state); }
    operator std::string();
    GameState(State state) : state(state) {}

    State state;
    State reload;
  };

  class ReloadState
  {
  public:
    enum State
    {
      None = -1,
      Tuning = 0,
      GameReset
    };

    State state;
    operator int() { return static_cast<int>(this->state); }
    ReloadState(State state) : state(state) {}
  };

  StateHandler() : gState(GameState::StartMenu), prevGState(GameState::StartMenu), rState(ReloadState::None)
  {
  }

  void processJS(Joystick &js);

  GameState getGState();
  GameState getPrevGState();
  ReloadState getRState();

  void setGState(GameState gState);
  void setRState(ReloadState rState);

private:
  GameState gState;
  GameState prevGState;
  ReloadState rState;
};
