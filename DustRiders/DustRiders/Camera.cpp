#include "Camera.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(float t, float p, float r) : theta(t), phi(p), radius(r), focusEntity(nullptr) {}

void Camera::setFocusEntity(Entity* body) {
	focusEntity = body;
}

glm::mat4 Camera::getView() {
	glm::vec3 eye = getPos();
	glm::vec3 at(0.0f, 0.0f, 0.0f);
	if (focusEntity != nullptr)
		at = focusEntity->transform->position;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::lookAt(eye, at, up);
}

glm::vec3 Camera::getPos() {
	glm::vec3 position(0.0f, 0.0f, 0.0f);
	glm::vec3 direction{ 0.0f, 0.0f, 1.0f };
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	if (focusEntity != nullptr) {
		position = focusEntity->transform->position;
		direction = -(focusEntity->transform->rotation * direction);
	}
	direction = glm::rotate(glm::mat4(1.0f), theta, glm::cross(direction, up)) * glm::vec4{ direction, 0.0f };

	return radius * direction + position;
}

void Camera::incrementTheta(float dt) {
	if (theta + (dt / 100.0f) < M_PI_2 && theta + (dt / 100.0f) > -M_PI_2) {
		theta += dt / 100.0f;
	}
}

void Camera::incrementPhi(float dp) {
	phi -= dp / 100.0f;
	if (phi > 2.0 * M_PI) {
		phi -= 2.0 * M_PI;
	}
	else if (phi < 0.0f) {
		phi += 2.0 * M_PI;
	}
}

void Camera::incrementR(float dr) {
	radius -= dr;
}
