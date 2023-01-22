#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class Model
{
public:
	std::vector<Vertex> verticies;
	glm::mat4 modelMatrix;
};