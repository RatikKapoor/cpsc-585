#pragma once
#include "PxPhysicsAPI.h"

#include "PhysicsEntity.h"
#include "PhysicsProvider.h"

#include "LogWriter.h"

class RayBeamCallback : public PxSimulationEventCallback
{
public:
  RayBeamCallback() : PxSimulationEventCallback(){};

  void onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer, const PxU32 count){};

  void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count){};

  void onWake(PxActor **actors, PxU32 count){};
  void onSleep(PxActor **actors, PxU32 count){};

  void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs){};

  void onTrigger(PxTriggerPair *pairs, PxU32 count) { LogWriter::log("Trigger fired"); };
};

class RayBeam : public PhysicsEntity
{
public:
  RayBeam(std::string n,
          Model *m,
          ShaderProgram *sp,
          glm::vec3 s,
          PhysicsProvider *pp,
          PxVec3 pos,
          unsigned int);

  virtual ~RayBeam(){};
  bool isActive;

  void updatePos(PxTransform, Transform *);

  PxRigidDynamic *body;

  RayBeamCallback *beamCallback;

protected:
  void initBeam(PxVec3 pos);
  PxVec3 posOffset;
};
