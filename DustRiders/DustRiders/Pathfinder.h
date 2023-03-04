#pragma once

#include <stack>
#include <iostream>
#include <set>

#include "Node.h"
#include "NavMesh.h"

class Pathfinder {
public:
	std::stack<glm::vec3>* path;
	NavMesh* navMesh;

	Pathfinder(NavMesh* navMesh);

	bool search(Node* src, Node* dest);
	glm::vec3 getNextWaypoint();
	bool pathEmpty();

private:
	bool isDestination(Node* src, Node* dest);
	float calculateHCost(Node* src, Node* dest);
	void tracePath(Node* src, Node* dest, std::map<unsigned int, unsigned int> parents);
	void smoothPath(std::vector<glm::vec3> cPoints);
};
