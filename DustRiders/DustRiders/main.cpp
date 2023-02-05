#include <iostream>

#include <PxPhysicsAPI.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "Entity.h"
#include "Model.h"
#include "PhysicsSystem.h"
#include "Overlay.h"
#include "ShaderProgram.h"
#include "Geometry.h"
#include "RenderingSystem.h"
#include "InputHandler.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();
	Window window(800, 800, "DustRiders");

	PhysicsSystem physics;
	RenderingSystem renderer;
	Overlay overlay;

	std::vector<Entity *> entityList;

	std::vector<Vertex> triangleVerts{ 
		Vertex{ glm::vec3{0.0f, 0.5f, 0.0f}, glm::vec3(0.0f), glm::vec2(0.0f) },
		Vertex{ glm::vec3 {-0.5f, -0.5f, 0.0f}, glm::vec3(0.0f), glm::vec2(0.0f) },
		Vertex{ glm::vec3{0.5f, -0.5f, 0.0f}, glm::vec3(0.0f), glm::vec2(0.0f) }
	};
	std::vector<unsigned int> triangleIndices{0, 1, 2};
	Model *triangle = renderer.loadModel("triangle", triangleVerts, triangleIndices);

	ShaderProgram *basicShader = renderer.compileShader("basic", "../DustRiders/basic.vert", "../DustRiders/basic.frag");

	entityList.reserve(465);
	for (int i = 0; i < 465; i++)
	{
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList[i];

		entityList.back()->model = triangle;
		entityList.back()->shaderProgram = basicShader;
	}

	// glfwSetFramebufferSizeCallback(window., framebuffer_size_callback);

	while (!window.shouldClose())
	{
		// Game Section
		// processInput(window.get);

		glfwPollEvents();

		physics.gScene->simulate(1.f / 60.f);
		physics.gScene->fetchResults(true);

		// auto position = physics.getPosition();

		window.swapBuffers();
		renderer.updateRender(entityList);

		overlay.RenderOverlay();
	}

	overlay.Cleanup();

	glfwTerminate();
	return 0;
}
