#pragma once

#include "Vehicle.h"
#include "Pathfinder.h"

class AIVehicle : public Vehicle
{
public:
	AIVehicle(std::string,
						Model *,
						ShaderProgram *,
						glm::vec3,
						PhysicsProvider *,
						PxVec3,
						unsigned int, NavMesh *, RayBeam *rb);
	virtual ~AIVehicle();

	void stepPhysics(double);

private:
	void handlePathfind();

	bool isClose(glm::vec3, glm::vec3);

	Pathfinder* pathfinder;
	glm::vec3 dest;
	bool shouldFindNewDest;
	int brakeCounter = 0;
};
