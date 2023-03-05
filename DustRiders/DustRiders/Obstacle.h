#pragma once

#include "PxPhysicsAPI.h"

#include "PhysicsEntity.h"
#include "PhysicsProvider.h"

using namespace physx;

class Obstacle : public PhysicsEntity {
public:
	Obstacle(std::string,
		Transform*,
		Model*,
		ShaderProgram*,
		glm::vec3,
		PhysicsProvider*,
		PxVec3,
		unsigned int);
	virtual ~Obstacle() {}

protected:
	void initObstacle(PxVec3);
};
