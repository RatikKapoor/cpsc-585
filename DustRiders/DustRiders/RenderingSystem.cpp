#include "RenderingSystem.h"

#include "Geometry.h"
#include <iostream>

Model* RenderingSystem::addModel(std::string name, Model* model)
{
	models.emplace(std::make_pair(name, model));
	return model;
}

Model* RenderingSystem::loadModelFromFile(std::string name, std::string filepath)
{
	Model* newModel = new Model();

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return nullptr;
	}

	processNode(scene->mRootNode, scene, newModel);

	models.emplace(std::make_pair(name, newModel));
	return newModel;
}

void RenderingSystem::processNode(aiNode* node, const aiScene* scene, Model* model)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model->meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, model);
	}
}

Mesh RenderingSystem::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 tempVector;

		tempVector.x = mesh->mVertices[i].x;
		tempVector.y = mesh->mVertices[i].y;
		tempVector.z = mesh->mVertices[i].z;
		vertex.position = tempVector;

		tempVector.x = mesh->mNormals[i].x;
		tempVector.y = mesh->mNormals[i].y;
		tempVector.z = mesh->mNormals[i].z;
		vertex.normal = tempVector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vec;
		}
		else
			vertex.texCoord = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials / textures
	
	return Mesh(vertices, indices);
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
		ShaderProgram& shader = *entity->shaderProgram;
		shader.use();
		entity->model->draw(shader);
	}

	glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
}