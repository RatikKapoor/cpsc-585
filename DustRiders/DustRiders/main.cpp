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

CarAction currentAction = CarAction::IDLE;

class DustRidersWindowCallbacks : public CallbackInterface
{
	virtual void keyCallback(int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			// Forward pressed
			currentAction = CarAction::ACCEL;
		}
		else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			// Forward released
			currentAction = CarAction::IDLE;
		}
		else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			// Back pressed
			currentAction = CarAction::BRAKE;
		}
		else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			// Back released
			currentAction = CarAction::IDLE;
		}
		else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			currentAction = CarAction::LEFT;
			// Left pressed
		}
		else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			// Left released
			currentAction = CarAction::IDLE;
		}
		else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			// Right pressed
			currentAction = CarAction::RIGHT;
		}
		else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			// Right released
			currentAction = CarAction::IDLE;
		}
	}
	virtual void mouseButtonCallback(int button, int action, int mods) {}
	virtual void cursorPosCallback(double xpos, double ypos) {}
	virtual void scrollCallback(double xoffset, double yoffset) {}
	virtual void windowSizeCallback(int width, int height) { glViewport(0, 0, width, height); }
};

CarAction isBeepBeep(int jsID);
float beepSteer(int jsID);
float beepGas(int jsID);

float beepGas(int jsID)
{
	if (glfwJoystickPresent(jsID))
	{
		int count;
		const float *axis = glfwGetJoystickAxes(jsID, &count);
		return axis[XBOX_L_YAXIS];
	}
	else
	{
		return 0.0f;
	}
}

float beepSteer(int jsID)
{
	if (glfwJoystickPresent(jsID))
	{
		int count;
		const float *axis = glfwGetJoystickAxes(jsID, &count);
		return axis[XBOX_R_XAXIS];
	}
	else
	{
		return 0.0f;
	}
}

CarAction isBeepBeep(int jsID)
{
	if (glfwJoystickPresent(jsID))
	{
		int count;

		const unsigned char *buttons = glfwGetJoystickButtons(jsID, &count);

		const float *axis = glfwGetJoystickAxes(jsID, &count);

		if (buttons[XBOX_RB])
		{
			return ACCEL;
		}
		else if (axis[XBOX_R_XAXIS] > 0.01f)
		{
			return RIGHT;
		}
		else if (axis[XBOX_R_XAXIS] < -0.01f)
		{
			return LEFT;
		}
		else if (buttons[XBOX_LB])
		{
			return BRAKE;
		}
		else
		{
			return IDLE;
		}
	}
	else
	{
		return IDLE;
	}
}

int main()
{
	glfwInit();
	Window window(800, 800, "DustRiders");
	window.setCallbacks(std::make_shared<DustRidersWindowCallbacks>());

	PhysicsSystem physics;
	RenderingSystem renderer;
	Overlay overlay;
	ShaderProgram *basicShader = renderer.compileShader("basic", "../DustRiders/basic.vert", "../DustRiders/basic.frag");

#pragma region Verts
	std::vector<Vertex> cubeVerts{
			Vertex{glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{0.0f, 1.0f, 1.0f}, glm::vec2(0.0f)},
			Vertex{glm::vec3{-0.5f, 0.5f, 0.5f}, glm::vec3{0.0f, 1.0f, 1.0f}, glm::vec2(0.0f)},
			Vertex{glm::vec3{-0.5f, -0.5f, 0.5f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(0.0f)},
			Vertex{glm::vec3{0.5f, -0.5f, 0.5f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(0.0f)},
			Vertex{glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3{1.0f, 0.0f, 1.0f}, glm::vec2(0.0f)},
			Vertex{glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3{1.0f, 0.0f, 1.0f}, glm::vec2(0.0f)},
			Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(0.0f)},
			Vertex{glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(0.0f)}};
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
#pragma endregion

	std::vector<Entity *> entityList;

	JoystickHandler::addJS(GLFW_JOYSTICK_1);

	Vehicle v1(physics);
	{
		// Setup v1
		v1.initVehicle(PxVec3(0.f, 0.5f, 0.f));
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList.back();
		entityList.back()->model = cubeModel;
		entityList.back()->shaderProgram = basicShader;
	}
	Vehicle v2(physics);
	{
		// Setup v2
		v2.initVehicle(PxVec3(-2.f, 0.5f, 0.f));
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList.back();
		entityList.back()->model = cubeModel;
		entityList.back()->shaderProgram = basicShader;
	}
	Vehicle v3(physics);
	{
		// Setup v2
		v3.initVehicle(PxVec3(2.f, 0.5f, 0.f));
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList.back();
		entityList.back()->model = cubeModel;
		entityList.back()->shaderProgram = basicShader;
	}

	// Follow the Player Vehicle
	Camera camera(entityList[0], glm::radians(45.0f), 20.0);

	double lastTime = 0.0f;
	int i = 0;
	while (!window.shouldClose())
	{
		auto t = glfwGetTime();
		if (t - lastTime > 0.0167)
		{
			auto deltaT = t - lastTime;
			// First few renders should be simulated with manual step to avoid objects clipping through ground
			if (i < 15)
			{
				deltaT = (1.f / 60.f);
				i++;
			}

			// Game Section
			glfwPollEvents();

			if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
				v1.stepPhysics(deltaT, beepGas(GLFW_JOYSTICK_1), beepSteer(GLFW_JOYSTICK_1));
			}
			else {
				v1.stepPhysics(deltaT, currentAction);
			}
			auto accel = (double)std::rand() / RAND_MAX * 0.5 + 0.2;
			v2.stepPhysics(deltaT, -accel, 0);
			accel = (double)std::rand() / RAND_MAX * 0.5 + 0.2;
			v3.stepPhysics(deltaT, -accel, 0);
			physics.updatePhysics(deltaT);

			window.swapBuffers();
			renderer.updateRender(entityList, camera);

			overlay.RenderOverlay();

			lastTime = t;
		}
	}

	overlay.Cleanup();

	glfwTerminate();
	return 0;
}
