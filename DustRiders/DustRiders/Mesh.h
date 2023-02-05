#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"
#include "ShaderProgram.h"

class Mesh
{
public:
	// These vectors are parallel to represent a vertex overall
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;

	GPU_Geometry geometry;

	void draw(ShaderProgram& shader) {
		// Setup Textures and other uniforms that may be needed

		geometry.bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i)
		: verts(v)
		, indices(i)
	{
		geometry.bind();
		geometry.setVerts(verts);
		geometry.setIndices(indices);
	}
};