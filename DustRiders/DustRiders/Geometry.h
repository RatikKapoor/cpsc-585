#pragma once

//------------------------------------------------------------------------------
// This file contains simple classes for storing geomtery on the CPU and the GPU
// Later assignments will require you to expand these classes or create your own
// similar classes with the needed functionality
//------------------------------------------------------------------------------

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

// #include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

// VAO and two VBOs for storing vertices and texture coordinates, respectively
class GPU_Geometry
{

public:
	GPU_Geometry();

	// Public interface
	void bind() { vao.bind(); }
	void setVerts(const std::vector<Vertex> &verts);
	void setIndices(const std::vector<unsigned int> &indices);
	void setTexCoords(const std::vector<glm::vec2> &texCoords);
	void setNorms(const std::vector<glm::vec3> &norms);

private:
	// note: due to how OpenGL works, vao needs to be
	// defined and initialized before the vertex buffers
	VertexArray vao;
	VertexBuffer vertBuffer;
	VertexBuffer normBuffer;
	VertexBuffer texCoordBuffer;
	ElementBuffer indexBuffer;
};
