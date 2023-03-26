#include "ChunkHandler.h"

EntityComponentSystem* ChunkHandler::ecs;
PhysicsProvider* ChunkHandler::physics;
unsigned int ChunkHandler::chunkCounter = 0;
float ChunkHandler::maxChunkDistance = 0.f;

void ChunkHandler::initialize(EntityComponentSystem& system, PhysicsProvider* provider) {
	ecs = &system;
	physics = provider;

	(*ecs)["ground0"] = new Ground("ground", ModelProvider::straightPath, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.f, 0.f), 0);
	chunkCounter += 1;
	maxChunkDistance = 150.f;
}

void ChunkHandler::updateChunks(Entity* car) {
	if (maxChunkDistance - car->transform->position.z < (float)CHUNK_LOADING_DISTANCE) {
		auto chunkName = std::string("ground") + std::to_string(chunkCounter);
		(*ecs)[chunkName] = new Ground(chunkName, ModelProvider::straightPath, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.f, maxChunkDistance + 150.f), 0);
		chunkCounter++;
		maxChunkDistance += CHUNK_SIZE;
	}
}