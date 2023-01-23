#include "Overlay.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

Overlay::Overlay() {
	lastTime = glfwGetTime();
}

void Overlay::RenderOverlay() {
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
}

void Overlay::Cleanup() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}