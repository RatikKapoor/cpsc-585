#pragma region Includes

#include <iostream>

#include <PxPhysicsAPI.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "ECS.h"
#include "Entity.h"
#include "Mesh.h"
#include "PhysicsSystem.h"
#include "Overlay.h"
#include "ShaderProvider.h"
#include "ModelProvider.h"
#include "ObstacleHandler.h"
#include "Geometry.h"
#include "RenderingSystem.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Vehicle.h"
#include "AIVehicle.h"
#include "Ground.h"
#include "Obstacle.h"
#include "RayBeam.h"
#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "MusicBuffer.h"
#include "WindowCallbacks.h"
#include "LogWriter.h"
#include "AIPathHandler.h"
#include "ConstantsHelper.h"
#include "ChunkHandler.h"
#include "DebugDefines.h"

#pragma endregion
int main()
{
	glfwInit();
	StateHandler stateHandle;
	Constants *Constants = ConstantsHelper::getConstants();

	Window window("DustRiders", glfwGetPrimaryMonitor());
	window.setCallbacks(std::make_shared<DustRidersWindowCallbacks>(std::ref(window), std::ref(stateHandle)));
#ifdef _DEBUG
	int windowHeight = 800;
	int windowWidth = 1422;
#else
	int windowHeight = window.getHeight();
	int windowWidth = window.getWidth();
#endif // _DEBUG

	Overlay overlay;

	// Let users know that content is loading in

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

	Joystick keyboardJS;

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		JoystickHandler::addJS(GLFW_JOYSTICK_1);
	}
	else
	{
		JoystickHandler::addJS(keyboardJS);
	}

	auto physics = new PhysicsSystem();
	RenderingSystem renderer;

	// Initialize Providers
	ShaderProvider::initialize(renderer);
	ModelProvider::initialize(renderer);

	EntityComponentSystem ecs = *EntityComponentSystem::getInstance();

	// Start chunking system
	ChunkHandler::initialize(ecs, physics);

	ecs["raybeam"] = new RayBeam("raybeam", ModelProvider::rayBeam, ShaderProvider::debugShader, glm::vec3(1.f), physics, std::ref(ecs), PxVec3(0.f, 1.75f, 0.f), 1);
	// Create main car
	ecs["car"] = new Vehicle("car", ModelProvider::carModel, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.5f, 0.f), 2, (RayBeam *)ecs["raybeam"]);


	// Add AI cars
	ecs["car2"] = new AIVehicle("car2", ModelProvider::carModel, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(-20.f, 0.5f, 0.f), 4, NULL, "./assets/drivingPaths/path1.json");
	ecs["car3"] = new AIVehicle("car3", ModelProvider::carModel, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(20.f, 0.5f, 0.f), 3, NULL
								//, "./assets/drivingPaths/path2.json"
	);

	// Vehicle references
	auto playerVehicle = (Vehicle *)ecs["car"];
	auto botVehicle1 = (AIVehicle *)ecs["car2"];
	auto botVehicle2 = (AIVehicle *)ecs["car3"];

	std::vector<Vehicle *> vehicles{
		playerVehicle,
		botVehicle1,
		botVehicle2};
	std::vector<Vehicle *> inactiveVehicles;

	// Add obstacles
	ObstacleHandler::renderObstacles(ecs, physics);

#ifndef NO_DEATH
	// Start by focusing on the Player Vehicle
	Camera camera(ecs["car"], glm::vec3{0.0f, 0.0f, -3.0f}, glm::radians(60.0f), 125.0);
#else
	// Start by focusing on the Player Vehicle
	Camera camera(ecs["car"], glm::vec3{0.0f, 0.0f, -3.0f}, glm::radians(30.0f), 125.0);
#endif

	SoundDevice *mysounddevice = SoundDevice::get();
	uint32_t /*ALuint*/ engine = SoundBuffer::get()->addSoundEffect("sound/engine.ogg");
	uint32_t raybeamFire = SoundBuffer::get()->addSoundEffect("sound/laser-shoot.wav");

	// Collision sound effect
	uint32_t /*ALuint*/ collision = SoundBuffer::get()->addSoundEffect("sound/collision.ogg");
	// uint32_t raybeamFire = SoundBuffer::get()->addSoundEffect("sound/laser-shoot.wav");

	SoundSource raybeamSpeaker;
	SoundSource engineSpeaker;
	engineSpeaker.changeMusicVolume(0.5f);

	ALint engineSoundState = AL_STOPPED;

	MusicBuffer storm("sound/storm_long.wav");
	storm.Play();
	ALint stormState = AL_PLAYING;
	storm.changeMusicVolume(0.3f);

	MusicBuffer theme("sound/jumphigher.ogg");
	theme.Play();
	ALint themeState = AL_PLAYING;
	theme.changeMusicVolume(0.2f);

	double lastTime = 0.0f;
	int i = 0;

	for (int i = 0; i < 15; i++)
	{
		glEnable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_FRAMEBUFFER_SRGB);
		window.swapBuffers();
	}

	while (stateHandle.getGState() != StateHandler::GameState::Exit)
	{
		glfwPollEvents();
		JoystickHandler::updateAll();
		stateHandle.processJS(JoystickHandler::getFirstJS());

		// The sound buffer should always update, not dependant on game state
		if (stormState == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{
			storm.UpdateBufferStream();
			alGetSourcei(storm.getSource(), AL_SOURCE_STATE, &stormState);
		}
		// else if (stormState != AL_PLAYING ) {
		//	std::cout << "arrived\n";
		//	storm.Play();
		//	stormState = AL_PLAYING;
		//	//storm.UpdateBufferStream();
		//	std::cout << stormState + "\n";
		// }

		if (themeState == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{
			theme.UpdateBufferStream();
			alGetSourcei(theme.getSource(), AL_SOURCE_STATE, &themeState);
		}

		// Game hasn't started, still on the initial start menu
		if (stateHandle.getGState() == StateHandler::GameState::StartMenu)
		{
			window.swapBuffers();
			overlay.RenderMenu(windowHeight / 2, windowWidth / 2);
		}
		else
		{
			auto t = glfwGetTime();
			if (t - lastTime > 0.0167)
			{
				auto deltaT = t - lastTime;
				if (deltaT > 0.1f)
				{
					deltaT = 0.1f;
				}

				// First few renders should be simulated with manual step to avoid objects clipping through ground
				if (i < 15)
				{
					deltaT = (1.f / 60.f);
					i++;
				}
				// Reset the cars back to where they were and restart the game
				// Not implemented in release version of game
				if (stateHandle.getRState() == StateHandler::ReloadState::GameReset)
				{

					int restoreCount = 0;
					vehicles.insert(vehicles.end(), inactiveVehicles.begin(), inactiveVehicles.end());
					inactiveVehicles.clear();

					for (Vehicle *vehicle : vehicles)
					{
						vehicle->restore();
						vehicle->reset();
					}
					ChunkHandler::reset();
					deltaT = 0.f;
					stateHandle.setRState(StateHandler::ReloadState::None);
					stateHandle.setGState(StateHandler::GameState::PauseMenu);
				}

				StateHandler::GameState gState = stateHandle.getGState();

				if (gState < 0 || gState > StateHandler::GameState::Playing) // The game is inactive (paused and similar)
				{
					switch (gState)
					{
					case StateHandler::GameState::PauseMenu:
						deltaT = 0.0f;
						overlay.RenderPause(stateHandle.getPrevGState(), windowHeight / 2, windowWidth / 2);
						break;
					case StateHandler::GameState::GameWon:
						deltaT = 0.0f;
						overlay.RenderWin(windowHeight / 2, windowWidth / 2);
						break;
					case StateHandler::GameState::GameLost:
						deltaT = 0.0f;
						overlay.RenderLoss(windowHeight / 2, windowWidth / 2);
						break;
					}
				}
				else // The game is active
				{
					// Vehicle physics
					for (Vehicle *vehicle : vehicles)
					{
						if (stateHandle.getRState() == StateHandler::ReloadState::Tuning)
						{
							ConstantsHelper::refreshConstants(); // Load up new constants from file
							vehicle->reloadTuning();			 // Apply changes
						}

						if (vehicle == playerVehicle)
						{
							if (vehicle->stepPhysics(deltaT, std::ref(JoystickHandler::getFirstJS())))
							{
								alSourcePlay(raybeamSpeaker.Play(raybeamFire));
							}

							alGetSourcei(engineSpeaker.getSource(), AL_SOURCE_STATE, &engineSoundState);
							if (vehicle->currentSpeed() > 0.2 && engineSoundState != AL_PLAYING)
							{
								alSourcePlay(engineSpeaker.Play(engine));
							}

							if (vehicle->engineGased(std::ref(JoystickHandler::getFirstJS())))
							{
								engineSpeaker.changeMusicVolume(1.f);
							}
							else
							{
								engineSpeaker.changeMusicVolume(0.5f);
							}
						}
						else
						{
							((AIVehicle *)vehicle)->stepPhysics(deltaT);
						}
					}
					if (stateHandle.getRState() == StateHandler::ReloadState::Tuning)
					{
						stateHandle.setRState(StateHandler::ReloadState::None);
					}

// Updating camera focus based on z position of vehicles
#ifndef NO_DEATH
					Entity *newFocus = nullptr;
					for (Vehicle *vehicle : vehicles)
						if (!newFocus || vehicle->transform->position.z > newFocus->transform->position.z)
							newFocus = vehicle;
					camera.setFocusEntity(newFocus);
					ChunkHandler::updateChunks(newFocus);
#else
					camera.setFocusEntity(playerVehicle);
					ChunkHandler::updateChunks(playerVehicle);
#endif

					glm::mat4 perspective = glm::perspective(glm::radians(45.0f), window.getAspectRatio(), 0.01f, 1000.f);
					glm::mat4 view = camera.getView();
					for (int i = 0; i < vehicles.size(); i++)
					{
						glm::vec3 drawPos = perspective * view * glm::vec4{vehicles[i]->transform->position, 1.0f};

						// giving a little bit of leeway by setting this to 1.1. This should become a parameter and approach 0 as the game progresses to force a winner. This is the storm distance

#ifndef NO_DEATH
						if (drawPos.y / drawPos.z < -1.1f)
						{
							// Vehicle has lost the game
							if (vehicles[i] == playerVehicle)
							{
								stateHandle.setGState(StateHandler::GameState::GameLost);
							}
							else
							{
								// erasing AI vehicle if it lost

								// store inactive vehicles
								vehicles[i]->suspend();
								inactiveVehicles.push_back(vehicles[i]);
								vehicles.erase(vehicles.begin() + i);

								if (vehicles.size() == 1)
								{
									if (vehicles[0] == playerVehicle)
										stateHandle.setGState(StateHandler::GameState::GameWon);
									else
										stateHandle.setGState(StateHandler::GameState::GameLost);
								}
							}
						}
#endif
					}
				}
				physics->updatePhysics(deltaT);
				for (Vehicle *v : vehicles)
				{
					v->updateRayBeamPos();
				}

				window.swapBuffers();

				auto entities = ecs.getAll();
				renderer.updateRender(entities, camera, window.getAspectRatio());
#ifdef _DEBUG
				overlay.RenderOverlay(stateHandle.getGState(), stateHandle.getPrevGState(), entities, &ecs);
				playerVehicle->saveLocation(); // Save player location history to json
#endif										   // _DEBUG

				lastTime = t;
			}
		}
	}
	window.close();

	AIPathHandler::write();

	overlay.Cleanup();

	glfwTerminate();
	return 0;
}
