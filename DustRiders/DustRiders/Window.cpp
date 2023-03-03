#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

// ---------------------------
// static function definitions
// ---------------------------

void Window::keyMetaCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	CallbackInterface *callbacks = static_cast<CallbackInterface *>(glfwGetWindowUserPointer(window));
	callbacks->keyCallback(key, scancode, action, mods);
}

void Window::mouseButtonMetaCallback(GLFWwindow *window, int button, int action, int mods)
{
	CallbackInterface *callbacks = static_cast<CallbackInterface *>(glfwGetWindowUserPointer(window));
	callbacks->mouseButtonCallback(button, action, mods);
}

void Window::cursorPosMetaCallback(GLFWwindow *window, double xpos, double ypos)
{
	CallbackInterface *callbacks = static_cast<CallbackInterface *>(glfwGetWindowUserPointer(window));
	callbacks->cursorPosCallback(xpos, ypos);
}

void Window::scrollMetaCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	CallbackInterface *callbacks = static_cast<CallbackInterface *>(glfwGetWindowUserPointer(window));
	callbacks->scrollCallback(xoffset, yoffset);
}

void Window::windowSizeMetaCallback(GLFWwindow *window, int width, int height)
{
	CallbackInterface *callbacks = static_cast<CallbackInterface *>(glfwGetWindowUserPointer(window));
	callbacks->windowSizeCallback(width, height);
}

// ----------------------
// non-static definitions
// ----------------------

Window::Window(
	std::shared_ptr<CallbackInterface> callbacks,
	const char *title, GLFWmonitor *monitor, GLFWwindow *share)
	: window(nullptr), callbacks(callbacks)
{
	// specify OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for mac?
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
	aspectRatio = float(vidMode->width) / float(vidMode->height);
#ifdef _DEBUG
	window = std::unique_ptr<GLFWwindow, WindowDeleter>(glfwCreateWindow(1422, 800, title, NULL, NULL));
#else
	window = std::unique_ptr<GLFWwindow, WindowDeleter>(glfwCreateWindow(vidMode->width, vidMode->height, title, monitor, share));
#endif // DEBUG
	if (window == nullptr)
	{
		std::cout << ("WINDOW failed to create GLFW window");
		throw std::runtime_error("Failed to create GLFW window.");
	}
	glfwMakeContextCurrent(window.get());

	// initialize OpenGL extensions for the current context (this window)
	/*GLenum err = glewInit();
	if (err != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW");
	}*/
	// initialize OpenGL extensions for the current context (this window)
	if (!gladLoadGL())
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetWindowSizeCallback(window.get(), defaultWindowSizeCallback);

	if (callbacks != nullptr)
	{
		connectCallbacks();
	}

	// Standard ImGui/GLFW middleware
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

Window::Window(const char *title, GLFWmonitor *monitor, GLFWwindow *share)
	: Window(nullptr, title, monitor, share)
{
}

void Window::connectCallbacks()
{
	// set userdata of window to point to the object that carries out the callbacks
	glfwSetWindowUserPointer(window.get(), callbacks.get());

	// bind meta callbacks to actual callbacks
	glfwSetKeyCallback(window.get(), keyMetaCallback);
	/*
	* Below two lines commented out otherwise ImGui can't get a hold of mouse
	*/
	//glfwSetMouseButtonCallback(window.get(), mouseButtonMetaCallback);
	//glfwSetCursorPosCallback(window.get(), cursorPosMetaCallback);
	glfwSetScrollCallback(window.get(), scrollMetaCallback);
	glfwSetWindowSizeCallback(window.get(), windowSizeMetaCallback);
}

void Window::setCallbacks(std::shared_ptr<CallbackInterface> callbacks_)
{
	callbacks = callbacks_;
	connectCallbacks();
}

glm::ivec2 Window::getPos() const
{
	int x, y;
	glfwGetWindowPos(window.get(), &x, &y);
	return glm::ivec2(x, y);
}

glm::ivec2 Window::getSize() const
{
	int w, h;
	glfwGetWindowSize(window.get(), &w, &h);
	return glm::ivec2(w, h);
}