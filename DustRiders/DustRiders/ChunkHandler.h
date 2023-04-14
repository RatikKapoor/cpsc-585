#pragma once

#include "ECS.h"
#include "ShaderProvider.h"
#include "ModelProvider.h"
#include "PhysicsProvider.h"
#include "Ground.h"
#include "RoadObjectHandler.h"
#include "DebugDefines.h"

#define CHUNK_SIZE 300.0
#define HALF_CHUNK_SIZE CHUNK_SIZE / 2
#define CHUNK_LOADING_DISTANCE 100.0

#pragma region Chunk Names
#define CHUNK_STRAIGHT "straight"
#define CHUNK_ONE_DIVIDED "oneDivided"
#define CHUNK_TWICE_DIVIDED "twiceDivided"
#define CHUNK_ARCH "arch"
#define CHUNK_CURVED "curved"
#pragma endregion


class ChunkHandler {
public:
	static void initialize(EntityComponentSystem&, PhysicsProvider*);
	static void reset();
	static void updateChunks(Entity*);
	static std::string getChunkForZ(float);

	static unsigned int chunkCounter;

private:
	static void setupFirstChunk();
	static Model* randomChunk();
	static EntityComponentSystem* ecs;
	static PhysicsProvider* physics;
	static float maxChunkDistance;
	static std::vector<Entity*> chunkList;
};
