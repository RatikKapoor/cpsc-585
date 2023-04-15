#include "Overlay.h"

Overlay::Overlay()
{
	lastTime = timeKeeper.getCurrentTime();

	std::string imagePath;

	// Storm images
	for (int i = 0; i < 14; i++) {
		imagePath = "./assets/stormImages/frame" + std::to_string(i) + ".png";
		stormImagesMap[imagePath] = (ImTextureID)loadFrameTexture(imagePath.c_str());
	}

	// Battery charge images
	for (int i = 0; i < 5; i++) {
		imagePath = "./assets/playerDataImages/batteryCharge" + std::to_string(i) + ".png";
		batteryImagesMap[imagePath] = (ImTextureID)loadFrameTexture(imagePath.c_str());
	}

	// Canvas coloured images
	canvasImageRed = (ImTextureID)loadFrameTexture("./assets/playerDataImages/playerInformationCanvasRed.png");
	canvasImageGreen = (ImTextureID)loadFrameTexture("./assets/playerDataImages/playerInformationCanvasGreen.png");
	canvasImageBlue = (ImTextureID)loadFrameTexture("./assets/playerDataImages/playerInformationCanvasBlue.png");
	canvasImageBlack = (ImTextureID)loadFrameTexture("./assets/playerDataImages/playerInformationCanvas.png");
	canvasTintImage = (ImTextureID)loadFrameTexture("./assets/playerDataImages/playerInformationCanvasTint.png");

	stormFrameCounter = 0;
}

void Overlay::LoadingContent(int windowWidth, int windowHeight)
{
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

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.77, windowHeight * 0.9));
	ImGui::Begin("DustRiderTitle", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 400.0f);
	ImGui::Text("Loading content...");
	ImGui::End();

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return;
}

void Overlay::RenderOverlay(StateHandler::GameState gameState, StateHandler::GameState prevGameState, std::vector<Entity *> entities, EntityComponentSystem *ecs)
{
#ifdef _DEBUG
	// Framerate calculations
	double currentTime = timeKeeper.getCurrentTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0)
	{ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		// printf("%f ms/frame\n", 1000.0 / double(nbFrames));
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
			ImGuiWindowFlags_NoMove |						// text "window" should not move
			ImGuiWindowFlags_NoResize |					// should not resize
			ImGuiWindowFlags_NoCollapse |				// should not collapse
			ImGuiWindowFlags_NoSavedSettings |	// don't want saved settings mucking things up
			ImGuiWindowFlags_AlwaysAutoResize | // window should auto-resize to fit the text
			// ImGuiWindowFlags_NoBackground |		// window should be transparent; only the text should be visible
			ImGuiWindowFlags_NoDecoration | // no decoration; only the text should be visible
			ImGuiWindowFlags_NoTitleBar;		// no title; only the text should be visible
																			// Begin a new window with these flags. (bool *)0 is the "default" value for its argument.
	ImGui::Begin("DustRiders", (bool *)0, textWindowFlags);
	ImGui::Text("FPS: %i", currentFps);
	ImGui::Text("Previous State: %s", std::string(prevGameState).c_str());
	ImGui::Text("Current State: %s", std::string(gameState).c_str());
	auto carRef = ((Vehicle*)ecs->get("car"));
	ImGui::Text("Current Chunk: %s", ChunkHandler::getChunkForZ(carRef->transform->position.z).c_str());
	auto& carPosRef = carRef->transform->position;
	ImGui::Text("Normalized Location: %f %f %f", carPosRef.x, carPosRef.y, std::fmod(carPosRef.z + HALF_CHUNK_SIZE, CHUNK_SIZE));
	if (ImGui::CollapsingHeader("Entity Introspection"))
	{
		if (ImGui::BeginCombo("Entities", &selectedEntity[0]))
		{
			for (auto &entity : entities)
			{
				if (ImGui::Selectable(&entity->name[0]))
				{
					selectedEntity = entity->name;
				}
				ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (selectedEntity != "")
		{
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

	if (ImGui::CollapsingHeader("Controllers"))
	{
		ImGui::Text("NumJS: %d", tMap.size());

		auto jsItr = tMap.begin();
		while (jsItr != tMap.end())
		{
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
#endif // _DEBUG
}


std::string battery(int c) {
	if (c == 100) { return "./assets/playerDataImages/batteryCharge4.png"; }
	if (c >= 75) { return "./assets/playerDataImages/batteryCharge3.png"; }
	if (c >= 50) { return "./assets/playerDataImages/batteryCharge2.png"; }
	if (c >= 25) { return "./assets/playerDataImages/batteryCharge1.png"; }
	return "./assets/playerDataImages/batteryCharge0.png";
}

std::string positionFancy(int x) {
	switch (x)
	{
	case 1:
		return "st";
		break;
	case 2:
		return "nd";
		break;
	case 3:
		return "rd";
		break;
	default:
		return "th";
		break;
	}
}

ImTextureID Overlay::loadFrameTexture(std::string filepath) {
	int width, height, channels;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (!data) {
		LogWriter::log("Failed to load texture from file " + filepath);
		return nullptr;
	}

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	return (ImTextureID)(intptr_t)texture_id;
}

void Overlay::RenderStorm(int frameWidth, int frameHeight, int screenWidth, int screenHeight)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	std::string imagePath = "./assets/stormImages/frame" + std::to_string(stormFrameCounter % 14) + ".png";

	ImGui::SetNextWindowPos(ImVec2(0, screenHeight - frameHeight));
	ImGui::Begin("storm", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Image(stormImagesMap[imagePath], ImVec2(frameWidth, frameHeight));

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (nbFrames % 15 == 0)
		stormFrameCounter++;
}

void Overlay::RenderCanvas(int colour, int screenWidth, int screenHeight, float scaler, float position) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(screenWidth - 150 * scaler, screenHeight * position - 100 * scaler));
	ImGui::Begin("canvas", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
	ImTextureID canvasImage;
	switch (colour)
	{
	case 2:
		canvasImage = canvasImageGreen;
		break;
	case 4:
		canvasImage = canvasImageRed;
		break;
	case 3:
		canvasImage = canvasImageBlue;
		break;
	default:
		canvasImage = canvasImageBlack;
		break;
	}
	ImGui::Image(canvasImage, ImVec2(150 * scaler, 100 * scaler));

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::RenderCanvasTint(int screenWidth, int screenHeight, float scaler, float position) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(screenWidth - 150 * scaler, screenHeight * position - 100 * scaler));
	ImGui::Begin("tint", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Image(canvasTintImage, ImVec2(150 * scaler, 100 * scaler));

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::RenderSpeedometer(float currentSpeed, int screenWidth, int screenHeight, float scaler, float position) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(screenWidth - 130 * scaler, screenHeight * position - 80 * scaler));
	ImGui::SetNextWindowSize(ImVec2(screenWidth * 0.10f, screenHeight * 0.10f));
	ImGui::Begin("Speedometer", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

	ImGui::SetWindowFontScale(scaler);
	ImGui::Text("%.0f km/h", currentSpeed * 3.6);
	ImGui::End();

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Overlay::RenderCharge(int charge, int screenWidth, int screenHeight, float scaler, float position) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(screenWidth - 135 * scaler, screenHeight * position - 35 * scaler));
	ImGui::Begin("Charge", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
	ImGui::Image(batteryImagesMap[battery(charge)], ImVec2(120 * scaler, 20 * scaler));

	ImGui::End();

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Overlay::RenderPlace(int place, int screenWidth, int screenHeight, float scaler, float position) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(screenWidth - 40 * scaler, screenHeight * position - 80 * scaler));
	ImGui::SetNextWindowSize(ImVec2(screenWidth * 0.20, screenHeight * 0.10));
	ImGui::Begin("place", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

	ImGui::SetWindowFontScale(scaler);
	ImGui::Text("%d%s", place, positionFancy(place).c_str());

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}




void Overlay::RenderPause(StateHandler::GameState prevState, int windowHeight, int windowWidth)
{
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
	ImGui::Begin("DustRiderTitle", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(5.0f);
	ImGui::Text("DustRiders");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.9, windowHeight * 1.6));
	ImGui::Begin("ScoreText", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(3.0f);

	if (prevState != StateHandler::GameState::Playing)
	{
		ImGui::Text("Press ENTER or Right D-Pad to start.");
		ImGui::Text("Press X or X Button to quit.");
		ImGui::End();
	}
	else
	{
		ImGui::Text("Press ESC or START to resume.");
		ImGui::Text("Press X or X Button to quit.");
		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.1, windowHeight * 1.3));
	ImGui::Begin("ControlInput", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 180.0f);

	ImGui::Text("RT to accelerate, LT to brake/reverse (Controller)");
	ImGui::Text("Left JS to steer (Controller)");
	ImGui::Text("A to Shoot (Controller)");
	ImGui::Text("Y to reset (Controller)");
	ImGui::Text("WASD to Steer (Keyboard)");
	ImGui::Text("SPACE to Shoot (Keyboard)");
	ImGui::Text("BACKSPACE to reset (Keyboard)");
	ImGui::Text("ESC or START to toggle pause");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 1.6, windowHeight * 0.6));
	ImGui::Begin("CarColours", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 270.0f);
	ImGui::Text("Player 1: ");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(58, 91, 50, 255));
	ImGui::Text("Green");
	ImGui::PopStyleColor();
	ImGui::Text("Player 2: ");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(136, 64, 47, 255));
	ImGui::Text("Red");
	ImGui::PopStyleColor();
	ImGui::Text("Player 3: ");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(44, 42, 102, 255));
	ImGui::Text("Blue");
	ImGui::PopStyleColor();
	ImGui::End();

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::RenderMenu(int windowHeight, int windowWidth)
{

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
			ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
			ImGuiWindowFlags_AlwaysAutoResize | // window should auto-resize to fit the text
			ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
			ImGuiWindowFlags_NoTitleBar;				// no title; only the text should be visible

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.75, windowHeight / 2));
	ImGui::Begin("DustRiderTitle", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 100.0f);
	ImGui::Text("DustRiders");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.7, windowHeight * 1.05));
	ImGui::Begin("ScoreText", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 180.0f);
	ImGui::Text("Press ENTER or Right D-Pad to Start Game");
	ImGui::Text("Press X or X Button to Exit");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.1, windowHeight * 1.4));
	ImGui::Begin("ControlInput", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 180.0f);
	ImGui::Text("RT to accelerate, LT to brake/reverse (Controller)");
	ImGui::Text("Left JS to steer (Controller)");
	ImGui::Text("A to Shoot (Controller)");
	ImGui::Text("WASD to Steer (Keyboard)");
	ImGui::Text("SPACE to Shoot (Keyboard)");
	ImGui::Text("ESC or START to toggle pause");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 1.6, windowHeight * 1.6));
	ImGui::Begin("CarColours", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 270.0f);
	ImGui::Text("Player 1: ");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(58, 91, 50, 255));
	ImGui::Text("Green");
	ImGui::PopStyleColor();
	ImGui::Text("Player 2: ");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(136, 64, 47, 255));
	ImGui::Text("Red");
	ImGui::PopStyleColor();
	ImGui::Text("Player 3: ");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(44, 42, 102, 255));
	ImGui::Text("Blue");
	ImGui::PopStyleColor();
	ImGui::End();

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::RenderWin(int windowHeight, int windowWidth, StateHandler::GameState player)
{

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
	ImGui::Begin("DustRiderWin", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 100.0f);
	ImGui::Text("%s", player.operator std::string().c_str());

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.7, windowHeight * 1.2));
	ImGui::Begin("ScoreText", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(3.0f);

	ImGui::Text("Press ENTER or Right D-Pad to Play Again");
	ImGui::Text("Press X or X Button to quit");
	ImGui::End();

	ImGui::End();

	ImGui::Render(); // Render the ImGui window
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Overlay::RenderLoss(int windowHeight, int windowWidth)
{
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
	ImGui::Begin("DustRiderLose", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(windowHeight / 100.0f);
	ImGui::Text("You Lose!");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(windowWidth * 0.7, windowHeight * 1.2));
	ImGui::Begin("ScoreText", (bool *)0, textWindowFlags);
	ImGui::SetWindowFontScale(3.0f);

	ImGui::Text("Press ENTER or Right D-Pad to Play Again");
	ImGui::Text("Press X or X Button to quit DustRiders");
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
