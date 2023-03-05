#include "Overlay.h"
#include "StateHandler.h"
#include "InputHandler.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>

Overlay::Overlay()
{
	lastTime = glfwGetTime();
}

void Overlay::RenderOverlay(StateHandler::GameState gameState, std::vector<Entity*> entities, EntityComponentSystem* ecs)
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
	switch (gameState)
	{
	case StateHandler::GameState::StartMenu:
		ImGui::Text("Press forward to start playing");
		break;
	case StateHandler::GameState::Playing:
		ImGui::Text("Playing game");
		break;
	case StateHandler::GameState::PauseMenu:
		ImGui::Text("Paused.");
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

		if (!ecs->doesKeyExist(selectedEntity))
			selectedEntity = "";

		if (selectedEntity != "") {
			ImGui::Text("X: %f", ecs->get(selectedEntity)->transform->position.x);
			ImGui::Text("Y: %f", ecs->get(selectedEntity)->transform->position.y);
			ImGui::Text("Z: %f", ecs->get(selectedEntity)->transform->position.z);
		}
	}

	// Current buttons being pressed
	// ImGui::Text("%s", Joystick::getButtonStr(GLFW_JOYSTICK_1).c_str());
	// ImGui::Text("%s", Joystick::getTriggerStr(GLFW_JOYSTICK_1).c_str());
	// ImGui::Text("%s", Joystick::getStickStr(GLFW_JOYSTICK_1).c_str());

	std::map<int, Joystick> tMap = JoystickHandler::getJSMap();

	if (ImGui::CollapsingHeader("Controllers")) {
		ImGui::Text("NumJS: %d", tMap.size());

		auto jsItr = tMap.begin();
		while (jsItr != tMap.end())
		{
			jsItr->second.updateAllInputs();
			ImGui::Text("All Buttons: %s", jsItr->second.buttonListRaw().c_str());
			ImGui::Text("New Buttons: %s", jsItr->second.buttonList().c_str());
			ImGui::Text("%s", jsItr->second.triggerList().c_str());
			ImGui::Text("%s", jsItr->second.axisList().c_str());
			jsItr++;
		}
	}


	// End the window.
	ImGui::End();
	// Render window
	ImGui::Render();																				// Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Some middleware thing

	// End ImGui
}

void Overlay::RenderPause(int windowHeight, int windowWidth)
{
	bool isKeyboard = JoystickHandler::getFirstJS().isPseudo();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// Putting the text-containing window in the top-left of the screen.

	// Setting flags
	ImGuiWindowFlags textWindowFlags =
		ImGuiWindowFlags_NoMove |						// text "window" should not move
		ImGuiWindowFlags_NoResize |					// should not resize
		ImGuiWindowFlags_NoCollapse |				// should not collapse
		ImGuiWindowFlags_NoSavedSettings |	// don't want saved settings mucking things up
		ImGuiWindowFlags_AlwaysAutoResize | // window should auto-resize to fit the text
		ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
		ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
		ImGuiWindowFlags_NoTitleBar;				// no title; only the text should be visible

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.75, windowHeight / 2));
	ImGui::Begin("DustRiderTitle", (bool*)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 100.0f);
	ImGui::Text("DustRiders");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.7, windowHeight * 1.05));
	ImGui::Begin("ScoreText", (bool*)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 180.0f);

	if (isKeyboard)
	{
		ImGui::Text("Press Enter to Resume Game");
		ImGui::Text("Press Escape to Exit Game");
		ImGui::End();
	}
	else
	{
		ImGui::Text("Press START to Resume Game");
		ImGui::Text("Press X to Exit");
		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.1, windowHeight * 1.4));
	ImGui::Begin("ControlInput", (bool*)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 180.0f);
	if (isKeyboard)
	{
		ImGui::Text("W to Forward");
		ImGui::Text("S to Backward");
		ImGui::Text("A to Left");
		ImGui::Text("D to Right");
		ImGui::Text("Left Shift to Reverse");
		ImGui::Text("Enter to Fire Weapon");
		ImGui::Text("ESC to open menu");
		ImGui::End();
	}
	else
	{
		ImGui::Text("RT to Accelerate");
		ImGui::Text("LT to Brake");
		ImGui::Text("LB to Reverse");
		ImGui::Text("LJS to Steer");
		ImGui::Text("START to toggle menu and pause");
		ImGui::End();
	}

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::RenderMenu(int windowHeight, int windowWidth)
{
	bool isKeyboard = JoystickHandler::getFirstJS().isPseudo();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// Putting the text-containing window in the top-left of the screen.

	// Setting flags
	ImGuiWindowFlags textWindowFlags =
		ImGuiWindowFlags_NoMove |						// text "window" should not move
		ImGuiWindowFlags_NoResize |					// should not resize
		ImGuiWindowFlags_NoCollapse |				// should not collapse
		ImGuiWindowFlags_NoSavedSettings |	// don't want saved settings mucking things up
		ImGuiWindowFlags_AlwaysAutoResize | // window should auto-resize to fit the text
		ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
		ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
		ImGuiWindowFlags_NoTitleBar;				// no title; only the text should be visible

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.75, windowHeight / 2));
	ImGui::Begin("DustRiderTitle", (bool*)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 100.0f);
	ImGui::Text("DustRiders");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.7, windowHeight * 1.05));
	ImGui::Begin("ScoreText", (bool*)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 180.0f);

	if (isKeyboard)
	{
		ImGui::Text("Press Enter to Start Game");
		ImGui::Text("Press Escape to Exit");
		ImGui::End();
	}
	else
	{
		ImGui::Text("Press A to Start Game");
		ImGui::Text("Press X to Exit");
		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.1, windowHeight * 1.4));
	ImGui::Begin("ControlInput", (bool*)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 180.0f);
	if (isKeyboard)
	{
		ImGui::Text("W to Forward");
		ImGui::Text("S to Backward");
		ImGui::Text("A to Left");
		ImGui::Text("D to Right");
		ImGui::Text("Left Shift to Reverse");
		ImGui::Text("Enter to Fire Weapon");
		ImGui::Text("ESC to open menu");
		ImGui::End();
	}
	else
	{
		ImGui::Text("RT to Accelerate");
		ImGui::Text("LT to Brake");
		ImGui::Text("LB to Reverse");
		ImGui::Text("LJS to Steer");
		ImGui::Text("START to toggle menu and pause");
		ImGui::End();
	}

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::RenderWin(int windowHeight, int windowWidth)
{
	bool isKeyboard = JoystickHandler::getFirstJS().isPseudo();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// Putting the text-containing window in the top-left of the screen.

	// Setting flags
	ImGuiWindowFlags textWindowFlags =
		ImGuiWindowFlags_NoMove |						// text "window" should not move
		ImGuiWindowFlags_NoResize |					// should not resize
		ImGuiWindowFlags_NoCollapse |				// should not collapse
		ImGuiWindowFlags_NoSavedSettings |	// don't want saved settings mucking things up
		ImGuiWindowFlags_AlwaysAutoResize | // window should auto-resize to fit the text
		ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
		ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
		ImGuiWindowFlags_NoTitleBar;				// no title; only the text should be visible

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.75, windowHeight / 2));
	ImGui::Begin("DustRiderWin", (bool*)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 100.0f);
	ImGui::Text("You Win!");
	ImGui::End();

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::RenderLoss(int windowHeight, int windowWidth)
{
	bool isKeyboard = !JoystickHandler::getFirstJS().isPseudo();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// Putting the text-containing window in the top-left of the screen.

	// Setting flags
	ImGuiWindowFlags textWindowFlags =
		ImGuiWindowFlags_NoMove |						// text "window" should not move
		ImGuiWindowFlags_NoResize |					// should not resize
		ImGuiWindowFlags_NoCollapse |				// should not collapse
		ImGuiWindowFlags_NoSavedSettings |	// don't want saved settings mucking things up
		ImGuiWindowFlags_AlwaysAutoResize | // window should auto-resize to fit the text
		ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
		ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
		ImGuiWindowFlags_NoTitleBar;				// no title; only the text should be visible

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.75, windowHeight / 2));
	ImGui::Begin("DustRiderLose", (bool*)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 100.0f);
	ImGui::Text("You Lose!");
	ImGui::End();

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
