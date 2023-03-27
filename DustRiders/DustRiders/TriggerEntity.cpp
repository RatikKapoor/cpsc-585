#include "TriggerEntity.h"

TriggerEntity::TriggerEntity(std::string n,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 pos,
	unsigned int mat): PhysicsEntity(n, m, sp, s, pp, pos, mat)
{
	initTriggerEntity(pos);
}

void TriggerEntity::initTriggerEntity(PxVec3 pos) {
	auto shape = this->gPhysics->createShape(PxBoxGeometry(0.5, 0.5, 0.5), *gMaterial);

	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
	shape->setFlag(PxShapeFlag::eVISUALIZATION, false);

	PxTransform localTm(pos);
	PxRigidDynamic *body = gPhysics->createRigidDynamic(localTm);
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
