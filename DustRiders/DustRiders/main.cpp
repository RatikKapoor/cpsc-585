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
#include "ShaderProgram.h"
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

bool LogWriter::firstWriting = true;
std::string LogWriter::logFileName = "latest_output.log";
std::ofstream LogWriter::logFile;

int main()
{
	glfwInit();
	StateHandler stateHandle;

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

	// Shaders
	auto carShader = renderer.compileShader("car", "./car.vert", "./car.frag");
	auto mainShader = renderer.compileShader("basic", "./Main.vert", "./Main.frag");
	auto debugShader = renderer.compileShader("debug", "./debug.vert", "./debug.frag");

	// To load in a model, just use "loadModelFromFile". Textures are handled automatically.
	auto carModel = renderer.loadModelFromFile("TestCar", "./assets/models/car-model.obj");
	auto testRock1 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock1.obj");
	auto testRock2 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock2.obj");
	auto testRock3 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock3.obj");
	auto rayBeam = renderer.loadModelFromFile("RayBeam", "./assets/models/raygun-beam.obj");
	auto groundPlane = renderer.loadModelFromFile("GroundPlane", "./assets/models/ground-plane.obj");

	// NavMesh
	auto navMesh = new NavMesh();

	EntityComponentSystem ecs = *EntityComponentSystem::getInstance();

	// Adds ground plane
	ecs["ground"] = new Ground("ground", groundPlane, carShader, glm::vec3(1.f));

	ecs["raybeam"] = new RayBeam("raybeam", rayBeam, debugShader, glm::vec3(1.f), physics, std::ref(ecs), PxVec3(0.f, 1.75f, 0.f), 1);
	// Create main car
	ecs["car"] = new Vehicle("car", carModel, carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.5f, 0.f), 2, (RayBeam *)ecs["raybeam"]);

	// Add AI cars
	ecs["car2"] = new AIVehicle("car2", carModel, carShader, glm::vec3(1.f), physics, PxVec3(-20.f, 0.5f, 0.f), 4, navMesh, NULL);
	ecs["car3"] = new AIVehicle("car3", carModel, carShader, glm::vec3(1.f), physics, PxVec3(20.f, 0.5f, 0.f), 3, navMesh, NULL);
	//ecs["car4"] = new AIVehicle("car4", carModel, carShader, glm::vec3(1.f), physics, PxVec3(20.f, 0.5f, 0.f), 1, navMesh, NULL);
	//ecs["car5"] = new AIVehicle("car5", carModel, carShader, glm::vec3(1.f), physics, PxVec3(-20.f, 0.5f, 0.f), 5, navMesh, NULL);
	
	// Vehicle references
	auto playerVehicle = (Vehicle*)ecs["car"];
	auto botVehicle1 = (AIVehicle*)ecs["car2"];
	auto botVehicle2 = (AIVehicle*)ecs["car3"];
	//auto botVehicle3 = (AIVehicle*)ecs["car4"];
	//auto botVehicle4 = (AIVehicle*)ecs["car5"];

	std::vector<Vehicle *> vehicles{playerVehicle, botVehicle1, botVehicle2};
	std::vector<Vehicle *> inactiveVehicles;

	// Add obstacles
	int rockCount = 0;
	for (float dist = 30; dist <= 1000.5f; dist += 20.0f)
	{
		std::string name = "rock" + std::to_string(rockCount);
		float random = ((float)rand()) / (float)RAND_MAX;
		float r = random * 40.f;
		float x = -20.f + r;
		float random2 = ((float)rand()) / (float)RAND_MAX;
		float r2 = random2 * 2.f;
		float z = -1.f + r;
		Model* rock;
		switch (rockCount % 3)
		{
		case 0:
			rock = testRock1;
			break;
		case 1:
			rock = testRock2;
			break;
		case 2:
		default:
			rock = testRock3;
			break;
		}
		ecs[name] = new Obstacle(name,
			rock,
			carShader,
			glm::vec3(1.f),
			physics,
			PxVec3(x, -0.48f, (rockCount % 5 == 0) ? dist + z : dist - z),
			1);

		rockCount++;
	}

	// Start by focusing on the Player Vehicle
	Camera camera(ecs["car"], glm::vec3{0.0f, 0.0f, -3.0f}, glm::radians(60.0f), 75.0);

	SoundDevice *mysounddevice = SoundDevice::get();
	uint32_t /*ALuint*/ sound1 = SoundBuffer::get()->addSoundEffect("sound/blessing.ogg");
	uint32_t raybeamFire = SoundBuffer::get()->addSoundEffect("sound/laser-shoot.wav");

	SoundSource mySpeaker;

	MusicBuffer myMusic("sound/TownTheme.wav");
	std::cout << "playing town theme music...\n";
	myMusic.Play();

	ALint state = AL_PLAYING;
	std::cout << "playing sound\n";

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
		if (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{
			myMusic.UpdateBufferStream();

			alGetSourcei(myMusic.getSource(), AL_SOURCE_STATE, &state);
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
					LogWriter::log("Before reset: " + std::to_string(vehicles.size()) + " vehicles");
					vehicles.insert(vehicles.end(), inactiveVehicles.begin(), inactiveVehicles.end());
					inactiveVehicles.clear();

					for (Vehicle *vehicle : vehicles)
					{
						vehicle->restore();
						vehicle->reset();
					}

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
							vehicle->reloadTuning();
						}

						if (vehicle == playerVehicle)
						{
							if (vehicle->stepPhysics(deltaT, std::ref(JoystickHandler::getFirstJS())))
							{
								alSourcePlay(mySpeaker.Play(raybeamFire));
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
					Entity *newFocus = nullptr;
					for (Vehicle *vehicle : vehicles)
						if (!newFocus || vehicle->transform->position.z > newFocus->transform->position.z)
							newFocus = vehicle;
					camera.setFocusEntity(newFocus);

					glm::mat4 perspective = glm::perspective(glm::radians(45.0f), window.getAspectRatio(), 0.01f, 1000.f);
					glm::mat4 view = camera.getView();
					for (int i = 0; i < vehicles.size(); i++)
					{
						glm::vec3 drawPos = perspective * view * glm::vec4{vehicles[i]->transform->position, 1.0f};

						// giving a little bit of leeway by setting this to 1.1. This should become a parameter and approach 0 as the game progresses to force a winner. This is the storm distance

						if (drawPos.y / drawPos.z < -1.1f)
						{
							LogWriter::log(vehicles[i]->name + "(y, z) = (" + std::to_string(drawPos.y) + ", " + std::to_string(drawPos.z) + "), y/z = " + std::to_string(drawPos.y / drawPos.z));
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

								LogWriter::log("Suspended " + inactiveVehicles.back()->name);

								if (vehicles.size() == 1)
								{
									if (vehicles[0] == playerVehicle)
										stateHandle.setGState(StateHandler::GameState::GameWon);
									else
										stateHandle.setGState(StateHandler::GameState::GameLost);
								}
							}
						}
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
				overlay.RenderOverlay(stateHandle.getGState(), stateHandle.getPrevGState(), entities, &ecs);

				lastTime = t;
			}
		}
	}
	window.close();

	overlay.Cleanup();

	glfwTerminate();
	return 0;
}
