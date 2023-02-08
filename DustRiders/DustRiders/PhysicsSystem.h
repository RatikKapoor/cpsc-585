#pragma once

#include <iostream>
#include <vector>

#include "PxPhysicsAPI.h"
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

	PhysicsSystem();

	//physx::PxVec3 getPosition();
	void updatePhysics();
	void updateTransforms();
};