#pragma once

#include "StateHandler.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Overlay
{
private:
	double lastTime;
	int nbFrames = 0;
	int currentFps = 0;

public:
	Overlay();
	void RenderMenu(int windowHeight, int windowWidth);
	void RenderPause(int windowHeight, int windowWidth);
	void RenderOverlay(StateHandler::GameState);
	void Cleanup();
};