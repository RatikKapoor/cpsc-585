#pragma once

#include <vector>
#include "ECS.h"
#include "Obstacle.h"
#include "ModelProvider.h"
#include "ShaderProvider.h"

class ObstacleHandler {
public:
	static void renderObstacles(EntityComponentSystem&, PhysicsProvider*);

private:
	static std::vector<Obstacle*> obstacleList;
};
