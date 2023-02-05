#include "ElementBuffer.h"

#include <utility>


ElementBuffer::ElementBuffer()
	: bufferID{}
{
	bind();
}


void ElementBuffer::uploadData(GLsizeiptr size, const void* data, GLenum usage) {
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}
