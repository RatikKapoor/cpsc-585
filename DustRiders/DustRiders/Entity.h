#pragma once

#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"

class Entity
{
public:
	Entity(std::string n, Transform* t, Model *m, ShaderProgram *sp, glm::vec3 s)
		: name(n)
		, transform(t)
		, model(m)
		, shaderProgram(sp)
		, scale(s)
	{}

	virtual ~Entity() {};

	std::string name;
	Transform* transform;

	Model *model;
	ShaderProgram *shaderProgram;
	glm::vec3 scale;
};