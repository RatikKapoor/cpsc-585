#include "ChunkHandler.h"

EntityComponentSystem* ChunkHandler::ecs;
PhysicsProvider* ChunkHandler::physics;
unsigned int ChunkHandler::chunkCounter = 0;
float ChunkHandler::maxChunkDistance = 0.f;
std::vector<Entity*> ChunkHandler::chunkList;

void ChunkHandler::initialize(EntityComponentSystem& system, PhysicsProvider* provider)
{
	ecs = &system;
	physics = provider;

	RoadObjectHandler::initialize(system, provider);

	setupFirstChunk();
}

void ChunkHandler::reset()
{
	for (auto& chunk : chunkList)
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
	maxChunkDistance = HALF_CHUNK_SIZE;

	RoadObjectHandler::addObstacles(30, HALF_CHUNK_SIZE);
}

void ChunkHandler::updateChunks(Entity* car)
{
	if (maxChunkDistance - car->transform->position.z < (float)CHUNK_LOADING_DISTANCE)
	{
		auto chunkName = std::string("ground") + std::to_string(chunkCounter);
		auto chunkType = randomChunk();
		(*ecs)[chunkName] = new Ground(chunkName, chunkType, ShaderProvider::mapShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.f, maxChunkDistance + HALF_CHUNK_SIZE), 0);
		chunkList.push_back((*ecs)[chunkName]);
		RoadObjectHandler::addObstacles(maxChunkDistance, maxChunkDistance + CHUNK_SIZE);

		chunkCounter++;
		maxChunkDistance += CHUNK_SIZE;
	}
}

Model* ChunkHandler::randomChunk()
{
#ifdef _DEBUG
#ifdef ONE_MODEL
	return ModelProvider::ONE_MODEL;
#endif // ONE_MODEL
#endif // _DEBUG

#ifdef SEQUENTIAL
	static unsigned int counter = 0;
	switch (counter++ % 5)
	{
	case 0:
		return ModelProvider::straightPath;
	case 1:
		return ModelProvider::oneDividedPath;
	case 2:
		return ModelProvider::twiceDividedPath;
	case 3:
		return ModelProvider::archPath;
	case 4:
	default:
		return ModelProvider::curvedPath;
	}
#endif // SEQUENTIAL



	auto r = (float)rand() / RAND_MAX;

	if (r <= 0.2)
	{
		return ModelProvider::straightPath;
	}
	else if (r <= 0.4)
	{
		return ModelProvider::oneDividedPath;
	}
	else if (r <= 0.6)
	{
		return ModelProvider::twiceDividedPath;
	}
	else if (r <= 0.8)
	{
		return ModelProvider::archPath;
	}
	else
	{
		return ModelProvider::curvedPath;
	}
}

std::string ChunkHandler::getChunkForZ(float z)
{
	Model* chunkType = NULL;
	if (z >= maxChunkDistance)
	{
		// I really hope this never happens
		chunkType = chunkList.back()->model;
	}
	else
	{
		auto chunkNumber = (int)std::floor((z + HALF_CHUNK_SIZE) / CHUNK_SIZE);
		if (chunkNumber > chunkCounter) {
			// I also really hope this never happens
			chunkType = chunkList.back()->model;
		}
		else
		{
			chunkType = chunkList[chunkNumber]->model;
		}
	}
	if (chunkType == ModelProvider::straightPath)
		return CHUNK_STRAIGHT;
	else if (chunkType == ModelProvider::oneDividedPath)
		return CHUNK_ONE_DIVIDED;
	else if (chunkType == ModelProvider::twiceDividedPath)
		return CHUNK_TWICE_DIVIDED;
	else if (chunkType == ModelProvider::archPath)
		return CHUNK_ARCH;
	else // Curved
		return CHUNK_CURVED;
}