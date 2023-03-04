#include "Pathfinder.h"

Pathfinder::Pathfinder(NavMesh* navMesh) {
	this->navMesh = navMesh;
	this->path = new std::stack<glm::vec3>;
}

bool Pathfinder::search(Node* src, Node* dest) {
	if (src == NULL)
	{
		std::cout << "ERROR: src is null" << std::endl;
		std::cout << "ERROR: src is null" << std::endl;
		return false;
	}
	else if (dest == NULL)
	{
		std::cout << "ERROR: dest is null" << std::endl;
		std::cout << "ERROR: dest is null" << std::endl;
		return false;
	}

	// Check for trivial case where we're already in the right spot
	if (isDestination(src, dest))
	{
		std::cout << "Already at destination!" << std::endl;
		return true;
	}

	// Declare data structures
	std::map<unsigned int, bool> explored;
	std::set<std::pair<float, Node*>> frontier;
	std::map<unsigned int, unsigned int> parents;

	// Start by adding the src node to the fronter with 0 cost (already there)
	frontier.insert(std::make_pair(0.f, src));

	// Also add it to the explored list as false
	explored.insert({ frontier.begin()->second->id, false });

	// Add the src node to the parent list
	parents.insert({ src->id, src->id });

	while (!frontier.empty())
	{
		// Save a ref to the first entry in the fronter and remove it
		std::pair<float, Node*> p = *frontier.begin();
		frontier.erase(frontier.begin());

		// Now add it to the explored list
		explored.find(p.second->id)->second = true;

		float gNew = 0;
		float hNew = 0;
		float fNew = 0;

		// Initialize connections in explored list
		for (unsigned int i = 0; i < p.second->connections->size(); i++)
		{
			if (explored.find(p.second->connections->at(i).second->id) == explored.end())
			{
				explored.insert({ p.second->connections->at(i).second->id, false });
			}
		}

		// Iterate through connection nodes
		for (unsigned int i = 0; i < p.second->connections->size(); i++)
		{
			// Get shortcut ref to this node
			std::pair<float, Node*> test = p.second->connections->at(i);

			// Check if this is the destination
			if (isDestination(test.second, dest))
			{
				parents.insert_or_assign(test.second->id, p.second->id);
				this->tracePath(test.second, dest, parents);
				return true;
			}
			else if (!explored.find(test.second->id)->second)
			{
				// Update cost
				// Cost so far + cost of current edge
				gNew = p.first + test.first;				// Actual cost
				hNew = calculateHCost(test.second, dest);	// Heuristic cost
				fNew = gNew = hNew;							// Total cost

				// Add to the frontier
				frontier.insert(std::make_pair(fNew, test.second));
				parents.insert_or_assign(test.second->id, p.second->id);
			}
		}
	}

	std::cout << "The destination cell is not found." << std::endl;
	return false;
}

bool Pathfinder::isDestination(Node* src, Node* dest)
{
	if (src->id == dest->id)
		return true;
	return false;
}

float Pathfinder::calculateHCost(Node* src, Node* dest)
{
	// Get centroid of nodes
	glm::vec3 srcCenter = src->v0 + src->v1 + src->v2 / 3.f;
	glm::vec3 destCenter = dest->v0 + dest->v1 + dest->v2 / 3.f;

	// Calculate euclidian distance between center points
	float dx = glm::abs(srcCenter.x - destCenter.x);
	float dy = glm::abs(srcCenter.y - destCenter.y);
	float dz = glm::abs(srcCenter.z - destCenter.z);

	return glm::sqrt(dx * dx + dy * dy + dz * dz);
}

void Pathfinder::tracePath(Node* src, Node* dest, std::map<unsigned int, unsigned int> parents)
{
	std::cout << "The path is: ";

	std::vector<glm::vec3> bPath;

	while (!this->path->empty())
	{
		this->path->pop();
	}

	unsigned int temp = dest->id;
	while (temp != parents.find(temp)->second)
	{
		bPath.push_back(this->navMesh->nodes->find(temp)->second->centroid);
		temp = parents.find(temp)->second;
	}
	bPath.push_back(this->navMesh->nodes->find(temp)->second->centroid);
}

void Pathfinder::smoothPath(std::vector<glm::vec3> cPoints)
{
	//// Skip attempting to draw curve if only 2 os less points exist
	//if (cPoints.size() < 3)
	//{
	//	std::cout << "Too few control points to smooth path." << std::endl;
	//	return;
	//}

	//std::vector<glm::vec3> iVerts;
	//int degree = cPoints.size();

	//// Clear previous path
	//while (!this->path->empty())
	//{
	//	this->path->pop();
	//}

	//// Open Chaikin algorithm
	//for (unsigned int u = 0; u < 4; u++)
	//{
	//	iVerts.push_back(cPoints[0]);
	//	iVerts.push_back(c2 * cPoints[0] + c2 * cPoints[1]);

	//	// Interpolate up to n-1 remaining points
	//}
}