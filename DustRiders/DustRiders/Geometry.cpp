#include "Geometry.h"

#include <utility>
#include <vector>

// 0 -> position
// 1 -> normal
// 2 -> texCoord
GPU_Geometry::GPU_Geometry()
		: vao(), vertBuffer(std::vector<GLint>{3, 3, 2}, std::vector<GLenum>{GL_FLOAT, GL_FLOAT, GL_FLOAT}, sizeof(Vertex))
	//, normBuffer(1,3,GL_FLOAT), texCoordBuffer(3, 2, GL_FLOAT) // normalBuffer(3, 3, GL_FLOAT)
{

}

void GPU_Geometry::setVerts(const std::vector<Vertex> &verts)
{

	vertBuffer.uploadData(sizeof(Vertex) * verts.size(), verts.data(), GL_STATIC_DRAW);

}

void GPU_Geometry::setIndices(const std::vector<unsigned int> &indices)
{
	indexBuffer.uploadData(sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}