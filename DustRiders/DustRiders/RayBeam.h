#pragma once
#include "PxPhysicsAPI.h"

#include "PhysicsEntity.h"
#include "PhysicsProvider.h"

#include "ECS.h"

#include "TimeKeeper.h"

class RayBeamCallback;

class RayBeam : public PhysicsEntity
{
public:
  RayBeam(std::string n,
          Model *m,
          ShaderProgram *sp,
          glm::vec3 s,
          PhysicsProvider *pp, EntityComponentSystem &ecs, PxVec3 pos,
          unsigned int mat);

  virtual ~RayBeam(){};
  bool isActive;

  void updatePos(PxTransform, Transform *);

  PxRigidDynamic *rcOrigin;
  PxRigidDynamic *rcDirection;

  RayBeamCallback *beamCallback;



  PxVec3 rayCastOrigin;
  PxTransform beamRotation;
  PxVec3 rayCastDirection;
  PxTransform vehiclePos;



  std::string castRayBeam();

  bool canFire();

protected:
  void initBeam(PxVec3 pos, EntityComponentSystem &ecs);
  PxVec3 beamOriginOffset;
  PxVec3 beamDirRaw;
  double lastFireTime;
  double coolDownTime;
  TimeKeeper timeKeep;

};

class RayBeamCallback : public PxSimulationEventCallback
{
public:
  RayBeamCallback(EntityComponentSystem &ecs, RayBeam &rb) : PxSimulationEventCallback(), ecs(ecs), rb(rb){};

  void onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer, const PxU32 count){};

  void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count){};

  void onWake(PxActor **actors, PxU32 count){};
  void onSleep(PxActor **actors, PxU32 count){};

  void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs){};

  void onTrigger(PxTriggerPair *pairs, PxU32 count);

  EntityComponentSystem &ecs;

  RayBeam &rb;
};