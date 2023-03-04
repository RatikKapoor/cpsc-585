#include <iostream>

#include <PxPhysicsAPI.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
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
#include "Ground.h"
#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "MusicBuffer.h"
#include "WindowCallbacks.h"

int main()
{
	glfwInit();
	StateHandler stateHandle;

	Joystick keyboardJS;

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		JoystickHandler::addJS(GLFW_JOYSTICK_1);
	}
	else
	{
		JoystickHandler::addJS(keyboardJS);
	}

	Window window("DustRiders", glfwGetPrimaryMonitor());
	window.setCallbacks(std::make_shared<DustRidersWindowCallbacks>(std::ref(window), std::ref(stateHandle)));
#ifdef _DEBUG
	int windowHeight = 800;
	int windowWidth = 1422;
#else
	int windowHeight = window.getHeight();
	int windowWidth = window.getWidth();
#endif // _DEBUG

	auto physics = new PhysicsSystem();
	RenderingSystem renderer;
	Overlay overlay;

	// Shaders
	auto carShader = renderer.compileShader("car", "./car.vert", "./car.frag");
	auto mainShader = renderer.compileShader("basic", "./Main.vert", "./Main.frag");

	// To load in a model, just use "loadModelFromFile". Textures are handled automatically.
	auto carModel = renderer.loadModelFromFile("TestCar", "./assets/models/car-model.obj");
	auto testRock = renderer.loadModelFromFile("TestRock", "./assets/models/test-obstacle-rock.obj");
	auto groundPlane = renderer.loadModelFromFile("GroundPlane", "./assets/models/ground-plane.obj");

	EntityComponentSystem ecs = *EntityComponentSystem::getInstance();

	// Create main car
	ecs["car"] = new Vehicle("car", new Transform(), carModel, carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.5f, 0.f), 5);

	// Adds ground plane
	ecs["ground"] = new Ground("ground", new Transform(), groundPlane, carShader, glm::vec3(1.f));

	// Add AI cars
	ecs["car2"] = new Vehicle("car2", new Transform(), carModel, carShader, glm::vec3(1.f), physics, PxVec3(-4.f, 0.5f, 0.f), 4);
	ecs["car3"] = new Vehicle("car3", new Transform(), carModel, carShader, glm::vec3(1.f), physics, PxVec3(4.f, 0.5f, 0.f), 3);

	// Vehicle references
	auto playerVehicle = (Vehicle*)ecs["car"];
	auto botVehicle1 = (Vehicle*)ecs["car2"];
	auto botVehicle2 = (Vehicle*)ecs["car3"];

	std::vector<Vehicle*> vehicles{ playerVehicle, botVehicle1, botVehicle2 };

	// Start by focusing on the Player Vehicle
	Camera camera(ecs["car"], glm::vec3{ 0.0f, 0.0f, -3.0f }, glm::radians(60.0f), 75.0);

	//int obstacleCount = 0;
	//for (float dist = 0; dist <= 1000.5f; dist += 10.0f)
	//{
	//	entityList.emplace_back(new Entity());
	//	entityList.back()->transform = new Transform();

	//	if (obstacleCount % 2 == 0)
	//	{
	//		entityList.back()->transform->position = glm::vec3{-7.0f, 0.0f, dist};
	//	}
	//	else
	//	{
	//		entityList.back()->transform->position = glm::vec3{7.0f, 0.0f, dist};
	//	}

	//	entityList.back()->model = testRock;
	//	entityList.back()->shaderProgram = basicShader;
	//	entityList.back()->scale = glm::vec3{2.0f, 2.0f, 2.0f};

	//	obstacleCount++;
	//}

	SoundDevice* mysounddevice = SoundDevice::get();
	uint32_t /*ALuint*/ sound1 = SoundBuffer::get()->addSoundEffect("../sound/blessing.ogg");

	SoundSource mySpeaker;
	alSourcePlay(mySpeaker.Play(sound1));

	MusicBuffer myMusic("../sound/TownTheme.wav");
	std::cout << "playing town theme music...\n";
	myMusic.Play();

	ALint state = AL_PLAYING;
	std::cout << "playing sound\n";

	double lastTime = 0.0f;
	int i = 0;
	while (stateHandle.getGState() != StateHandler::GameState::Exit)
	{
		glfwPollEvents();
		if (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{
			myMusic.UpdateBufferStream();

			alGetSourcei(myMusic.getSource(), AL_SOURCE_STATE, &state);
		}

		if (stateHandle.getGState() == StateHandler::GameState::StartMenu)
		{
			overlay.RenderMenu(windowHeight / 2, windowWidth / 2);
			window.swapBuffers();
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

				if (stateHandle.getGState() == StateHandler::GameState::PauseMenu) {
					deltaT = 0.0f;
					overlay.RenderMenu(windowHeight / 2, windowWidth / 2);
				}
				else if (stateHandle.getGState() == StateHandler::GameState::GameWon) {
					deltaT = 0.0f;
					overlay.RenderWin(windowHeight / 2, windowWidth / 2);
				} else if (stateHandle.getGState() == StateHandler::GameState::GameLost) {
					deltaT = 0.0f;
					overlay.RenderLoss(windowHeight / 2, windowWidth / 2);
				}

				// Game Section
				if (stateHandle.getGState() == StateHandler::GameState::Playing || stateHandle.getGState() == StateHandler::GameState::PauseMenu)
				{
					// Vehicle physics
					for (Vehicle* vehicle : vehicles) {
						if (vehicle == playerVehicle) {
							vehicle->stepPhysics(deltaT, std::ref(JoystickHandler::getFirstJS()));
						}
						else {
							auto accel = (double)std::rand() / RAND_MAX * 0.5 + 0.2;
							vehicle->stepPhysics(deltaT, -accel, 0);
						}
					}

					// Updating camera focus based on z position of vehicles
					Entity* newFocus = nullptr;
					for (Vehicle* vehicle : vehicles)
						if (!newFocus || vehicle->transform->position.z > newFocus->transform->position.z) newFocus = vehicle;
					camera.setFocusEntity(newFocus);

					glm::mat4 perspective = glm::perspective(glm::radians(45.0f), window.getAspectRatio(), 0.01f, 1000.f);
					glm::mat4 view = camera.getView();
					for (int i = 0; i < vehicles.size(); i++) {
						glm::vec3 drawPos = perspective * view * glm::vec4{ vehicles[i]->transform->position, 1.0f };

						// giving a little bit of leeway by setting this to 1.1. This should become a parameter and approach 0 as the game progresses to force a winner. This is the storm distance
						if (drawPos.y / drawPos.z < -1.1f) {
							// Vehicle has lost the game
							if (vehicles[i] == playerVehicle) {
								stateHandle.setGState(StateHandler::GameState::GameLost);
							}
							else {
								// erasing AI vehicle if it lost
								ecs.erase(vehicles[i]->name);
								vehicles.erase(vehicles.begin() + i);

								if (vehicles.size() == 1) {
									if (vehicles[0] == playerVehicle) stateHandle.setGState(StateHandler::GameState::GameWon);
									else stateHandle.setGState(StateHandler::GameState::GameLost);
								}
							}
						}
					}
				}
				physics->updatePhysics(deltaT);
					
				window.swapBuffers();

				auto entities = ecs.getAll();
				renderer.updateRender(entities, camera, window.getAspectRatio());
				overlay.RenderOverlay(stateHandle.getGState(), entities);

				lastTime = t;
			}
		}
		glfwPollEvents();
		JoystickHandler::updateAll();
		stateHandle.processJS(JoystickHandler::getFirstJS());
	}
	window.close();

	overlay.Cleanup();

	glfwTerminate();
	return 0;
}
