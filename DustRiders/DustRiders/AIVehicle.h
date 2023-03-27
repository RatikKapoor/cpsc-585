#pragma once

#include "Vehicle.h"
#include "AIPathHandler.h"

class AIVehicle : public Vehicle
{
public:
	AIVehicle(std::string,
		Model*,
		ShaderProgram*,
		glm::vec3,
		PhysicsProvider*,
		PxVec3,
		unsigned int,
		RayBeam* rb,
		std::string pathFile = "");
	virtual ~AIVehicle();

	void stepPhysics(double);

private:
	void handlePathfind();
	void handleShooting(double timeStep);

	bool targetSpotted;
	double aimDelay;


	bool isClose(glm::vec3, glm::vec3);

	glm::vec3 dest;
	bool shouldFindNewDest;
	int brakeCounter = 0;
	std::vector<vec3> locations;
	int nextLocPtr = 0;
};
