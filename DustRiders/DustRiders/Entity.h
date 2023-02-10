#pragma once

#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"

class Entity
{
public:
	Entity()
		: name("")
		, transform(nullptr)
		, model(nullptr)
		, shaderProgram(nullptr)
		, scale(1.0f)
		, texture(nullptr)
	{}

	std::string name;
	Transform* transform;

	Model* model;
	ShaderProgram* shaderProgram;
	glm::vec3 scale;
	Texture* texture;
};