#pragma once
#include "Entity.h"

class RayBeam : public Entity
{
public:
  RayBeam(std::string n, Model *m, ShaderProgram *sp, glm::vec3 s)
      : Entity(n, m, sp, s)
  {
    isActive = true;
  }

  virtual ~RayBeam(){};

  bool isActive;
};