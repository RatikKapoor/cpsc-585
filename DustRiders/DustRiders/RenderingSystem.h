#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "ShaderProgram.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Entity.h"

class RenderingSystem
{
	// storing assets to avoid duplication
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, ShaderProgram*> shaders;
	std::unordered_map<std::string, Texture*> textures;

public:

	Model* loadModel(std::string name, std::vector<Vertex>& verts, std::vector<unsigned int>& indices);
	ShaderProgram* compileShader(std::string name, const std::string& vertexPath, const std::string& fragmentPath);
	Texture* loadTexture(std::string name, std::string path, GLint interpolation);

	void updateRender(std::vector<Entity*>& entityList);
};


