#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Window.h"
#include "ShaderProgram.h"
#include "Camera.h"

class Callbacks : public CallbackInterface
{

public:
	Callbacks()
		: camera(glm::radians(45.f), glm::radians(45.f), 3.0), aspect(1.0f), rightMouseDown(false), mouseOldX(0.0), mouseOldY(0.0), time(0.0)
	{
	}

	virtual void keyCallback(int key, int scancode, int action, int mods) {}
	virtual void mouseButtonCallback(int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			if (action == GLFW_PRESS)
				rightMouseDown = true;
			else if (action == GLFW_RELEASE)
				rightMouseDown = false;
		}
	}
	virtual void cursorPosCallback(double xpos, double ypos)
	{
		if (rightMouseDown)
		{
			camera.incrementTheta(ypos - mouseOldY);
			camera.incrementPhi(xpos - mouseOldX);
		}
		mouseOldX = xpos;
		mouseOldY = ypos;
	}
	virtual void scrollCallback(double xoffset, double yoffset)
	{
		camera.incrementR(yoffset);
	}
	virtual void windowSizeCallback(int width, int height)
	{
		// The CallbackInterface::windowSizeCallback will call glViewport for us
		CallbackInterface::windowSizeCallback(width, height);
		aspect = float(width) / float(height);
	}

	void viewPipeline(ShaderProgram& sp)
	{
		//auto M = s->getMatrix(time);
		glm::mat4 nM = glm::mat4(1.0);
		glm::mat4 V = camera.getView();
		glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 1000.f);
		GLint location = glGetUniformLocation(sp, "lightPosition");
		glm::vec3 light = camera.getPos();
		glUniform3fv(location, 1, glm::value_ptr(light));
		GLint uniMat = glGetUniformLocation(sp, "M");
		//glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(M));
		uniMat = glGetUniformLocation(sp, "V");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(V));
		uniMat = glGetUniformLocation(sp, "P");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(P));
		uniMat = glGetUniformLocation(sp, "nM");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(nM));

		//GLint shouldUseLightingLoc = glGetUniformLocation(sp, "shouldUseLighting");
		//glUniform1i(shouldUseLightingLoc, s->shouldUseLighting());
	}

	void setTime(double time) { this->time = time; }
	double getTime() { return time; }
	Camera camera;

private:
	bool rightMouseDown;
	float aspect;
	double mouseOldX;
	double mouseOldY;
	double time = 0.0;
};