#pragma once

#include "PhysicsEntity.h"
#include "ECS.h"

class TriggerEntity : public PhysicsEntity {
public:
	TriggerEntity(std::string n,
		Model* m,
		ShaderProgram* sp,
		glm::vec3 s,
		PhysicsProvider* pp,
		PxVec3 pos,
		unsigned int mat);

	virtual ~TriggerEntity() {}

protected:
	void initTriggerEntity(PxVec3);
};

