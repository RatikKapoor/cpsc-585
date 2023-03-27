#include "RoadObjectHandler.h"

std::vector<TriggerEntity*> RoadObjectHandler::obstacleList;
EntityComponentSystem* RoadObjectHandler::ecs;
PhysicsProvider* RoadObjectHandler::physics;
unsigned int RoadObjectHandler::obstacleCounter = 0;

void RoadObjectHandler::initialize(EntityComponentSystem& system, PhysicsProvider* provider) {
	ecs = &system;
	physics = provider;
}

void RoadObjectHandler::reset() {
	for (auto& obstacle : obstacleList) {
		ecs->erase(obstacle->name);
	}
	obstacleList.clear();
	obstacleCounter = 0;
}

void RoadObjectHandler::addObstacles(float minDistance, float maxDistance) {
	for (float dist = minDistance; dist <= maxDistance; dist += 20.0f)
	{
		std::string name = "speedUpZone" + std::to_string(obstacleCounter);
		float random = ((float)rand()) / (float)RAND_MAX;
		float r = random * 40.f;
		float x = -20.f + r;
		float random2 = ((float)rand()) / (float)RAND_MAX;
		float r2 = random2 * 2.f;
		float z = -1.f + r;
		Model* rock;
		switch (obstacleCounter % 3)
		{
		case 0:
			rock = ModelProvider::rock1;
			break;
		case 1:
			rock = ModelProvider::rock2;
			break;
		case 2:
		default:
			rock = ModelProvider::rock3;
			break;
		}
		(*ecs)[name] = new TriggerEntity(name,
			rock,
			ShaderProvider::carShader,
			glm::vec3(1.f),
			physics,
			PxVec3(x, 0.f, (obstacleCounter % 5 == 0) ? dist + z : dist - z),
			1);

		obstacleList.push_back((TriggerEntity*)(*ecs)[name]);

		obstacleCounter++;
	}
}