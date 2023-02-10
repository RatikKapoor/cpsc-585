#pragma once
#include "Entity.h"

class Camera
{
public:
	Camera(Entity* focus, float t, float r);

	glm::mat4 getView();
	glm::vec3 getPos();
	void incrementTheta(float dt);
	void incrementR(float dr);

private:
	float theta;
	float radius;

	Entity* focusEntity;
};

