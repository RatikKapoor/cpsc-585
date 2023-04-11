#pragma once
#include "PxPhysicsAPI.h"

#include "PhysicsEntity.h"
#include "PhysicsProvider.h"

#include "ECS.h"

#include "TimeKeeper.h"
#include "SoundSource.h"
#include "SoundBuffer.h"

#ifdef _MSC_VER
#undef GetObject
#endif

class RayBeamCallback;

class RayBeam : public PhysicsEntity
{
public:
	RayBeam(std::string n,
					Model *m,
					ShaderProgram *sp,
					glm::vec3 s,
					PhysicsProvider *pp,
					EntityComponentSystem &ecs,
					PxVec3 pos,
					unsigned int mat);

	virtual ~RayBeam(){};
	bool isActive;

	void updatePos(PxTransform, Transform *);

	PxRigidDynamic *rcOrigin;
	PxRigidDynamic *rcDirection;

	RayBeamCallback *beamCallback;

	PxVec3 rayCastOrigin;
	PxTransform beamRotation;
	PxVec3 rayCastDirection;
	PxTransform vehiclePos;

	unsigned short beamFrameCount;

	std::string castRayBeam();
	bool targetInRange();

	bool canFire();
	bool wasJustCharged();
	int getChargePercentage();

protected:
	void initBeam(PxVec3 pos, EntityComponentSystem &ecs);
	EntityComponentSystem &ecs;
	PxVec3 beamOriginOffset;
	PxVec3 beamDirRaw;
	double lastFireTime;
	double coolDownTime;
	TimeKeeper timeKeep;
	SoundSource *raybeamFireSpeaker;
	SoundSource *raybeamHitSpeaker;
	uint32_t raybeamFireSound;
	uint32_t raybeamHitSound;

private:
	bool wasCharged;
};
