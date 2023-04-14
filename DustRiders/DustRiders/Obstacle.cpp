#include "Obstacle.h"

Obstacle::Obstacle(std::string n,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 pos = { 0.f, 0.f, 0.f },
	unsigned int mat = 0) : PhysicsEntity(n, m, sp, s, pp, pos, mat)
{
	initObstacle(pos);
}

void Obstacle::initObstacle(PxVec3 pos)
{
	auto shape = this->gPhysics->createShape(PxBoxGeometry(0.5, 0.2, 0.5), *gMaterial);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

	PxTransform localTm(pos);
	PxRigidDynamic* body = gPhysics->createRigidDynamic(localTm);
	body->setGlobalPose(localTm);
	body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	body->attachShape(*shape);
	body->setName((this->name).c_str());
	PxRigidBodyExt::updateMassAndInertia(*body, 10.f);
	gBody = body;
	gScene->addActor(*gBody);
	physicsProvider->AddEntity(gBody, this->transform);

	shape->release();
}
