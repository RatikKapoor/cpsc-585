#include "RenderingSystem.h"

#include "Geometry.h"
#include "Vehicle.h"
#include "AIVehicle.h"
#include <regex>

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "DebugDefines.h"

Model *RenderingSystem::addModel(std::string name, Model *model)
{
	models.emplace(std::make_pair(name, model));
	return model;
}

// Model *RenderingSystem::loadModelFromFile(std::string name, std::string filepath)
// {
// 	Model *newModel = new Model();

// 	Assimp::Importer import;
// 	const aiScene *scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

// 	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
// 	{
// 		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
// 		return nullptr;
// 	}

// 	processNode(scene->mRootNode, scene, newModel);

// 	models.emplace(std::make_pair(name, newModel));
// 	return newModel;
// }

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
	std::vector<glm::vec2> texCoords;
	std::vector<std::vector<Texture>> materialTextures;

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

	for (int i = mesh->mMaterialIndex; i > -1; i--)
	{
		std::vector<Texture> textures;
		aiMaterial *material = scene->mMaterials[i];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
		textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
		materialTextures.push_back(textures);
	}

	return Mesh(vertices, indices, materialTextures);
}

ShaderProgram *RenderingSystem::compileShader(std::string name, const std::string &vertexPath, const std::string &fragmentPath)
{
	ShaderProgram *s = new ShaderProgram(vertexPath, fragmentPath);
	shaders.emplace(std::make_pair(name, s));

	return s;
}

std::vector<Texture> RenderingSystem::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string strDir = "assets/models/";
		strDir.append(std::string(str.C_Str()));
		Texture texture(strDir.c_str(), GL_LINEAR);
		texture.setType(typeName);
		textures.push_back(texture);
	}
	return textures;
}


void RenderingSystem::updateRender(EntityComponentSystem &ecs, Camera &cam, float aspect)
{
		lightPos = glm::vec3(cam.getPos().x-160.f, 0.f, cam.getPos().z+60.f) + lightOffset;
		drawShadowMap(ecs, cam, aspect, lightPos);
		lastChunkCount = ChunkHandler::chunkCounter;

#ifdef SHADOW_ONLY
	renderDepth(ecs, cam, aspect);
	return;
#endif

	// Rendering Objects
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = cam.getView();
	glm::vec3 camPos = cam.getPos();

	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 1000.f);
	glm::vec3 lightCol = glm::vec3(1.f, 1.f, 1.f);


	int numRendered = 0;

	for (Entity *entity : ecs.getAll())
	{
		if (entity->shouldRender)
		{

			ShaderProgram &shader = *entity->shaderProgram;
			shader.use();

			glm::mat4 model(1.0f);
			model = glm::translate(model, entity->transform->position);
			model = model * glm::toMat4(entity->transform->rotation);
			model = glm::scale(model, entity->scale);

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

			if(regex_match(entity->name, regex("(ground)(.*)"))){
				unsigned int lightSpaceMatrixPos = glGetUniformLocation(shader, "lightSpaceMatrix");
				glUniformMatrix4fv(lightSpaceMatrixPos, 1, GL_FALSE, glm::value_ptr(getLightSpaceMatrix(lightPos)));

				glUniform1i(glGetUniformLocation(shader, "shadowMap"), 3);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, shadowMap);
				glActiveTexture(GL_TEXTURE0);
			}

			if (regex_match(entity->name, regex("(car)(.*)")))
			{
				// LogWriter::log("Rendering car \"" + entity->name + "\"");
				location = glGetUniformLocation(shader, "hitVisible");
				if (((AIVehicle *)ecs[entity->name])->hitVisible())
				{
					// LogWriter::log("	Hit is visible.");
					glUniform1f(location, 1.0f);
				}
				else
				{
					// LogWriter::log("	Hit is not visible.");
					glUniform1f(location, 0.0f);
				}
			}
			else
			{
				location = glGetUniformLocation(shader, "hitVisible");
				glUniform1f(location, 0.0f);
			}
			entity->model->draw(shader, entity->useMatInt);
			numRendered++;

		}
	}

	glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
}

void RenderingSystem::createShadowmap(EntityComponentSystem &ecs, Camera &cam, float aspect)
{
	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 RenderingSystem::getLightSpaceMatrix(glm::vec3 lightPos)
{
	glm::mat4 lightProjection, lightView, lightSpaceMatrix;
	float nearPlane = 300.0f;
	float farPlane = 400.0f;
	lightProjection = glm::ortho(-80.0f, 80.0f, -40.0f, 40.0f, nearPlane, farPlane);
	lightView = glm::lookAt(lightPos, (-1.0f*lightOffset)+lightPos+glm::vec3(0.f, -100.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}

void RenderingSystem::drawShadowMap(EntityComponentSystem &ecs, Camera &cam, float aspect, glm::vec3 lightPos)
{
	ShaderProgram &shader = *ShaderProvider::depthShader;
	shader.use();
	glm::mat4 lightSpaceMatrix = getLightSpaceMatrix(lightPos);
	unsigned int lightSpaceMatrixLoc = glGetUniformLocation(shader, "lightSpaceMatrix");
	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = cam.getView();
	glm::vec3 camPos = cam.getPos();

	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 1000.f);
	glm::vec3 lightCol = glm::vec3(1.f, 1.f, 1.f);

	int numRendered = 0;

	for (Entity *entity : ecs.getAll())
	{
		if (entity->shouldRender){
		if (!regex_match(entity->name, regex("(ground)(.*)"))&&!regex_match(entity->name,regex("(ray)(.*)")))
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, entity->transform->position);
			model = model * glm::toMat4(entity->transform->rotation);
			model = glm::scale(model, entity->scale);

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

			if (regex_match(entity->name, regex("(car)(.*)")))
			{
				// LogWriter::log("Rendering car \"" + entity->name + "\"");
				location = glGetUniformLocation(shader, "hitVisible");
				if (((AIVehicle *)ecs[entity->name])->hitVisible())
				{
					// LogWriter::log("	Hit is visible.");
					glUniform1f(location, 1.0f);
				}
				else
				{
					// LogWriter::log("	Hit is not visible.");
					glUniform1f(location, 0.0f);
				}
			}
			else
			{
				location = glGetUniformLocation(shader, "hitVisible");
				glUniform1f(location, 0.0f);
			}
			entity->model->shadowDraw(shader);
			numRendered++;
		}
	}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::renderDepth(EntityComponentSystem &ecs, Camera &cam, float aspect)
{

	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ShaderProgram &shader = *ShaderProvider::shadowShader;

	shader.use();
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	GPU_Geometry gpuGeom;
	std::vector<Vertex> verts{
			Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.0f, -1.f), glm::vec2(0.0f, 1.0f)),
			Vertex(glm::vec3(-1.f, -1.0f, 0.0f), glm::vec3(0.f, 0.0f, -1.f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f))};
	gpuGeom.bind();
	gpuGeom.setVerts(verts);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glDisable(GL_FRAMEBUFFER_SRGB);
}