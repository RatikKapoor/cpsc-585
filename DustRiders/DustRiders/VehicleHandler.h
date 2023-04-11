#pragma once

#include <glm/glm.hpp>

#include "ECS.h"
#include "DebugDefines.h"
#include "Vehicle.h"
#include "AIVehicle.h"
#include "RayBeam.h"
#include "ModelProvider.h"
#include "ShaderProvider.h"
#include "PhysicsSystem.h"
#include "StateHandler.h"
#include "AudioHelper.h"

class VehicleHandler
{
public:
	VehicleHandler(EntityComponentSystem &ecs, PhysicsSystem *physics, StateHandler &stateHandler, TimeKeeper &timer) : ecs(ecs),
																																																											physics(physics),
																																																											stateHandler(stateHandler),
																																																											timer(timer)
	{
		Joystick keyboardJS;

		if (glfwJoystickPresent(GLFW_JOYSTICK_1))
		{
			JoystickHandler::addJS(GLFW_JOYSTICK_1);
		}
		else
		{
			JoystickHandler::addJS(keyboardJS);
		}
	}

	void InitCars()
	{
		ecs["raybeam"] = new RayBeam("raybeam", ModelProvider::rayBeam, ShaderProvider::flameShader, glm::vec3(1.f), physics, std::ref(ecs), PxVec3(0.f, 1.75f, 0.f), 1);
		ecs["flames"] = new Flames("flames", ModelProvider::carFlames, ShaderProvider::flameShader, glm::vec3(1.f), 1);
		ecs["raybeam2"] = new RayBeam("raybeam2", ModelProvider::rayBeam, ShaderProvider::flameShader, glm::vec3(1.f), physics, std::ref(ecs), PxVec3(0.f, 1.75f, 0.f), 1);
		ecs["flames2"] = new Flames("flames2", ModelProvider::carFlames, ShaderProvider::flameShader, glm::vec3(1.f), 1);
		ecs["raybeam3"] = new RayBeam("raybeam3", ModelProvider::rayBeam, ShaderProvider::flameShader, glm::vec3(1.f), physics, std::ref(ecs), PxVec3(0.f, 1.75f, 0.f), 1);
		ecs["flames3"] = new Flames("flames3", ModelProvider::carFlames, ShaderProvider::flameShader, glm::vec3(1.f), 1);

		// Create main car
		vehicles.push_back(new Vehicle("car", ModelProvider::carModel, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.5f, 0.f), 2, (RayBeam *)ecs["raybeam"], &JoystickHandler::getFirstJS()));
		vehicles.back()->setFlames((Flames *)ecs["flames"]);
		// Add other cars
		if (glfwJoystickPresent(GLFW_JOYSTICK_2))
		{
			JoystickHandler::addJS(GLFW_JOYSTICK_2);
			vehicles.push_back(new Vehicle("car2", ModelProvider::carModel, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(-20.f, 0.5f, 0.f), 4, (RayBeam *)ecs["raybeam2"], &JoystickHandler::getJoystick(GLFW_JOYSTICK_2)));
			vehicles.back()->setFlames((Flames *)ecs["flames2"]);
		}
		else
		{
			vehicles.push_back(new AIVehicle("car2ai", ModelProvider::carModel, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(-20.f, 0.5f, 0.f), 4, (RayBeam *)ecs["raybeam2"], "./assets/drivingPaths/path1.json"));
			vehicles.back()->setFlames((Flames *)ecs["flames2"]);
		}
		if (glfwJoystickPresent(GLFW_JOYSTICK_3))
		{
			JoystickHandler::addJS(GLFW_JOYSTICK_3);
			vehicles.push_back(new Vehicle("car3", ModelProvider::carModel, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(20.f, 0.5f, 0.f), 3, (RayBeam *)ecs["raybeam3"], &JoystickHandler::getJoystick(GLFW_JOYSTICK_3)));
			vehicles.back()->setFlames((Flames *)ecs["flames3"]);
		}
		else
		{
			vehicles.push_back(new AIVehicle("car3ai", ModelProvider::carModel, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(20.f, 0.5f, 0.f), 3, (RayBeam *)ecs["raybeam3"]

																			 //, "./assets/drivingPaths/path2.json"
																			 ));
			vehicles.back()->setFlames((Flames *)ecs["flames3"]);
		}

		for (auto &v : vehicles)
		{
			ecs[v->name] = v;
		}
	}

	void Reset()
	{
		vehicles.insert(vehicles.end(), inactiveVehicles.begin(), inactiveVehicles.end());
		inactiveVehicles.clear();

		for (Vehicle *vehicle : vehicles)
		{
			vehicle->restore();
			vehicle->reset();
		}
	}

	void UpdateVehiclePhysics()
	{
		for (Vehicle *vehicle : vehicles)
		{
			if (stateHandler.getRState() == StateHandler::ReloadState::Tuning)
			{
				ConstantsHelper::refreshConstants(); // Load up new constants from file
				vehicle->reloadTuning();						 // Apply changes
			}

			// if (vehicle == playerVehicle)
			if (!isAiVehicle(vehicle))
			{
				vehicle->stepPhysics(timer.getCounter());
				if (vehicle->currentSpeed() > 0.2)
				{
					AudioHelper::PlayEngineNoise();
				}

				if (vehicle->engineGased())
				{
					AudioHelper::SetEngineVolume(1.f);
				}
				else
				{
					AudioHelper::SetEngineVolume(0.5f);
				}
			}
			else
			{
				((AIVehicle *)vehicle)->stepPhysics(timer.getCounter());
			}
		}
	}

	Entity *GetFurthestVehicle()
	{
		Entity *newFocus = nullptr;
		for (Vehicle *vehicle : vehicles)
			if (!newFocus || vehicle->transform->position.z > newFocus->transform->position.z)
				newFocus = vehicle;
		return newFocus;
	}

	void CheckForDeath(glm::mat4 pv)
	{
		for (int i = 0; i < vehicles.size(); i++)
		{
			glm::vec3 drawPos = pv * glm::vec4{vehicles[i]->transform->position, 1.0f};

#pragma region Controller Rumble When Close to Storm
			if (!isAiVehicle(vehicles[i]))
			{
				if (drawPos.y / drawPos.z < -0.7f)
					vehicles[i]->js->setVibrate(2000);
				else
					vehicles[i]->js->setVibrate(0);
			}
#pragma endregion

			// giving a little bit of leeway by setting this to 1.1. This should become a parameter and approach 0 as the game progresses to force a winner. This is the storm distance

#ifndef NO_DEATH
			if (drawPos.y / drawPos.z < -1.1f)
			{
				JoystickHandler::getFirstJS().setVibrate(0);
				// Vehicle has lost the game
				if (!isAiVehicle(vehicles[i]))
				{
					stateHandler.setGState(StateHandler::GameState::GameLost);
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
						if (!isAiVehicle(vehicles[0]))
							stateHandler.setGState(StateHandler::GameState::GameWon);
						else
							stateHandler.setGState(StateHandler::GameState::GameLost);
					}
				}
				AudioHelper::PlayThunder();
			}
#endif
		}
	}

	void UpdateRayBeamPositions()
	{
		for (Vehicle *v : vehicles)
		{
			v->updateRayBeamPos();
		}
	}

	void ControllerChargeNotification() {
		for (Vehicle* v : vehicles)
		{
			if (v->js == NULL)
				continue;
			if (v->wasRaybeamJustCharged())
			{
				v->js->setVibrateFrames(65534, 30); // Vibrate for 30 frames (half a second)
			}
		}
	}

	void Debug()
	{
#ifdef _DEBUG
		((Vehicle *)ecs["car"])->saveLocation(); // Save player location history to json
#endif
	}

protected:
	bool isAiVehicle(Vehicle *v)
	{
		auto name = v->name;
		return name.compare(name.length() - 2, name.length(), "ai") == 0;
	}

	EntityComponentSystem &ecs;
	PhysicsSystem *physics;
	StateHandler &stateHandler;
	TimeKeeper &timer;

	std::vector<Vehicle *> vehicles;
	std::vector<Vehicle *> inactiveVehicles;
};
