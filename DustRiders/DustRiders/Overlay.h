#pragma once

#include "StateHandler.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ECS.h"

class Overlay
{
private:
	double lastTime;
	int nbFrames = 0;
	int currentFps = 0;

	std::string selectedEntity;

public:
	Overlay();
	void RenderOverlay(StateHandler::GameState, StateHandler::GameState, std::vector<Entity *>, EntityComponentSystem *);
	void RenderMenu(int windowHeight, int windowWidth);
	void RenderPause(int windowHeight, int windowWidth);
	void RenderWin(int windowHeight, int windowWidth);
	void RenderLoss(int windowHeight, int windowWidth);
	void Cleanup();
};
