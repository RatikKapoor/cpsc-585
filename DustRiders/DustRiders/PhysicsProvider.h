#pragma once
#include "PxPhysicsAPI.h"
#include "Transform.h"

class PhysicsProvider
{
public:
	virtual physx::PxPhysics *GetPxPhysics() = 0;
	virtual physx::PxMaterial *GetPxMaterial() = 0;
	virtual physx::PxScene *GetPxScene() = 0;
	virtual physx::PxFoundation *GetPxFoundation() = 0;
	virtual physx::PxCooking *GetPxCooking() = 0;
	virtual void AddEntity(physx::PxRigidActor *, Transform *) = 0;
};
