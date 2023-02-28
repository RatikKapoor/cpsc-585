#pragma once

#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"

class Entity
{
public:
	Entity(std::string n, std::shared_ptr<Transform> t, Model *m, ShaderProgram *sp, glm::vec3 s)
		: name(n)
		, transform(t)
		, model(m)
		, shaderProgram(sp)
		, scale(s)
	{}

	virtual ~Entity() = 0;

	std::string name;
	std::shared_ptr<Transform> transform;

	Model *model;
	ShaderProgram *shaderProgram;
	glm::vec3 scale;
};