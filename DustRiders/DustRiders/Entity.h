#pragma once

#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"
#include <map>

class Entity
{
public:
	Entity(std::string n, Model *m, ShaderProgram *sp, glm::vec3 s, unsigned int matInt = 0)
			: name(n), model(m), shaderProgram(sp), scale(s), useMatInt(matInt), shouldRender(true)
	{
		transform = new Transform();
	}

	virtual ~Entity(){};

	std::string name;
	Transform *transform;
	bool shouldRender;

	Model *model;
	ShaderProgram *shaderProgram;
	glm::vec3 scale;

	std::map<std::string, bool> flags;

	Texture *texture;
	unsigned int useMatInt;

	operator std::string()
	{
		std::string result;

		result.append(name + " ");
	}
};
