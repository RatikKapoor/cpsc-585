#pragma once

#include "PhysicsEntity.h"
#include "Vehicle.h"
#include "ECS.h"
#include "SoundSource.h"

class SimulationCallback : public PxSimulationEventCallback {
public:
	SimulationCallback(EntityComponentSystem&);

	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {};
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {};
	void onWake(PxActor** actors, PxU32 count) {};
	void onSleep(PxActor** actors, PxU32 count) {};
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {};
	void onTrigger(PxTriggerPair* pairs, PxU32 count);

private:
	EntityComponentSystem& ecs;
	SoundSource* noiseSpeaker;
	uint32_t speedUpSound;
	uint32_t slowDownSound;
};
