#include "ShaderProvider.h"

ShaderProgram* ShaderProvider::carShader;
ShaderProgram* ShaderProvider::mainShader;
ShaderProgram* ShaderProvider::debugShader;

void ShaderProvider::initialize(RenderingSystem& renderer) {
	carShader = renderer.compileShader("car", "./car.vert", "./car.frag");
	mainShader = renderer.compileShader("basic", "./Main.vert", "./Main.frag");
	debugShader = renderer.compileShader("debug", "./debug.vert", "./debug.frag");
}