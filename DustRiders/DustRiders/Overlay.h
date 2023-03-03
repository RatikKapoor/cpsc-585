#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ECS.h"

class Overlay {
private:
	double lastTime;
	int nbFrames = 0;
	int currentFps = 0;

	EntityComponentSystem* ecs;
	std::string selectedEntity;

public:
	Overlay();
	void RenderOverlay(std::vector<Entity*>);
	void Cleanup();
};