#include "ChunkHandler.h"

EntityComponentSystem *ChunkHandler::ecs;
PhysicsProvider *ChunkHandler::physics;
unsigned int ChunkHandler::chunkCounter = 0;
float ChunkHandler::maxChunkDistance = 0.f;
std::vector<Entity *> ChunkHandler::chunkList;

void ChunkHandler::initialize(EntityComponentSystem &system, PhysicsProvider *provider)
{
	ecs = &system;
	physics = provider;

	RoadObjectHandler::initialize(system, provider);

	setupFirstChunk();
}

void ChunkHandler::reset()
{
	for (auto &chunk : chunkList)
	{
		ecs->erase(chunk->name);
	}
	chunkList.clear();
	chunkCounter = 0;

	RoadObjectHandler::reset();

	setupFirstChunk();
}

void ChunkHandler::setupFirstChunk()
{
	(*ecs)["ground0"] = new Ground("ground0", ModelProvider::straightPath, ShaderProvider::mapShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.f, 0.f), 0);
	chunkList.push_back((*ecs)["ground0"]);
	chunkCounter = 1;
	maxChunkDistance = 150.f;

	RoadObjectHandler::addObstacles(30, 150);
}

void ChunkHandler::updateChunks(Entity *car)
{
	if (maxChunkDistance - car->transform->position.z < (float)CHUNK_LOADING_DISTANCE)
	{
		auto chunkName = std::string("ground") + std::to_string(chunkCounter);
		(*ecs)[chunkName] = new Ground(chunkName, ModelProvider::straightPath, ShaderProvider::mapShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.f, maxChunkDistance + 150.f), 0);
		chunkList.push_back((*ecs)[chunkName]);
		RoadObjectHandler::addObstacles(maxChunkDistance, maxChunkDistance + CHUNK_SIZE);

		chunkCounter++;
		maxChunkDistance += CHUNK_SIZE;
	}
}