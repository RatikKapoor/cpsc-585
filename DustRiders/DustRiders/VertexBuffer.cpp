#include "VertexBuffer.h"

#include <utility>
#include <vector>


VertexBuffer::VertexBuffer(std::vector <GLint> sizes, std::vector <GLenum> dataTypes, int stride)
	: bufferID{}
{
	bind();
	int offset = 0;
	for (int i = 0; i < sizes.size(); i++) {
		glVertexAttribPointer(i, sizes[i], dataTypes[i], GL_FALSE, stride, (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);

		offset += sizes[i];
	}
}

VertexBuffer::VertexBuffer(GLuint index, GLint size, GLenum dataType)
	: bufferID{}
{
	bind();
	glVertexAttribPointer(index, size, dataType, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(index);
}


void VertexBuffer::uploadData(GLsizeiptr size, const void* data, GLenum usage) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
