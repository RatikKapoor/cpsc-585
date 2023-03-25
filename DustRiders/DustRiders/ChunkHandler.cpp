#include "ChunkHandler.h"

EntityComponentSystem* ChunkHandler::ecs;
PhysicsProvider* ChunkHandler::physics;

void ChunkHandler::initialize(EntityComponentSystem& system, PhysicsProvider* provider) {
	ecs = &system;
	physics = provider;

	(*ecs)["ground"] = new Ground("ground", ModelProvider::straightPath, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.f, 0.f), 0);
	(*ecs)["ground2"] = new Ground("ground2", ModelProvider::dividedPath, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.f, 300.f), 0);
	(*ecs)["ground3"] = new Ground("ground3", ModelProvider::straightPath, ShaderProvider::carShader, glm::vec3(1.f), physics, PxVec3(0.f, 0.f, 600.f), 0);
}