#pragma once

#include "ECS.h"
#include "ShaderProvider.h"
#include "ModelProvider.h"
#include "PhysicsProvider.h"
#include "Ground.h"

class ChunkHandler {
public:
	static void initialize(EntityComponentSystem&, PhysicsProvider*);

private:
	static EntityComponentSystem* ecs;
	static PhysicsProvider* physics;
};
