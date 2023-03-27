#pragma once
#include "PxPhysicsAPI.h"

#include "PhysicsEntity.h"
#include "PhysicsProvider.h"

#include "ECS.h"

#include "TimeKeeper.h"
#include "SoundSource.h"
#include "SoundBuffer.h"

class RayBeamCallback;

class RayBeam : public PhysicsEntity
{
public:
	RayBeam(std::string n,
		Model* m,
		ShaderProgram* sp,
		glm::vec3 s,
		PhysicsProvider* pp,
		EntityComponentSystem& ecs,
		PxVec3 pos,
		unsigned int mat);

	virtual ~RayBeam() {};
	bool isActive;

	void updatePos(PxTransform, Transform*);

	PxRigidDynamic* rcOrigin;
	PxRigidDynamic* rcDirection;

	RayBeamCallback* beamCallback;

	PxVec3 rayCastOrigin;
	PxTransform beamRotation;
	PxVec3 rayCastDirection;
	PxTransform vehiclePos;

	unsigned short beamFrameCount;

	std::string castRayBeam();
	bool targetInRange();

	bool canFire();

protected:
	void initBeam(PxVec3 pos, EntityComponentSystem& ecs);
	EntityComponentSystem& ecs;
	PxVec3 beamOriginOffset;
	PxVec3 beamDirRaw;
	double lastFireTime;
	double coolDownTime;
	TimeKeeper timeKeep;
	SoundSource* raybeamFireSpeaker;
	SoundSource* raybeamHitSpeaker;
	uint32_t raybeamFireSound;
	uint32_t raybeamHitSound;
};

//class RayBeamCallback : public PxSimulationEventCallback
//{
//public:
//	RayBeamCallback(EntityComponentSystem& ecs, RayBeam& rb) : PxSimulationEventCallback(), ecs(ecs), rb(rb) {};
//
//	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {};
//
//	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {};
//
//	void onWake(PxActor** actors, PxU32 count) {};
//	void onSleep(PxActor** actors, PxU32 count) {};
//
//	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {};
//
//	EntityComponentSystem& ecs;
//
//	RayBeam& rb;
//};