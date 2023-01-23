#pragma once

#include <string>
#include "Model.h"
#include "Transform.h"

class Entity
{
public:
	std::string name;
	Model* model;
	Transform* transform;
};