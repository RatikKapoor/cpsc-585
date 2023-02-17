#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "ShaderProgram.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Entity.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class RenderingSystem
{
	// storing assets to avoid duplication
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, ShaderProgram*> shaders;
	std::unordered_map<std::string, Texture*> textures;

public:

	Model* addModel(std::string name, Model* model);

	Model* loadModelFromFile(std::string name, std::string filepath);
	void processNode(aiNode* node, const aiScene* scene, Model* model);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	ShaderProgram* compileShader(std::string name, const std::string& vertexPath, const std::string& fragmentPath);
	Texture loadTexture(std::string name, std::string path, GLint interpolation);

	void updateRender(std::vector<Entity*>& entityList, Camera& cam, float aspect);
};


