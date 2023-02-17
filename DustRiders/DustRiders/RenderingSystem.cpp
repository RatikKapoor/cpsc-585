#include "RenderingSystem.h"

#include "Geometry.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Model *RenderingSystem::addModel(std::string name, Model *model)
{
	models.emplace(std::make_pair(name, model));
	return model;
}

Model *RenderingSystem::loadModelFromFile(std::string name, std::string filepath)
{
	Model *newModel = new Model();

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return nullptr;
	}

	processNode(scene->mRootNode, scene, newModel);

	models.emplace(std::make_pair(name, newModel));
	return newModel;
}

void RenderingSystem::processNode(aiNode *node, const aiScene *scene, Model *model)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model->meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, model);
	}
}

Mesh RenderingSystem::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<glm::vec2> texCoords;

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

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		int matIndex = mesh->mMaterialIndex;
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

ShaderProgram *RenderingSystem::compileShader(std::string name, const std::string &vertexPath, const std::string &fragmentPath)
{
	ShaderProgram *s = new ShaderProgram(vertexPath, fragmentPath);
	shaders.emplace(std::make_pair(name, s));

	return s;
}

// Texture RenderingSystem::loadTexture(std::string name, std::string path, GLint interpolation)
// {
// 	Texture t = Texture(path.c_str(), interpolation);
// 	textures.emplace(std::make_pair(name, t));

// 	return t;
// }

std::vector<Texture> RenderingSystem::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string strDir = "../DustRiders/assets/models/";
		strDir.append(std::string(str.C_Str()));
		Texture texture(strDir.c_str(), GL_LINEAR);
		texture.setType(typeName);
		textures.push_back(texture);
	}
	return textures;
}

void RenderingSystem::updateRender(std::vector<Entity *> &entityList, Camera &cam)
{
	// Rendering Objects
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = cam.getView();
	glm::vec3 camPos = cam.getPos();

	float aspect = 1.0f;
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 1000.f);

	for (Entity *entity : entityList)
	{
		ShaderProgram &shader = *entity->shaderProgram;
		shader.use();

		glm::mat4 model(1.0f);
		model = glm::translate(model, entity->transform->position);
		model = model * glm::toMat4(entity->transform->rotation);
		model = glm::scale(model, entity->scale);
		glm::vec3 lightPos = glm::vec3(0.f, 40.f, 0.f);
		glm::vec3 lightCol = glm::vec3(1.f, 1.f, 1.f);


		GLuint location = glGetUniformLocation(shader, "M");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));

		location = glGetUniformLocation(shader, "V");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));

		location = glGetUniformLocation(shader, "P");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(perspective));

		location = glGetUniformLocation(shader, "lightPos");
		glUniform3fv(location, 1, glm::value_ptr(lightPos));
		
		location = glGetUniformLocation(shader, "lightCol");
		glUniform3fv(location, 1, glm::value_ptr(lightCol));

		location = glGetUniformLocation(shader, "cameraPos");
		glUniform3fv(location, 1, glm::value_ptr(camPos));

		entity->model->draw(shader);
	}

	glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
}