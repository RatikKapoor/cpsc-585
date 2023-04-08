#pragma once

#include <vector>
#include "ECS.h"
#include "Obstacle.h"
#include "ModelProvider.h"
#include "ShaderProvider.h"
#include "TriggerEntity.h"

enum RoadObjectType
{
	ObstacleRock,
	SpeedUpZone,
	SlowDownZone,
	None
};

class RoadObjectHandler {
public:
	static void initialize(EntityComponentSystem&, PhysicsProvider*);
	static void reset();
	static void addObstacles(float, float);

private:
	static std::vector<Obstacle*> obstacleList;
	static std::vector<TriggerEntity*> triggerList;
	static EntityComponentSystem* ecs;
	static PhysicsProvider* physics;
	static unsigned int obstacleCounter;
	static unsigned int speedUpZoneCounter;
	static unsigned int slowDownZoneCounter;
	static unsigned int roadObjectCounter;
};
