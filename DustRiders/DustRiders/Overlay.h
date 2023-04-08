#pragma once

#include "StateHandler.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ECS.h"
#include "TimeKeeper.h"

class Overlay
{
private:
	double lastTime;
	int nbFrames = 0;
	int currentFps = 0;
	std::map<std::string, ImTextureID> stormImagesMap;
	std::map<std::string, ImTextureID> batteryImagesMap;
	unsigned int stormFrameCounter;
	std::string selectedEntity;
	TimeKeeper timeKeeper;
	ImTextureID canvisImagePath;
	ImTextureID canvisTintImagePath;
	ImTextureID deathImagePath;

	ImTextureID loadFrameTexture(std::string);

public:
	Overlay();
	void LoadingContent(int windowWidth, int windowHeight);
	void RenderOverlay(StateHandler::GameState, StateHandler::GameState, std::vector<Entity *>, EntityComponentSystem *);
	void RenderMenu(int windowHeight, int windowWidth);
	void RenderPause(StateHandler::GameState prevState, int windowHeight, int windowWidth);
	void RenderWin(int windowHeight, int windowWidth);
	void RenderLoss(int windowHeight, int windowWidth);
	void RenderStorm(int frameWidth, int frameHeight, int screenWidth, int screenHeight);
	void RenderCanvis(int screenWidth, int screenHeight, float scaler);
	void RenderCanvisTint(int screenWidth, int screenHeight, float scaler);
	void RenderSpeedometer(float currentSpeed, int screenWidth, int screenHeight, float scaler);
	void RenderCharge(int charge, int screenWidth, int screenHeight, float scaler);
	void RenderPlace(int place, int screenWidth, int screenHeight, float scaler);
	void Cleanup();
};
