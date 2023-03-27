#pragma once

#include <vector>
#include "ECS.h"
#include "Obstacle.h"
#include "ModelProvider.h"
#include "ShaderProvider.h"
#include "TriggerEntity.h"

class RoadObjectHandler {
public:
	static void initialize(EntityComponentSystem&, PhysicsProvider*);
	static void reset();
	static void addObstacles(float, float);

private:
	static std::vector<TriggerEntity*> obstacleList;
	static EntityComponentSystem* ecs;
	static PhysicsProvider* physics;
	static unsigned int obstacleCounter;
};
