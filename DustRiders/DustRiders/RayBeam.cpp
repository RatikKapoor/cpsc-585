#include "RayBeam.h"

RayBeam::RayBeam(std::string n,
                 Model *m,
                 ShaderProgram *sp,
                 glm::vec3 s,
                 PhysicsProvider *pp,
                 PxVec3 pos = {0.f, 0.f, 0.f},
                 unsigned int mat = 0) : PhysicsEntity(n, m, sp, s, pp, pos, mat)
{
  isActive = false;
  shouldRender = false;
  posOffset = PxVec3(0.f, 5.1f, 33.9f);
  initBeam(pos);
}

void RayBeam::initBeam(PxVec3 pos)
{

  beamCallback = new RayBeamCallback;

  gScene->setSimulationEventCallback(beamCallback);

  auto shape = this->gPhysics->createShape(PxBoxGeometry(0.1, 10.0, 30.0), *gMaterial);

  shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
  shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
  shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);

  PxTransform localTm(pos + posOffset);
  body = gPhysics->createRigidDynamic(localTm);
  body->setGlobalPose(localTm);
  body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
  body->attachShape(*shape);
  PxRigidBodyExt::updateMassAndInertia(*body, 10.f);
  gScene->addActor(*body);
  physicsProvider->AddEntity(body, this->transform);

  shape->release();
}

void RayBeam::updatePos(PxTransform vehiclePos, Transform *tf)
{

  PxTransform beamRotation(PxTransform(posOffset).getInverse() * PxTransform(vehiclePos.q) * PxTransform(posOffset));

  PxTransform localPos(posOffset + vehiclePos.p);

  this->body->setGlobalPose(localPos * beamRotation);
  this->transform = tf;
}
