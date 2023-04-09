#include "GameHandler.h"

int main()
{
	glfwInit();

	GameHandler* game = GameHandler::GetInstance();

	while (game->GetGameState() != StateHandler::GameState::Exit)
	{
		game->Loop();
	}

	game->Close();

	glfwTerminate();
	return 0;
}
