#pragma once

#include "ECS.h"
#include "ShaderProvider.h"
#include "ModelProvider.h"
#include "PhysicsProvider.h"
#include "Ground.h"

#define CHUNK_SIZE 300.0
#define CHUNK_LOADING_DISTANCE 50.0

class ChunkHandler {
public:
	static void initialize(EntityComponentSystem&, PhysicsProvider*);

	static void updateChunks(Entity*);

private:
	static EntityComponentSystem* ecs;
	static PhysicsProvider* physics;
	static unsigned int chunkCounter;
	static float maxChunkDistance;
};
