#pragma once

#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Node {
public:
	unsigned int id;
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 centroid;
	std::vector<std::pair<float, Node*>>* connections;

	glm::vec3 getCentroid(Node* node) {
		return (node->v0 + node->v1 + node->v2) / 3.f;
	}

	Node(unsigned int id, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
		this->id = id;
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		this->centroid = getCentroid(this);
		this->connections = new std::vector<std::pair<float, Node*>>;
	}

	~Node() {
		delete connections;
	}
};
