#include "PhysicsSystem.h"

using namespace physx;
using namespace physx::vehicle2;
using namespace snippetvehicle2;

PhysicsSystem::PhysicsSystem()
{
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

	// Scene
	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

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
	groundPlane = physx::PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0.01), *gMaterial);
	for (PxU32 i = 0; i < groundPlane->getNbShapes(); i++)
	{
		PxShape* shape = NULL;
		groundPlane->getShapes(&shape, 1, i);
		shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	}
	gScene->addActor(*groundPlane);

	PxInitVehicleExtension(*gFoundation);

	//// Define a box
	//float halfLen = 0.5f;
	//physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(halfLen, halfLen, halfLen), *gMaterial);
	//physx::PxU32 size = 30;
	//physx::PxTransform tran(physx::PxVec3(0));

	//// Save some space for all the boxes in the scene
	//rigidDynamicList.reserve(465);
	//transformList.reserve(465);

	//// Create a pyramid of physics-enabled boxes
	//for (physx::PxU32 i = 0; i < size; i++)
	//{
	//	for (physx::PxU32 j = 0; j < size - i; j++)
	//	{
	//		physx::PxTransform localTran(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 - 1), 0) * halfLen);
	//		physx::PxRigidDynamic* body = gPhysics->createRigidDynamic(tran.transform(localTran));

	//		// Add the PxRigidDynamic to a vector
	//		rigidDynamicList.push_back(body);
	//		transformList.emplace_back(new Transform());

	//		body->attachShape(*shape);
	//		physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	//		gScene->addActor(*body);
	//	}
	//}

	//// Clean up
	//shape->release();

	// Initialize transforms;
	updateTransforms();
}

//physx::PxVec3 PhysicsSystem::getPosition()
//{
//	// Get the global position data from a physics collider as a vec3
//	physx::PxVec3 position = rigidDynamicList[50]->getGlobalPose().p;
//
//	return position;
//}



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
		transformList[i]->position.x = rigidDynamicList[i]->getGlobalPose().p.x;
		transformList[i]->position.y = rigidDynamicList[i]->getGlobalPose().p.y;
		transformList[i]->position.z = rigidDynamicList[i]->getGlobalPose().p.z;

		// Copy rotation values from physx::PxQuat to glm::quat
		transformList[i]->rotation.x = rigidDynamicList[i]->getGlobalPose().q.x;
		transformList[i]->rotation.y = rigidDynamicList[i]->getGlobalPose().q.y;
		transformList[i]->rotation.z = rigidDynamicList[i]->getGlobalPose().q.z;
		transformList[i]->rotation.w = rigidDynamicList[i]->getGlobalPose().q.w;
	}
}

std::shared_ptr<physx::PxPhysics> PhysicsSystem::GetPxPhysics()
{
	return std::shared_ptr<physx::PxPhysics>(this->gPhysics);
}

std::shared_ptr<physx::PxMaterial> PhysicsSystem::GetPxMaterial()
{
	return std::shared_ptr<physx::PxMaterial>(this->gMaterial);
}

std::shared_ptr<physx::PxScene> PhysicsSystem::GetPxScene() 
{
	return std::shared_ptr<physx::PxScene>(this->gScene);
}

void PhysicsSystem::AddEntity(physx::PxRigidDynamic *r, std::shared_ptr<Transform> t)
{
	this->rigidDynamicList.push_back(r);
	this->transformList.emplace_back(t.get());
}