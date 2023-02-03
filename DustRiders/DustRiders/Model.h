#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"

class Model
{
public:
	// These vectors are parallel to represent a vertex overall
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	//std::vector<glm::vec3> normals;
	//std::vector<glm::vec2> texCoords;

	GPU_Geometry geometry;

	int use() {
		geometry.bind();

		return positions.size();
	}

	Model(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& cols)
		: positions(verts)
		, colors(cols)
		//, normals(norms)
		//, texCoords(tex)
	{
		geometry.bind();
		geometry.setVerts(positions);
		geometry.setCols(colors);
		//geometry.setNormals(normals);
		//geometry.setTexCoords(texCoords);
	}
};