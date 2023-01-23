#include <iostream>

#include <PxPhysicsAPI.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "Entity.h"
#include "PhysicsSystem.h"
#include "Overlay.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	PhysicsSystem physics;
	Overlay overlay;

	std::vector<Entity*> entityList;
	entityList.reserve(465);
	for (int i = 0; i < 465; i++)
	{
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList[i];
	}

	glfwInit();
	Window window(800, 800, "DustRiders");

	//glfwSetFramebufferSizeCallback(window., framebuffer_size_callback);
	
	while (!window.shouldClose()) {
		// Game Section
		//processInput(window.get);

		physics.gScene->simulate(1.f / 60.f);
		physics.gScene->fetchResults(true);

		auto position = physics.getPosition();

		window.swapBuffers();
		glfwPollEvents();
		glClearColor(0.5f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		overlay.RenderOverlay();
		
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
	}

	overlay.Cleanup();
	

	glfwTerminate();
	return 0;
}