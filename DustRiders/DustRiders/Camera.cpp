#include "Camera.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(Entity* focus, glm::vec3 offset, float t, float r) 
	: focusEntity(focus)
	, focusOffset(offset)
	, theta(t)
	, radius(r)
{}

void Camera::setFocusEntity(Entity* newFocus) {
	focusEntity = newFocus;
}

glm::mat4 Camera::getView() {
	glm::vec3 eye = getPos();
	glm::vec3 at = focusEntity->transform->position + focusOffset;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::lookAt(eye, at, up);
}

glm::vec3 Camera::getPos() {
	glm::vec3 position = focusEntity->transform->position;
	glm::vec3 direction{ 0.0f, 0.0f, -1.0f };
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	// direction = -(focusEntity->transform->rotation * glm::vec3{ 0.0f, 0.0f, 1.0f });
	direction = glm::rotate(glm::mat4(1.0f), theta, glm::cross(direction, up)) * glm::vec4{ direction, 0.0f };

	return radius * direction + position + focusOffset;
}

void Camera::incrementTheta(float dt) {
	if (theta + (dt / 100.0f) < M_PI_2 && theta + (dt / 100.0f) > -M_PI_2) {
		theta += dt / 100.0f;
	}
}

void Camera::incrementR(float dr) {
	radius -= dr;
}
