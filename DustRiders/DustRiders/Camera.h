#pragma once
#include "Entity.h"

class Camera
{
public:
	Camera(float t, float p, float r);

	void setFocusEntity(Entity* entity);

	glm::mat4 getView();
	glm::vec3 getPos();
	void incrementTheta(float dt);
	void incrementPhi(float dp);
	void incrementR(float dr);

private:

	float theta;
	float phi;
	float radius;

	Entity* focusEntity;
};

