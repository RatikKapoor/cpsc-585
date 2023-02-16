#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "Geometry.h"
#include "ShaderProgram.h"
#include "Texture.h"

class Mesh
{
public:
	// These vectors are parallel to represent a vertex overall
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	GPU_Geometry geometry;

	void draw(ShaderProgram& shader) {
		// Setup Textures and other uniforms that may be needed
	
		geometry.bind();
		glUniform1i(glGetUniformLocation(shader, "baseSampler"),0);	
		glUniform1i(glGetUniformLocation(shader, "specularTex"), 1);
		glUniform1i(glGetUniformLocation(shader, "emissionTex"), 2);


		for (auto t : textures) {
			if (t.type.compare("texture_specular") == 0) {
				glActiveTexture(GL_TEXTURE1);
				t.bind(); 
			}
			else if(t.type.compare("texture_diffuse") == 0) {
				glActiveTexture(GL_TEXTURE0);
				t.bind();
			}
		}
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		for (auto t : textures) {
			if (t.type.compare("texture_specular") == 0) {
				glActiveTexture(GL_TEXTURE1);
				t.unbind();
			}
			else if (t.type.compare("texture_diffuse") == 0) {
				glActiveTexture(GL_TEXTURE0);
				t.unbind();
			}
		}
		glBindVertexArray(0);
	}


	Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i, std::vector<Texture>& textures)
		: verts(v)
		, indices(i),
		textures(textures)
	{
		std::vector<glm::vec3> norms;
		std::vector<glm::vec2> texCoords;
		for (auto v : verts) {
			norms.push_back(v.normal);
			texCoords.push_back(v.texCoord);
		}
		geometry.bind();
		geometry.setVerts(verts);
		geometry.setNorms(norms);
		geometry.setTexCoords(texCoords);
		geometry.setIndices(indices);
	}
};