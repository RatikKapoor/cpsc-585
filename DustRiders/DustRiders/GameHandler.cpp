#include "GameHandler.h"

GameHandler *GameHandler::_instance = nullptr;

GameHandler::GameHandler() : window("DustRiders", glfwGetPrimaryMonitor()),
#ifndef NO_DEATH
														 camera(glm::vec3{0.0f, 0.0f, -3.0f}, glm::radians(60.0f), 125.0),
#else
														 camera(glm::vec3{0.0f, 0.0f, -3.0f}, glm::radians(60.0f), 125.0),
#endif
														 ecs(*EntityComponentSystem::getInstance()),
														 physics(new PhysicsSystem(ecs)),
														 stateHandle(),
														 timer(),
														 overlay(),
														 renderer(),
														 vehicleHandler(ecs, physics, stateHandle, timer)
{
	AudioHelper::Initialize();
	Constants = ConstantsHelper::getConstants();

	window.setCallbacks(std::make_shared<DustRidersWindowCallbacks>(std::ref(window), std::ref(stateHandle)));
#ifdef _DEBUG
	windowHeight = 800;
	windowWidth = 1422;
#else
	windowHeight = window.getHeight();
	windowWidth = window.getWidth();
#endif // _DEBUG

	// Render Loading Content
	RenderLoadingContent();
	renderer.screenHeight = windowHeight;
	renderer.screenWidth = windowWidth;

	ShaderProvider::initialize(renderer);
	ModelProvider::initialize(renderer);

	ChunkHandler::initialize(ecs, physics);

	vehicleHandler.InitCars();

	// Render first frames
	RenderFirstFewFrames();

	renderIterator = 0;
	timer.updateTime();
}

void GameHandler::RenderLoadingContent()
{
	for (int i = 0; i < 15; i++)
	{
		glEnable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_FRAMEBUFFER_SRGB);

		overlay.LoadingContent(windowWidth, windowHeight);
		window.swapBuffers();
	}
}

void GameHandler::RenderFirstFewFrames()
{
	for (int i = 0; i < 15; i++)
	{
		glEnable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_FRAMEBUFFER_SRGB);
		window.swapBuffers();
	}
}

void GameHandler::Loop()
{
	glfwPollEvents();
	JoystickHandler::updateAll();
	stateHandle.processJS(JoystickHandler::getFirstJS());

	// Game hasn't started, still on the initial start menu
	if (stateHandle.getGState() == StateHandler::GameState::StartMenu)
	{
		window.swapBuffers();
		overlay.RenderMenu(windowHeight / 2, windowWidth / 2);
	}
	else
	{
		if (timer.getCounterRaw() > 0.0167)
		{
			// First few renders should be simulated with manual step to avoid objects clipping through ground
			if (renderIterator < 15)
			{
				timer.setDeltaT(1.0 / 60.0);
				timer.setCounter(1.f / 60.f);
				renderIterator++;
			}
			// Reset the cars back to where they were and restart the game
			// Not implemented in release version of game
			if (stateHandle.getRState() == StateHandler::ReloadState::GameReset)
			{
				vehicleHandler.Reset();

				ChunkHandler::reset();
				stateHandle.setRState(StateHandler::ReloadState::None);
				stateHandle.setGState(StateHandler::GameState::PauseMenu);
			}

			StateHandler::GameState gState = stateHandle.getGState();

			if (gState < 0 || gState > StateHandler::GameState::Playing) // The game is inactive (paused and similar)
			{
				switch (gState)
				{
				case StateHandler::GameState::PauseMenu:
					timer.pauseTime();
					overlay.RenderPause(stateHandle.getPrevGState(), windowHeight / 2, windowWidth / 2);
					break;
				case StateHandler::GameState::GameWonPlayer1:
				case StateHandler::GameState::GameWonPlayer2:
				case StateHandler::GameState::GameWonPlayer3:
					timer.pauseTime();
					overlay.RenderWin(windowHeight / 2, windowWidth / 2, gState);
					break;
				case StateHandler::GameState::GameLost:
					timer.pauseTime();
					overlay.RenderLoss(windowHeight / 2, windowWidth / 2);
					break;
				}
			}
			else // The game is active
			{
				// Render game overlays
				vehicleHandler.RenderPlayerDataOverlay(overlay, windowWidth, windowHeight);
				overlay.RenderStorm(1920, 150, windowWidth, windowHeight);

				timer.playTime();
				// Vehicle physics
				vehicleHandler.UpdateVehiclePhysics();
				if (stateHandle.getRState() == StateHandler::ReloadState::Tuning)
				{
					stateHandle.setRState(StateHandler::ReloadState::None);
				}

				// Updating camera focus based on z position of vehicles
#ifndef NO_DEATH
				auto furthest = vehicleHandler.GetFurthestVehicle();
				camera.setFocusEntity(furthest);
				ChunkHandler::updateChunks(furthest);
#else
				camera.setFocusEntity(ecs["car"]);
				ChunkHandler::updateChunks(ecs["car"]);
#endif

				glm::mat4 perspective = glm::perspective(glm::radians(45.0f), window.getAspectRatio(), 0.01f, 1000.f);
				glm::mat4 view = camera.getView();
				vehicleHandler.CheckForDeath(perspective * view);
				vehicleHandler.ControllerChargeNotification();
			}
			physics->updatePhysics(timer.getCounter());
			vehicleHandler.UpdateRayBeamPositions();

			window.swapBuffers();

			auto entities = ecs.getAll();
			renderer.updateRender(ecs, camera, window.getAspectRatio());
			overlay.RenderOverlay(stateHandle.getGState(), stateHandle.getPrevGState(), entities, &ecs);

			vehicleHandler.Debug(); // Only does anything in debug mode

			timer.setCounter(0.f);
		}
	}
	timer.updateTime();
}

void GameHandler::Reset()
{
}

StateHandler::GameState GameHandler::GetGameState()
{
	return stateHandle.getGState();
}

void GameHandler::Close()
{
	window.close();

#ifdef _DEBUG
	AIPathHandler::write();
#endif // _DEBUG

	overlay.Cleanup();
}
