#pragma once

#include "ECS.h"
#include "ShaderProvider.h"
#include "ModelProvider.h"
#include "PhysicsProvider.h"
#include "Ground.h"
#include "ObstacleHandler.h"

#define CHUNK_SIZE 300.0
#define CHUNK_LOADING_DISTANCE 100.0

class ChunkHandler {
public:
	static void initialize(EntityComponentSystem&, PhysicsProvider*);
	static void reset();
	static void updateChunks(Entity*);

private:
	static void setupFirstChunk();

	static EntityComponentSystem* ecs;
	static PhysicsProvider* physics;
	static unsigned int chunkCounter;
	static float maxChunkDistance;
	static std::vector<Entity*> chunkList;
};
