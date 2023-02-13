#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Geometry.h"
#include "ShaderProgram.h"
#include "Texture.h"

class Mesh
{
public:
	// These vectors are parallel to represent a vertex overall
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	GPU_Geometry geometry;

	void draw(ShaderProgram& shader) {
		// Setup Textures and other uniforms that may be needed
	
		geometry.bind();
		for (auto t : textures) {
			t.bind();
		}
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}


	Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i, std::vector<Texture>& textures)
		: verts(v)
		, indices(i),
		textures(textures)
	{
		geometry.bind();
		geometry.setVerts(verts);
		std::vector<glm::vec2> texCoords;
		for (auto vert : v) {
			texCoords.push_back(vert.texCoord);
		}
		geometry.setTexCoords(texCoords);
		geometry.setIndices(indices);
	}
};