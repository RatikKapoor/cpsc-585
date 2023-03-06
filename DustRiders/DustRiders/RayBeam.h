#pragma once
#include "PxPhysicsAPI.h"

#include "PhysicsEntity.h"
#include "PhysicsProvider.h"

using namespace physx;

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

protected:
  void initBeam(PxVec3 pos);
};