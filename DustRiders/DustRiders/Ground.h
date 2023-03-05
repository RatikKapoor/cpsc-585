#pragma once

#include "Entity.h"

class Ground : public Entity {
public:
	Ground(std::string n, Model *m, ShaderProgram *sp, glm::vec3 s)
		: Entity(n, m, sp, s)
	{}

	virtual ~Ground() {}
};
