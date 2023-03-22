#pragma once

#include "PxPhysicsAPI.h"

#include "PhysicsEntity.h"
#include "PhysicsProvider.h"

using namespace physx;

class Ground : public PhysicsEntity
{
public:
	Ground(std::string,
				 Model *,
				 ShaderProgram *,
				 glm::vec3,
				 PhysicsProvider *,
				 PxVec3,
				 unsigned int);
	virtual ~Ground() {}

protected:
	void initGround(PxVec3);
};
