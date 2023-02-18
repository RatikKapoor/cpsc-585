#include <iostream>

#include <PxPhysicsAPI.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <memory>

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
#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "MusicBuffer.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

CarAction currentAction = CarAction::IDLE;
bool hasStarted = true;
int gameState = 0; // 0 - need to start, 1 - playing, 2 - won, 3 - lost

class DustRidersWindowCallbacks : public CallbackInterface
{
public:
	DustRidersWindowCallbacks(Window &window) : window(window) {}
	virtual void keyCallback(int key, int scancode, int action, int mods)
	{
		Joystick &js = JoystickHandler::getFirstJS();
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			gameState = -1;
		}
		if (!js.isPseudo())
		{
			return;
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			// Forward pressed
			if (!hasStarted)
			{
				hasStarted = true;
				gameState = 1;
			}
			js.pressW();
		}
		else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			// Forward released
			js.releaseW();
		}
		else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			// Back pressed
			js.pressS();
		}
		else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			// Back released
			js.releaseS();
		}
		else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			js.pressA();
			// Left pressed
		}
		else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			// Left released
			js.releaseA();
		}
		else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			// Right pressed
			js.pressD();
		}
		else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			// Right released
			js.releaseD();
		}
		else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		{
			// Right pressed
			js.pressLeftShift();
		}
		else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		{
			// Right released
			js.releaseLeftShift();
		}
		else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			js.pressEnter();
		}
		else if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
		{
			js.releaseEnter();
		}

	}

	virtual void mouseButtonCallback(int button, int action, int mods) {}
	virtual void cursorPosCallback(double xpos, double ypos) {}
	virtual void scrollCallback(double xoffset, double yoffset) {}
	virtual void windowSizeCallback(int width, int height) { glViewport(0, 0, width, height); }

	Window &window;

protected:
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

		if (axis[XBOX_L_YAXIS] < -0.1 && !hasStarted)
		{
			hasStarted = true;
			gameState = 1;
		}

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

	Joystick keyboardJS;

	bool inputIsKeyboard = true;

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		JoystickHandler::addJS(GLFW_JOYSTICK_1);
		inputIsKeyboard = false;
	}
	else
	{
		JoystickHandler::addJS(keyboardJS);
		inputIsKeyboard = true;
	}

	Window window("DustRiders", glfwGetPrimaryMonitor());
	window.setCallbacks(std::make_shared<DustRidersWindowCallbacks>(std::ref(window)));

	PhysicsSystem physics;
	RenderingSystem renderer;
	Overlay overlay;
	ShaderProgram *carShader = renderer.compileShader("car", "./car.vert", "./car.frag");
	ShaderProgram *basicShader = renderer.compileShader("basic", "./basic.vert", "./basic.frag");

	// To load in a model, just use "loadModelFromFile". Textures are handled automatically.
	Model *testCarModel = renderer.loadModelFromFile("TestCar", "./assets/models/better-car-v2.obj");
	Model *testRock = renderer.loadModelFromFile("TestRock", "./assets/models/test-obstacle-rock.obj");
	Model *groundPlane = renderer.loadModelFromFile("GroundPlane", "./assets/models/ground-plane.obj");

	std::vector<Entity *> entityList;

	Vehicle v1(physics);
	{
		// Setup v1
		v1.initVehicle(PxVec3(0.f, 0.5f, 0.f));
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList.back();
		entityList.back()->model = testCarModel;
		entityList.back()->shaderProgram = basicShader;
		entityList.back()->scale = glm::vec3{1.f, 1.f, 1.f};
	}

	// Adds ground plane
	entityList.emplace_back(new Entity());
	entityList.back()->transform = new Transform();
	entityList.back()->transform->position = glm::vec3(0.f, -0.5f, 0.f);
	entityList.back()->model = groundPlane;
	entityList.back()->shaderProgram = basicShader;
	entityList.back()->scale = glm::vec3{1.f, 1.f, 1.f};

	Vehicle v2(physics);
	{
		// Setup v2
		v2.initVehicle(PxVec3(-2.f, 0.5f, 0.f));
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList.back();
		entityList.back()->model = testCarModel;
		entityList.back()->shaderProgram = basicShader;
		entityList.back()->scale = glm::vec3{1.f, 1.f, 1.f};
	}
	Vehicle v3(physics);
	{
		// Setup v2
		v3.initVehicle(PxVec3(2.f, 0.5f, 0.f));
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList.back();
		entityList.back()->model = testCarModel;
		entityList.back()->shaderProgram = basicShader;
		entityList.back()->scale = glm::vec3{1.f, 1.f, 1.f};
	}

	// Follow the Player Vehicle
	Camera camera(entityList[0], glm::vec3{0.0f, 0.0f, -3.0f}, glm::radians(60.0f), 50.0);

	int obstacleCount = 0;
	for (float dist = 0; dist <= 1000.5f; dist += 10.0f)
	{
		entityList.emplace_back(new Entity());
		entityList.back()->transform = new Transform();

		if (obstacleCount % 2 == 0)
		{
			entityList.back()->transform->position = glm::vec3{-7.0f, 0.0f, dist};
		}
		else
		{
			entityList.back()->transform->position = glm::vec3{7.0f, 0.0f, dist};
		}

		entityList.back()->model = testRock;
		entityList.back()->shaderProgram = basicShader;
		entityList.back()->scale = glm::vec3{2.0f, 2.0f, 2.0f};

		obstacleCount++;
	}

	SoundDevice *mysounddevice = SoundDevice::get();
	uint32_t /*ALuint*/ sound1 = SoundBuffer::get()->addSoundEffect("../sound/blessing.ogg");

	SoundSource mySpeaker;
	alSourcePlay(mySpeaker.Play(sound1));

	MusicBuffer myMusic("../sound/TownTheme.wav");
	std::cout << "playing town theme music...\n";
	myMusic.Play();

	ALint state = AL_PLAYING;
	std::cout << "playing sound\n";

	double lastTime = 0.0f;
	int i = 0;
	while (gameState > -1)
	{
		if (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{

			myMusic.UpdateBufferStream();

			alGetSourcei(myMusic.getSource(), AL_SOURCE_STATE, &state);
		}

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
			JoystickHandler::updateAll();

			auto gasValue = beepGas(GLFW_JOYSTICK_1);
			auto steerValue = beepSteer(GLFW_JOYSTICK_1);

			if (hasStarted)
			{
				// Vehicle physics
				//	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
				//{
				v1.stepPhysics(deltaT, JoystickHandler::getFirstJS());
				//}
				// else
				//	{
				//	v1.stepPhysics(deltaT, currentAction);
				//}
				auto accel = (double)std::rand() / RAND_MAX * 0.5 + 0.2;
				v2.stepPhysics(deltaT, -accel, 0);
				accel = (double)std::rand() / RAND_MAX * 0.5 + 0.2;
				v3.stepPhysics(deltaT, -accel, 0);

				// Win condition
				if (physics.transformList[0]->position.z - physics.transformList[1]->position.z > 50.f)
				{
					// Game won
					gameState = 2;
				}
				else if (physics.transformList[0]->position.z - physics.transformList[1]->position.z < -50.f)
				{
					// Game lost
					gameState = 3;
				}
			}
			physics.updatePhysics(deltaT);

			window.swapBuffers();
			renderer.updateRender(entityList, camera, window.getAspectRatio());

			overlay.RenderOverlay(gameState);

			lastTime = t;
		}
	}
	window.close();

	overlay.Cleanup();

	glfwTerminate();
	return 0;
}
