#include "ShaderProgram.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "Log.h"

ShaderProgram::ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath)
	: programID(), vertex(vertexPath, GL_VERTEX_SHADER), fragment(fragmentPath, GL_FRAGMENT_SHADER)
{
	attach(*this, vertex);
	attach(*this, fragment);
	glLinkProgram(programID);

	if (!checkAndLogLinkSuccess())
	{
		glDeleteProgram(programID);
		throw std::runtime_error("Shaders did not link.");
	}
}

bool ShaderProgram::recompile()
{

	try
	{
		// Try to create a new program
		ShaderProgram newProgram(vertex.getPath(), fragment.getPath());
		*this = std::move(newProgram);
		return true;
	}
	catch (std::runtime_error &e)
	{
		return false;
	}
}

void attach(ShaderProgram &sp, Shader &s)
{
	glAttachShader(sp.programID, s.shaderID);
}

bool ShaderProgram::checkAndLogLinkSuccess() const
{

	GLint success;

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetProgramInfoLog(programID, logLength, NULL, log.data());

		return false;
	}
	else
	{
		return true;
	}
}
