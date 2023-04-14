#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
#include <stb_image.h>

#include "StateHandler.h"
#include "InputHandler.h"
#include "ECS.h"
#include "TimeKeeper.h"
#include "Vehicle.h"
#include "ChunkHandler.h"

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
	ImTextureID canvasImageRed;
	ImTextureID canvasImageGreen;
	ImTextureID canvasImageBlue;
	ImTextureID canvasImageBlack;
	ImTextureID canvasTintImage;

	ImTextureID loadFrameTexture(std::string);

public:
	Overlay();
	void LoadingContent(int windowWidth, int windowHeight);
	void RenderOverlay(StateHandler::GameState, StateHandler::GameState, std::vector<Entity *>, EntityComponentSystem *);
	void RenderMenu(int windowHeight, int windowWidth);
	void RenderPause(StateHandler::GameState prevState, int windowHeight, int windowWidth);
	void RenderWin(int windowHeight, int windowWidth, StateHandler::GameState player);
	void RenderLoss(int windowHeight, int windowWidth);
	void RenderStorm(int frameWidth, int frameHeight, int screenWidth, int screenHeight);
	void RenderCanvas(int colour, int screenWidth, int screenHeight, float scaler, float position);
	void RenderCanvasTint(int screenWidth, int screenHeight, float scaler, float position);
	void RenderSpeedometer(float currentSpeed, int screenWidth, int screenHeight, float scaler, float position);
	void RenderCharge(int charge, int screenWidth, int screenHeight, float scaler, float position);
	void RenderPlace(int place, int screenWidth, int screenHeight, float scaler, float position);
	void Cleanup();
};
