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
    /**
     * @brief States where the game is active are greater than 0.
     * States where the game is inactive (pause, won, lost, etc),
     * are less than 0
     *
     */
    enum State
    {
      StartMenu = -3,
      PauseMenu = -2,
      Exit = -1,
      NotStarted = 0,
      Playing = 1,
      GameWonPlayer1 = 2,
      GameWonPlayer2 = 3,
      GameWonPlayer3 = 4,
      GameLost = 5,
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
