#include "Overlay.h"
#include "InputHandler.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>

Overlay::Overlay()
{
	lastTime = glfwGetTime();

	this->ecs = EntityComponentSystem::getInstance();
}

void Overlay::RenderOverlay(std::vector<Entity*> entities)
{
	// Framerate calculations
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0)
	{ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		// printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		currentFps = nbFrames;
		nbFrames = 0;
		lastTime += 1.0;
	}

	JoystickHandler jsHand;

	// ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	// Putting the text-containing window in the top-left of the screen.
	ImGui::SetNextWindowPos(ImVec2(5, 5));
	// Setting flags
	ImGuiWindowFlags textWindowFlags =
		ImGuiWindowFlags_NoMove |						// text "window" should not move
		ImGuiWindowFlags_NoResize |					// should not resize
		ImGuiWindowFlags_NoCollapse |				// should not collapse
		ImGuiWindowFlags_NoSavedSettings |	// don't want saved settings mucking things up
		ImGuiWindowFlags_AlwaysAutoResize | // window should auto-resize to fit the text
		// ImGuiWindowFlags_NoBackground |		// window should be transparent; only the text should be visible
		ImGuiWindowFlags_NoDecoration | // no decoration; only the text should be visible
		ImGuiWindowFlags_NoTitleBar;		// no title; only the text should be visible
// Begin a new window with these flags. (bool *)0 is the "default" value for its argument.
	ImGui::Begin("DustRiders", (bool*)0, textWindowFlags);
	ImGui::Text("FPS: %i", currentFps);
	switch (0)
	{
	case 0:
		ImGui::Text("Press forward to start playing");
		break;
	case 1:
		ImGui::Text("Playing game");
		break;
	case 2:
		ImGui::Text("Game won!");
		break;
	case 3:
		ImGui::Text("Game lost :(");
		break;
	default:
		break;
	}

	if (ImGui::CollapsingHeader("Entity Introspection")) {
		if (ImGui::BeginCombo("Entities", &selectedEntity[0])) {
			for (auto& entity : entities) {
				if (ImGui::Selectable(&entity->name[0])) {
					selectedEntity = entity->name;
				}
				ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (selectedEntity != "") {
			ImGui::Text("Data: %s", ecs->get(selectedEntity));
		}
	}

	// Current buttons being pressed
	// ImGui::Text("%s", Joystick::getButtonStr(GLFW_JOYSTICK_1).c_str());
	// ImGui::Text("%s", Joystick::getTriggerStr(GLFW_JOYSTICK_1).c_str());
	// ImGui::Text("%s", Joystick::getStickStr(GLFW_JOYSTICK_1).c_str());

	std::map<int, Joystick> tMap = JoystickHandler::getJSMap();

	if (ImGui::CollapsingHeader("Controllers")) {
		ImGui::Text("NumJS: %d", tMap.size());

		if (tMap.size() == 0) {
			ImGui::Text("Buttons: N/A");
			ImGui::Text("Triggers N/A");
			ImGui::Text("Axes N/A");
		}
		else {
			auto jsItr = tMap.begin();
			while (jsItr != tMap.end())
			{
				jsItr->second.updateInputs();
				ImGui::Text("Buttons: %s", jsItr->second.buttonList().c_str());
				ImGui::Text("%s", jsItr->second.triggerList().c_str());
				ImGui::Text("%s", jsItr->second.axisList().c_str());
				jsItr++;
			}
		}
	}


	// End the window.
	ImGui::End();
	// Render window
	ImGui::Render();																				// Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Some middleware thing

	// End ImGui
}

void Overlay::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
