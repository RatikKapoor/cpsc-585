#pragma once

#include "Provider.h"
#include "ShaderProgram.h"
#include "RenderingSystem.h"

class ShaderProvider : public Provider
{
public:
	static void initialize(RenderingSystem &);

	static ShaderProgram *carShader;
	static ShaderProgram *mainShader;
	static ShaderProgram *debugShader;
	static ShaderProgram *mapShader;
	static ShaderProgram *flameShader;
};
