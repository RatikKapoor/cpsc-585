#pragma once

#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"

class Entity
{
public:
	std::string name;
	Transform* transform;

	Model* model;
	ShaderProgram* shaderProgram;
	Texture* texture;
};