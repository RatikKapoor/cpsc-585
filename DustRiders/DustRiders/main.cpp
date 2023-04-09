#include "GameHandler.h"

int main()
{
	SoundDevice* mysounddevice = SoundDevice::get();
	glfwInit();
	GameHandler* game = GameHandler::GetInstance();

	Joystick keyboardJS;

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		JoystickHandler::addJS(GLFW_JOYSTICK_1);
	}
	else
	{
		JoystickHandler::addJS(keyboardJS);
	}

	while (game->GetGameState() != StateHandler::GameState::Exit)
	{
		game->Loop();
	}
	game->Close();

	glfwTerminate();
	return 0;
}
