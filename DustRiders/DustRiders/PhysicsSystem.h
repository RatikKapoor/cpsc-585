#pragma once

#include <iostream>
#include <vector>

#include "PxPhysicsAPI.h"
#include "vehicle2/PxVehicleAPI.h"
#include "snippetvehicle2common/serialization/BaseSerialization.h"

#include "Transform.h"

class PhysicsSystem
{
public:
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

	PhysicsSystem();

	//physx::PxVec3 getPosition();
	void updatePhysics(double);
	void updateTransforms();
};