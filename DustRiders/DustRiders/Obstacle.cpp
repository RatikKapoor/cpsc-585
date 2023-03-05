#include "Obstacle.h"

Obstacle::Obstacle(std::string n,
	Transform* t,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 pos = { 0.f, 0.f, 0.f },
	unsigned int mat = 0) : PhysicsEntity(n, t, m, sp, s, pp, pos, mat)
{
	initObstacle(pos);
}

void Obstacle::initObstacle(PxVec3 pos) {
	auto shape = this->gPhysics->createShape(PxBoxGeometry(2, 2, 2), *gMaterial);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

	PxTransform localTm(PxVec3(PxReal(pos.x) - PxReal(pos.y), PxReal(pos.z), 0) * 2);
	PxRigidDynamic* body = gPhysics->createRigidDynamic(localTm);
	body->setGlobalPose(localTm);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.f);
	gScene->addActor(*body);
	physicsProvider->AddEntity(body, this->transform);

	shape->release();
}
