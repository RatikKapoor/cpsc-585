#include "ObstacleHandler.h"

std::vector<Obstacle*> ObstacleHandler::obstacleList;

void ObstacleHandler::renderObstacles(EntityComponentSystem& ecs, PhysicsProvider* physics) {
	int rockCount = 0;
	for (float dist = 30; dist <= 1000.5f; dist += 20.0f)
	{
		std::string name = "rock" + std::to_string(rockCount);
		float random = ((float)rand()) / (float)RAND_MAX;
		float r = random * 40.f;
		float x = -20.f + r;
		float random2 = ((float)rand()) / (float)RAND_MAX;
		float r2 = random2 * 2.f;
		float z = -1.f + r;
		Model* rock;
		switch (rockCount % 3)
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
		ecs[name] = new Obstacle(name,
			rock,
			ShaderProvider::carShader,
			glm::vec3(1.f),
			physics,
			PxVec3(x, -0.48f, (rockCount % 5 == 0) ? dist + z : dist - z),
			1);

		obstacleList.push_back((Obstacle*)ecs[name]);

		rockCount++;
	}
}