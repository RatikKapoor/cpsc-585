
#include "Window.h"
#include "InputHandler.h"
#include "StateHandler.h"

class DustRidersWindowCallbacks : public CallbackInterface
{
public:
  DustRidersWindowCallbacks(Window &window, StateHandler &state) : window(window), state(state) {}
  virtual void keyCallback(int key, int scancode, int action, int mods)
  {
    Joystick &js = JoystickHandler::getFirstJS();

    if (!js.isPseudo())
    {
      return;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      if (js.getButton(Xbox::Button::XBOX_MENU))
      {
        js.releaseEsc();
      }
      else
      {
        js.pressEsc();
      }
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
      js.pressR();
    }
    if (GLFW_KEY_R && action == GLFW_RELEASE)
    {
      js.releaseR();
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
      if (js.getButton(Xbox::Button::XBOX_X))
      {
        js.releaseX();
      }
      else
      {
        js.pressX();
      }
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
      js.pressSpace();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
      js.releaseSpace();
    }

    if (key == GLFW_KEY_X && action == GLFW_RELEASE)
    {
      js.releaseX();
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
      js.pressW();
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
      // Forward released
      js.releaseW();
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
      // Back pressed
      js.pressS();
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
      // Back released
      js.releaseS();
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
      js.pressA();
      // Left pressed
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
      // Left released
      js.releaseA();
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
      // Right pressed
      js.pressD();
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
      // Right released
      js.releaseD();
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
    {
      // Right pressed
      js.pressLeftShift();
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    {
      // Right released
      js.releaseLeftShift();
    }
    else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
      js.pressEnter();
    }
    else if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    {
      js.releaseEnter();
    }

    else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
    {
      js.pressBackspace();
    }
    else if (key == GLFW_KEY_BACKSPACE && action == GLFW_RELEASE)
    {
      js.releaseBackspace();
    }
  }

  virtual void mouseButtonCallback(int button, int action, int mods) {}
  virtual void cursorPosCallback(double xpos, double ypos) {}
  virtual void scrollCallback(double xoffset, double yoffset) {}
  virtual void windowSizeCallback(int width, int height)
  {
    glViewport(0, 0, width, height);
  }

  Window &window;
  StateHandler &state;

protected:
};