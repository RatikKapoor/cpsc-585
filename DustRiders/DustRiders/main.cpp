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
#include "ShaderProgram.h"

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

	std::vector<Entity*> entityList;
	entityList.reserve(465);
	for (int i = 0; i < 465; i++)
	{
		entityList.emplace_back(new Entity());
		entityList.back()->transform = physics.transformList[i];
	}

	glfwInit();
	Window window(800, 800, "DustRiders");

	ShaderProgram basicShader("../DustRiders/basic.vert", "../DustRiders/basic.frag");

	//glfwSetFramebufferSizeCallback(window., framebuffer_size_callback);

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	int currentFps = 0;
	while (!window.shouldClose()) {
		// Game Section
		/*processInput(window);*/

		physics.gScene->simulate(1.f / 60.f);
		physics.gScene->fetchResults(true);

		auto position = physics.getPosition();

		window.swapBuffers();
		glfwPollEvents();

		// Rendering Objects
		glEnable(GL_FRAMEBUFFER_SRGB);
		glClearColor(0.5f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		// Development Peripherals Section

		// Framerate calculations
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			currentFps = nbFrames;
			nbFrames = 0;
			lastTime += 1.0;
		}


		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Putting the text-containing window in the top-left of the screen.
		ImGui::SetNextWindowPos(ImVec2(5, 5));
		// Setting flags
		ImGuiWindowFlags textWindowFlags =
			ImGuiWindowFlags_NoMove |			// text "window" should not move
			ImGuiWindowFlags_NoResize |			// should not resize
			ImGuiWindowFlags_NoCollapse |		// should not collapse
			ImGuiWindowFlags_NoSavedSettings |	// don't want saved settings mucking things up
			ImGuiWindowFlags_AlwaysAutoResize | // window should auto-resize to fit the text
			// ImGuiWindowFlags_NoBackground |		// window should be transparent; only the text should be visible
			ImGuiWindowFlags_NoDecoration | // no decoration; only the text should be visible
			ImGuiWindowFlags_NoTitleBar;	// no title; only the text should be visible
		// Begin a new window with these flags. (bool *)0 is the "default" value for its argument.
		ImGui::Begin("DustRiders", (bool*)0, textWindowFlags);
		ImGui::Text("FPS: %i", currentFps);
		// End the window.
		ImGui::End();
		// Render window
		ImGui::Render();										// Render the ImGui window
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Some middleware thing
		// End ImGui
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}