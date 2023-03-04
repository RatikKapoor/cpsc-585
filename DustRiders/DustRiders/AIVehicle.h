#pragma once

#include "Vehicle.h"
#include "Pathfinder.h"

class AIVehicle : public Vehicle
{
public:
	AIVehicle(std::string,
		Transform*,
		Model*,
		ShaderProgram*,
		glm::vec3,
		PhysicsProvider*,
		PxVec3,
		unsigned int,
		NavMesh*);
	virtual ~AIVehicle();

	void stepPhysics(double);
	std::vector<glm::vec3> path;

private:
	void handlePathfind();

	bool isClose(glm::vec3, glm::vec3);

	//Joystick* aiJS;
	Pathfinder* pathfinder;
	glm::vec3 dest;
};
