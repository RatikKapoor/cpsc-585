#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Overlay {
private:
	double lastTime;
	int nbFrames = 0;
	int currentFps = 0;

public:
	Overlay();
	void RenderOverlay(int);
	void Cleanup();
};