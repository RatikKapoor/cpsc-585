#pragma once

#include "GLHandles.h"
//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Texture {
public:
	Texture(const char* path, GLint interpolation);

	// Because we're using the TextureHandle to do RAII for the texture for us
	// and our other types are trivial or provide their own RAII
	// we don't have to provide any specialized functions here. Rule of zero
	//
	// https://en.cppreference.com/w/cpp/language/rule_of_three
	// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero

	// Public interface
	std::string getPath() const { return path; }
	GLenum getInterpolation() const { return interpolation; }

	// Although uint (i.e. uvec2) might make more sense here, went with int (i.e. ivec2) under
	// the assumption that most students will want to work with ints, not uints, in main.cpp
	glm::ivec2 getDimensions() const { return glm::uvec2(width, height); }

	void bind() { glBindTexture(GL_TEXTURE_2D, textureID); }
	void unbind() { glBindTexture(GL_TEXTURE_2D, textureID); }

	void setID(GLint iD) { this->textureID = iD; }
	void setPath(std::string path) { this->path = path; }
	void setInterpolation(GLint interpolation) { this->interpolation = interpolation; }
	void setType(std::string type) { this->type = type; }

	void textureFromFile(const char* path);
	void textureFromFile(const char* path, const std::string &directory);


	std::string type;
private:
	GLuint textureID;
	std::string path;
	GLint interpolation;
	


	// Although uint might make more sense here, went with int under the assumption
	// that most students will want to work with ints, not uints, in main.cpp
	int width;
	int height;



};
