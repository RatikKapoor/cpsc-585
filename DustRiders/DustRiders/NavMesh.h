#pragma once

#include <map>

#include "Node.h"

class NavMesh {
public:
	std::map<unsigned int, Node*>* nodes;

	NavMesh();
	~NavMesh();

	Node* findEntity(glm::vec3 pos);

private:
	float cost(Node* src, Node* dest);
};
