#pragma once

#include "PxPhysicsAPI.h"

#include "Entity.h"
#include "PhysicsProvider.h"

using namespace physx;

class PhysicsEntity : public Entity {
public:
	PhysicsEntity(std::string n,
	Transform* t,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 position = { 0.f, 0.f, 0.f },
	unsigned int mat = 0) : Entity(n, t, m, sp, s, mat)
	{
		physicsProvider = pp;
		this->gPhysics = pp->GetPxPhysics();
		this->gMaterial = pp->GetPxMaterial();
		this->gScene = pp->GetPxScene();
	}

	virtual ~PhysicsEntity() {}

protected:
	PhysicsProvider *physicsProvider;

	PxPhysics *gPhysics;
	PxMaterial *gMaterial;
	PxScene *gScene;
};
