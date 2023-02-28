#pragma once

#include <iostream>
#include <vector>

#include "PxPhysicsAPI.h"
#include "vehicle2/PxVehicleAPI.h"
#include "snippetvehicle2common/serialization/BaseSerialization.h"

#include "PhysicsProvider.h"
#include "Transform.h"

class PhysicsSystem: public PhysicsProvider
{
private:
	std::vector<physx::PxRigidDynamic*> rigidDynamicList;
	std::vector<Transform*> transformList;

	//PhysX management class instances.
	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;
	physx::PxFoundation* gFoundation = NULL;
	physx::PxPhysics* gPhysics = NULL;
	physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
	physx::PxScene* gScene = NULL;
	physx::PxMaterial* gMaterial = NULL;
	physx::PxPvd* gPvd = NULL;
	physx::PxRigidStatic* groundPlane = NULL;

	void updateTransforms();

public:
	PhysicsSystem();
	void updatePhysics(double);

	std::shared_ptr<physx::PxPhysics> GetPxPhysics();
	std::shared_ptr<physx::PxMaterial> GetPxMaterial();
	std::shared_ptr<physx::PxScene> GetPxScene();
	void AddEntity(physx::PxRigidDynamic*, std::shared_ptr<Transform>);
};