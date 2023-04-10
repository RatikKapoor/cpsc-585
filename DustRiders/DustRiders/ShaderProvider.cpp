#include "ShaderProvider.h"

ShaderProgram *ShaderProvider::carShader;
ShaderProgram *ShaderProvider::mainShader;
ShaderProgram *ShaderProvider::debugShader;
ShaderProgram *ShaderProvider::mapShader;
ShaderProgram *ShaderProvider::flameShader;

void ShaderProvider::initialize(RenderingSystem &renderer)
{
	carShader = renderer.compileShader("car", "./car.vert", "./car.frag");
	mainShader = renderer.compileShader("basic", "./Main.vert", "./Main.frag");
	debugShader = renderer.compileShader("debug", "./debug.vert", "./debug.frag");
	mapShader = renderer.compileShader("map", "./map.vert", "./map.frag");
	flameShader = renderer.compileShader("flame", "./flame.vert", "./flame.frag");
}