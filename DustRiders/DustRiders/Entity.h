#pragma once

#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"

class Entity
{
public:
	Entity(std::string n, Transform* t, Model *m, ShaderProgram *sp, glm::vec3 s, unsigned int matInt=0)
		: name(n)
		, transform(t)
		, model(m)
		, shaderProgram(sp)
		, scale(s)
		, useMatInt(matInt)
	{}

	virtual ~Entity() {};

	std::string name;
	Transform *transform;

	Model *model;
	ShaderProgram *shaderProgram;
	glm::vec3 scale;

	Texture *texture;
	unsigned int useMatInt;

	operator std::string() {
		std::string result;

		result.append(name + " ");
	}
};
