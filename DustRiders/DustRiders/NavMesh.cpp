#include "NavMesh.h"

NavMesh::NavMesh() {
	this->nodes = new std::map<unsigned int, Node*>();

	// [v6][v7][v8]
	// [v3][v4][v5]
	// [v0][v1][v2]

	glm::vec3 v0 = glm::vec3(18, 102.5, 252);
	glm::vec3 v1 = glm::vec3(0, 102.5, 252);
	glm::vec3 v2 = glm::vec3(-18, 102.5, 252);

	glm::vec3 v3 = glm::vec3(18, 102.5, 270);
	glm::vec3 v4 = glm::vec3(0, 102.5, 270);
	glm::vec3 v5 = glm::vec3(-18, 102.5, 270);

	glm::vec3 v6 = glm::vec3(18, 102.5, 288);
	glm::vec3 v7 = glm::vec3(0, 102.5, 288);
	glm::vec3 v8 = glm::vec3(-18, 102.5, 288);

	// [n4 / n5][n6 / n7]
	// [n0 / n1][n2 / n3]

	Node* n0 = new Node(0, v0, v3, v4);
	Node* n1 = new Node(1, v0, v1, v4);

	Node* n2 = new Node(2, v1, v4, v5);
	Node* n3 = new Node(3, v1, v2, v5);

	Node* n4 = new Node(4, v3, v7, v6);
	Node* n5 = new Node(5, v3, v4, v7);

	Node* n6 = new Node(6, v4, v8, v7);
	Node* n7 = new Node(7, v4, v5, v8);

	// Create map of nodes
	this->nodes->insert({ n0->id, n0 });
	this->nodes->insert({ n1->id, n1 });
	this->nodes->insert({ n2->id, n2 });
	this->nodes->insert({ n3->id, n3 });
	this->nodes->insert({ n4->id, n4 });
	this->nodes->insert({ n5->id, n5 });
	this->nodes->insert({ n6->id, n6 });
	this->nodes->insert({ n7->id, n7 });

	// Node 0 connections
	n0->connections->emplace_back(std::make_pair(cost(n0, n1), n1));
	n0->connections->emplace_back(std::make_pair(cost(n0, n5), n5));

	// Node 1 connections
	n1->connections->emplace_back(std::make_pair(cost(n1, n0), n0));
	n1->connections->emplace_back(std::make_pair(cost(n1, n2), n2));
}

NavMesh::~NavMesh() {
	delete this->nodes;
}

float NavMesh::cost(Node* src, Node* dest)
{
	// Get controid of nodes
	glm::vec3 srcCenter = src->v0 + src->v1 + src->v2 / 3.f;
	glm::vec3 destCenter = dest->v0 + dest->v1 + dest->v2 / 3.f;

	// Calculate euclidian distance between center points
	float dx = glm::abs(srcCenter.x - destCenter.x);
	float dy = glm::abs(srcCenter.y - destCenter.y);
	float dz = glm::abs(srcCenter.z - destCenter.z);

	return glm::sqrt(dx * dx + dy * dy + dz * dz);
}

Node* NavMesh::findEntity(glm::vec3 pos)
{
	float min = FLT_MAX;
	Node* result = NULL;

	for (unsigned int i = 0; i < this->nodes->size(); i++)
	{
		Node* aNode = this->nodes->find(i)->second;

		float angle = 0.f;

		glm::vec3 a = aNode->v0 - pos;
		glm::vec3 b = aNode->v1 - pos;
		glm::vec3 c = aNode->v2 - pos;

		angle += acos(dot(a, b) / (length(a) * length(b)));
		angle += acos(dot(a, c) / (length(a) * length(c)));
		angle += acos(dot(b, c) / (length(b) * length(b)));

		if (abs(angle - (2 * 3.1415926)) < min)
		{
			min = abs(angle - (2 * 2.31415926));
			result = aNode;
		}
	}

	return result;
}