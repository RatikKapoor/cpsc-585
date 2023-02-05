#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"

class Model
{
public:
	// These vectors are parallel to represent a vertex overall
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;

	GPU_Geometry geometry;

	int use() {
		geometry.bind();

		return indices.size();
	}

	Model(std::vector<Vertex>& v, std::vector<unsigned int>& i)
		: verts(v)
		, indices(i)
	{
		geometry.bind();
		geometry.setVerts(verts);
		geometry.setIndices(indices);
	}
};