#pragma once

#include <iostream>
#include <vector>

#include "PxPhysicsAPI.h"
#include "vehicle2/PxVehicleAPI.h"
#include "snippetvehicle2common/serialization/BaseSerialization.h"

#include "PhysicsProvider.h"
#include "Transform.h"

class PhysicsSystem : public PhysicsProvider
{
private:
	std::vector<physx::PxRigidDynamic *> rigidDynamicList;
	std::vector<Transform *> transformList;

	// PhysX management class instances.
	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;
	physx::PxFoundation *gFoundation = NULL;
	physx::PxPhysics *gPhysics = NULL;
	physx::PxDefaultCpuDispatcher *gDispatcher = NULL;
	physx::PxScene *gScene = NULL;
	physx::PxMaterial *gMaterial = NULL;
	physx::PxPvd *gPvd = NULL;
	physx::PxRigidStatic *groundPlane = NULL;
	physx::PxCooking *gCooking = NULL;

	void updateTransforms();

public:
	PhysicsSystem();
	void updatePhysics(double);

	physx::PxPhysics *GetPxPhysics();
	physx::PxMaterial *GetPxMaterial();
	physx::PxScene *GetPxScene();
	physx::PxFoundation *GetPxFoundation();
	physx::PxCooking *GetPxCooking();
	void AddEntity(physx::PxRigidDynamic *, Transform *);
};