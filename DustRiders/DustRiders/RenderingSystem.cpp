#include "RenderingSystem.h"

Model* RenderingSystem::loadModel(std::string name, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& colors)
{
	Model* m = new Model(positions, colors);
	models.emplace(std::make_pair(name, m));

	return m;
}

ShaderProgram* RenderingSystem::compileShader(std::string name, const std::string& vertexPath, const std::string& fragmentPath)
{
	ShaderProgram* s = new ShaderProgram(vertexPath, fragmentPath);
	shaders.emplace(std::make_pair(name, s));

	return s;
}

Texture* RenderingSystem::loadTexture(std::string name, std::string path, GLint interpolation)
{
	Texture* t = new Texture(path, interpolation);
	textures.emplace(std::make_pair(name, t));

	return t;
}

void RenderingSystem::updateRender(std::vector<Entity*>& entityList)
{
	// Rendering Objects
	glEnable(GL_FRAMEBUFFER_SRGB);
	glClearColor(0.5f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (Entity* entity : entityList) {
		int count = entity->model->use();
		entity->shaderProgram->use();

		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
}