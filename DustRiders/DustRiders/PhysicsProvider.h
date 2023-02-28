#pragma once
#include "PxPhysicsAPI.h"
#include "Transform.h"

class PhysicsProvider {
public:
	virtual std::shared_ptr<physx::PxPhysics> GetPxPhysics() = 0;
	virtual std::shared_ptr<physx::PxMaterial> GetPxMaterial() = 0;
	virtual std::shared_ptr<physx::PxScene> GetPxScene() = 0;
	virtual void AddEntity(physx::PxRigidDynamic*, std::shared_ptr<Transform>) = 0;
};
