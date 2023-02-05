#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"

# include <vector>


class Model
{
public:
	std::vector<Mesh> meshes;

	void draw(ShaderProgram& shader) {
		for (int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(shader);
		}
	}
};

