#include "RoadObjectHandler.h"

std::vector<Obstacle*> RoadObjectHandler::obstacleList;
std::vector<TriggerEntity*> RoadObjectHandler::triggerList;
EntityComponentSystem* RoadObjectHandler::ecs;
PhysicsProvider* RoadObjectHandler::physics;
unsigned int RoadObjectHandler::obstacleCounter = 0;
unsigned int RoadObjectHandler::speedUpZoneCounter = 0;
unsigned int RoadObjectHandler::slowDownZoneCounter = 0;
unsigned int RoadObjectHandler::roadObjectCounter = 0;

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

	for (auto& zone : triggerList) {
		ecs->erase(zone->name);
	}
	triggerList.clear();
	speedUpZoneCounter = 0;
	slowDownZoneCounter = 0;

	roadObjectCounter = 0;
}

void RoadObjectHandler::addObstacles(float minDistance, float maxDistance) {
	for (float dist = minDistance; dist <= maxDistance; dist += 20.0f)
	{
		std::string name = "";
		Model* roadObjectModel;
		enum RoadObjectType type;
		switch (roadObjectCounter % 15)
		{
		case 0:
		case 11:
			// Create speed up zone
			name = "speedUpZone" + std::to_string(speedUpZoneCounter);
			type = RoadObjectType::SpeedUpZone;
			roadObjectModel = ModelProvider::speedupBlock;
			break;
		case 8:
		case 4:
			// Create slow down zone
			name = "slowDownZone" + std::to_string(slowDownZoneCounter);
			type = RoadObjectType::SlowDownZone;
			roadObjectModel = ModelProvider::slowdownBlock;
			break;
		case 3:
		case 5:
		case 10:
		case 13:
			// Create rock
			name = "obstacle" + std::to_string(obstacleCounter);
			type = RoadObjectType::ObstacleRock;
			switch (obstacleCounter % 3)
			{
			case 0:
				roadObjectModel = ModelProvider::rock1;
				break;
			case 1:
				roadObjectModel = ModelProvider::rock2;
				break;
			case 2:
			default:
				roadObjectModel = ModelProvider::rock3;
				break;
			}
			break;
		default:
			type = RoadObjectType::None;
			roadObjectModel = nullptr;
			break;
		}

		// Calculate position
		float random = ((float)rand()) / (float)RAND_MAX;
		float r = random * 40.f;
		float x = -20.f + r;
		float random2 = ((float)rand()) / (float)RAND_MAX;
		float r2 = random2 * 2.f;
		float z = -1.f + r;

		switch (type)
		{
		case SpeedUpZone:
			(*ecs)[name] = new TriggerEntity(name,
				roadObjectModel,
				ShaderProvider::carShader,
				glm::vec3(1.f),
				physics,
				PxVec3(x, 0.5f, (obstacleCounter % 5 == 0) ? dist + z : dist - z),
				0);
			triggerList.push_back((TriggerEntity*)(*ecs)[name]);
			speedUpZoneCounter++;
			break;
		case SlowDownZone:
			(*ecs)[name] = new TriggerEntity(name,
				roadObjectModel,
				ShaderProvider::carShader,
				glm::vec3(1.f),
				physics,
				PxVec3(x, 0.5f, (obstacleCounter % 5 == 0) ? dist + z : dist - z),
				0);
			triggerList.push_back((TriggerEntity*)(*ecs)[name]);
			slowDownZoneCounter++;
			break;
		case ObstacleRock:
			(*ecs)[name] = new Obstacle(name,
				roadObjectModel,
				ShaderProvider::carShader,
				glm::vec3(1.f),
				physics,
				PxVec3(x, -0.425f, (obstacleCounter % 5 == 0) ? dist + z : dist - z),
				1);
			obstacleList.push_back((Obstacle*)(*ecs)[name]);
			obstacleCounter++;
			break;
		default:
			break;
		}

		roadObjectCounter++;
	}
}