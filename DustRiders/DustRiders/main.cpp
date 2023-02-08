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
#include "Mesh.h"
#include "PhysicsSystem.h"
#include "Overlay.h"
#include "ShaderProgram.h"
#include "Geometry.h"
#include "RenderingSystem.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Vehicle.h"

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
	Camera camera(glm::radians(0.0f), glm::radians(0.0f), 80.0);

	std::vector<Vertex> cubeVerts{
			Vertex{glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3(0.0f), glm::vec2(0.0f)},
			Vertex{glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec3(0.0f), glm::vec2(0.0f)},
			Vertex{glm::vec3{-0.5f, -0.5f, 0.5f}, glm::vec3(0.0f), glm::vec2(0.0f)},
			Vertex{glm::vec3{0.5f, -0.5f, 0.5f}, glm::vec3(0.0f), glm::vec2(0.0f)},
			Vertex{glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3(0.0f), glm::vec2(0.0f)},
			Vertex{glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3(0.0f), glm::vec2(0.0f)},
			Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3(0.0f), glm::vec2(0.0f)},
			Vertex{glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3(0.0f), glm::vec2(0.0f)}};
	std::vector<unsigned int> cubeIndices{
			// front face
			0, 1, 2,
			0, 2, 3,

			// right face
			4, 0, 3,
			4, 3, 7,

			// back face
			5, 4, 7,
			5, 7, 6,

			// left face
			1, 5, 6,
			1, 6, 2,

			// top face
			0, 4, 5,
			0, 5, 1,

			// bottom face
			2, 3, 7,
			2, 7, 6};
	Model *cubeModel = new Model();
	cubeModel->meshes.push_back(Mesh(cubeVerts, cubeIndices));
	cubeModel = renderer.addModel("cube", cubeModel);

	ShaderProgram *basicShader = renderer.compileShader("basic", "../DustRiders/basic.vert", "../DustRiders/basic.frag");

	std::vector<Entity *> entityList;
	//entityList.reserve(465);
	//physics.transformList.reserve(465);
	//for (int i = 0; i < 465; i++)
	//{
	//	entityList.emplace_back(new Entity());
	//	entityList.back()->transform = physics.transformList[i];

	//	entityList.back()->model = cubeModel;
	//	entityList.back()->shaderProgram = basicShader;
	//}

	// camera.setFocusEntity(entityList[0]);

	// glfwSetFramebufferSizeCallback(window., framebuffer_size_callback);

	JoystickHandler::addJS(GLFW_JOYSTICK_1);

	Vehicle v(physics);
	v.initVehicle();

	//{
	//	entityList.emplace_back(new Entity());
	//	physics.transformList.emplace_back(new Transform());
	//	entityList.back()->transform = physics.transformList.back();
	//	entityList.back()->model = cubeModel;
	//	entityList.back()->shaderProgram = basicShader;
	//}

	double lastTime = 0.0f;

	while (!window.shouldClose())
	{
		auto t = glfwGetTime();
		if (t - lastTime > 0.0167) { // FIXME: Set program to run at 60 FPS, there's definitely a better way to do this using real clock time

			// Game Section
			// processInput(window.get);

			glfwPollEvents();

			v.stepPhysics();
			physics.updatePhysics(t);

			window.swapBuffers();
			renderer.updateRender(entityList, camera);

			overlay.RenderOverlay();

			camera.incrementTheta(0.5f);

			lastTime = t;
		}
	}

	overlay.Cleanup();

	glfwTerminate();
	return 0;
}
