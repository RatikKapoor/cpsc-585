#pragma once

#include "Entity.h"

class Ground : public Entity {
public:
	Ground(std::string n, std::shared_ptr<Transform> t, Model *m, ShaderProgram *sp, glm::vec3 s)
		: Entity(n, t, m, sp, s)
	{}

	virtual ~Ground() {}
};
