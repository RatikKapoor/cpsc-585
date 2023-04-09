#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DebugDefines.h"
#include "InputHandler.h"
#include "ECS.h"
#include "StateHandler.h"
#include "AudioHelper.h"
#include "Window.h"
#include "Camera.h"
#include "WindowCallbacks.h"
#include "Overlay.h"
#include "PhysicsSystem.h"
#include "RenderingSystem.h"
#include "ShaderProvider.h"
#include "ModelProvider.h"
#include "VehicleHandler.h"

class GameHandler {
public:
	static GameHandler* GetInstance() {
		if (_instance == nullptr) {
			_instance = new GameHandler();
		}
		return _instance;
	}

	GameHandler();

	//void Setup();
	void Loop();
	void Reset();
	StateHandler::GameState GetGameState();
	void Close();


protected:
	void RenderLoadingContent();
	void RenderFirstFewFrames();

	Window window;
	Camera camera;

	Constants* Constants;

	StateHandler stateHandle;
	AudioHelper audioHelper;
	Overlay overlay;

	int windowHeight;
	int windowWidth;

	EntityComponentSystem& ecs;
	PhysicsSystem* physics;
	TimeKeeper timer;
	RenderingSystem renderer;

	VehicleHandler vehicleHandler;

	int renderIterator;

private:
	static GameHandler* _instance;
};
