#pragma once
#include "Entity.h"

class Camera
{
public:
	Camera(Entity* focus, glm::vec3 offset, float t, float r);

	void setFocusEntity(Entity* newFocus);
	glm::mat4 getView();
	glm::vec3 getPos();
	void incrementTheta(float dt);
	void incrementR(float dr);

private:
	float theta;
	float radius;

	Entity* focusEntity;
	glm::vec3 focusOffset;
};

