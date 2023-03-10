#include "RayBeam.h"
#include "LogWriter.h"

void RayBeamCallback::onTrigger(PxTriggerPair *pairs, PxU32 count)
{
  for (PxU32 i = 0; i < count; i++)
  {
    // ignore pairs when shapes have been deleted
    if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
      continue;

    const char *shapeName = nullptr;

    shapeName = pairs[i].otherActor->getName();

    if (rb.shouldRender && shapeName != nullptr && std::string::npos != std::string(shapeName).find("car"))
    {
      ecs[shapeName]->flags["beamHit"] = true;
      LogWriter::log(std::string(shapeName) + " triggered");
    }
  }
}

RayBeam::RayBeam(std::string n,
                 Model *m,
                 ShaderProgram *sp,
                 glm::vec3 s,
                 PhysicsProvider *pp, EntityComponentSystem &ecs, PxVec3 pos = {0.f, 0.f, 0.f},
                 unsigned int mat = 0) : PhysicsEntity(n, m, sp, s, pp, pos, mat)
{
  isActive = false;
  shouldRender = false;
  posOffset = PxVec3(0.f, 5.1f, 33.9f);
  initBeam(pos, ecs);
}

void RayBeam::initBeam(PxVec3 pos, EntityComponentSystem &ecs)
{

  beamCallback = new RayBeamCallback(ecs, std::ref(*this));

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
