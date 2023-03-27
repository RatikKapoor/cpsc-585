#include "PhysicsSystem.h"

using namespace physx;
using namespace physx::vehicle2;
using namespace snippetvehicle2;

PhysicsSystem::PhysicsSystem(EntityComponentSystem& ecs): ecs(ecs)
{
	simulationCallbacks = new SimulationCallback(ecs);

	// Initialize PhysX
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation)
	{
		std::cout << "PxCreateFoundation failed!" << std::endl;
	}

	// PVD
	gPvd = PxCreatePvd(*gFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	// Physics
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);
	if (!gPhysics)
	{
		std::cout << "PxCreatePhysics failed!" << std::endl;
	}

	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale());
	if (!gCooking)
	{
		std::cout << "PxCreateCooking failed!" << std::endl;
	}

	// Scene
	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -25.f, 0.0f);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	gScene->setSimulationEventCallback(simulationCallbacks);

	// Prep PVD
	physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// Simulate
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	//groundPlane = physx::PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0.01), *gMaterial);
	//for (PxU32 i = 0; i < groundPlane->getNbShapes(); i++)
	//{
	//	PxShape* shape = NULL;
	//	groundPlane->getShapes(&shape, 1, i);
	//	shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	//	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	//	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	//}
	//gScene->addActor(*groundPlane);

	PxInitVehicleExtension(*gFoundation);

	// Initialize transforms;
	updateTransforms();
}

// physx::PxVec3 PhysicsSystem::getPosition()
//{
//	// Get the global position data from a physics collider as a vec3
//	physx::PxVec3 position = rigidDynamicList[50]->getGlobalPose().p;
//
//	return position;
// }

void PhysicsSystem::updatePhysics(double dt)
{
	gScene->simulate(dt);
	gScene->fetchResults(true);

	this->updateTransforms();
}

void PhysicsSystem::updateTransforms()
{
	for (int i = 0; i < transformList.size(); i++)
	{
		// Copy position values from physx::PxVec3 to glm::vec3
		transformList[i]->position.x = rigidActorList[i]->getGlobalPose().p.x;
		transformList[i]->position.y = rigidActorList[i]->getGlobalPose().p.y;
		transformList[i]->position.z = rigidActorList[i]->getGlobalPose().p.z;

		// Copy rotation values from physx::PxQuat to glm::quat
		transformList[i]->rotation.x = rigidActorList[i]->getGlobalPose().q.x;
		transformList[i]->rotation.y = rigidActorList[i]->getGlobalPose().q.y;
		transformList[i]->rotation.z = rigidActorList[i]->getGlobalPose().q.z;
		transformList[i]->rotation.w = rigidActorList[i]->getGlobalPose().q.w;
	}
}

physx::PxPhysics* PhysicsSystem::GetPxPhysics()
{
	return this->gPhysics;
}

physx::PxMaterial* PhysicsSystem::GetPxMaterial()
{
	return this->gMaterial;
}

physx::PxScene* PhysicsSystem::GetPxScene()
{
	return this->gScene;
}

physx::PxFoundation* PhysicsSystem::GetPxFoundation()
{
	return this->gFoundation;
}

physx::PxCooking* PhysicsSystem::GetPxCooking()
{
	return this->gCooking;
}

void PhysicsSystem::AddEntity(physx::PxRigidActor* r, Transform* t)
{
	this->rigidActorList.push_back(r);
	this->transformList.emplace_back(t);
}

void PhysicsSystem::RemoveEntity(physx::PxRigidActor* r)
{
	auto it = std::find(rigidActorList.begin(), rigidActorList.end(), r);
	// If element was found
	if (it != rigidActorList.end())
		return; // Does not exist in list

	int index = it - rigidActorList.begin();

	rigidActorList.erase(rigidActorList.begin() + index);
	transformList.erase(transformList.begin() + index);
}