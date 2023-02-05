#pragma once

#include "GLHandles.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class ElementBuffer {

public:
	ElementBuffer();

	void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID); }
	void uploadData(GLsizeiptr size, const void* data, GLenum usage);

private:
	VertexBufferHandle bufferID;
};
