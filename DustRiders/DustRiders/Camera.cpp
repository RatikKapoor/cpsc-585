#include "Camera.h"

#include <iostream>
#include "InputHandler.h"
#include "DebugDefines.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(Entity *focus, glm::vec3 offset, float t, float r)
		: focusEntity(focus), focusOffset(offset), theta(t), radius(r)
{
	lastDir = focusEntity->transform->position;
}

void Camera::setFocusEntity(Entity *newFocus)
{
	focusEntity = newFocus;
}

glm::mat4 Camera::getView()
{
	glm::vec3 eye = getPos();
	glm::vec3 at = focusEntity->transform->position + focusOffset;

#ifndef NO_DEATH
	at.x = 0;
#endif
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::lookAt(eye, at, up);
}

glm::vec3 Camera::getPos()
{
	glm::vec3 newDir = glm::vec3(0.f);
	glm::vec3 position = focusEntity->transform->position;
#ifndef NO_DEATH
	position.x = 0;
	glm::vec3 direction{0.0f, 0.0f, -1.0f};
#else
	glm::vec3 direction = glm::normalize(position);
	const float *analogs = JoystickHandler::getFirstJS().getAnalogs();

	newDir = glm::vec3(analogs[Xbox::Analog::XBOX_R_XAXIS]/2.f, analogs[Xbox::Analog::XBOX_R_YAXIS]/-2.f, analogs[Xbox::Analog::XBOX_R_XAXIS]/2.f);

	direction = direction + lastDir + newDir;
	lastDir = lastDir + newDir;

#endif

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	// direction = -(focusEntity->transform->rotation * glm::vec3{ 0.0f, 0.0f, 1.0f });
	direction = glm::rotate(glm::mat4(1.0f), theta, glm::cross(direction, up)) * glm::vec4{direction, 0.0f};

#ifndef NO_DEATH
	return radius * direction + position + focusOffset;
#else
	return radius + direction + position + focusOffset;

#endif
}

void Camera::incrementTheta(float dt)
{
	if (theta + (dt / 100.0f) < M_PI_2 && theta + (dt / 100.0f) > -M_PI_2)
	{
		theta += dt / 100.0f;
	}
}

void Camera::incrementR(float dr)
{
	radius -= dr;
}
