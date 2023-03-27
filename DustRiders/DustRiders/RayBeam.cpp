#include "RayBeam.h"
#include "LogWriter.h"
#include "AIVehicle.h"


#include "TimeKeeper.h"
#include <regex>

RayBeam::RayBeam(std::string n,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp, 
	EntityComponentSystem& ecs,
	PxVec3 pos = { 0.f, 0.f, 0.f },
	unsigned int mat = 0) : PhysicsEntity(n, m, sp, s, pp, pos, mat), ecs(ecs)
{
	coolDownTime = 2.0;
	isActive = false;
	shouldRender = false;
	beamOriginOffset = PxVec3(0.f, 1.47f / 2.f, 3.85f);
	beamDirRaw = PxVec3(0.f, 0.f, 1.f);
	initBeam(pos, ecs);
	lastFireTime = 0.0;
	raybeamFireSpeaker = new SoundSource();
	raybeamHitSpeaker = new SoundSource();
	raybeamFireSound = SoundBuffer::get()->addSoundEffect("sound/laser-shoot.wav");
	raybeamHitSound = SoundBuffer::get()->addSoundEffect("sound/collision.ogg");
}

void RayBeam::initBeam(PxVec3 pos, EntityComponentSystem& ecs)
{

	// beamCallback = new RayBeamCallback(ecs, std::ref(*this));

	// gScene->setSimulationEventCallback(beamCallback);

	auto originMarker = this->gPhysics->createShape(PxBoxGeometry(0.1, 0.1, 0.1), *gMaterial);
	auto directionMarker = this->gPhysics->createShape(PxBoxGeometry(0.2, 0.2, 0.2), *gMaterial);


	originMarker->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	originMarker->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	originMarker->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);

	directionMarker->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	directionMarker->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	directionMarker->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);

	// Setting position of the origin marker
	PxTransform localTm(pos + beamOriginOffset);
	rcOrigin = gPhysics->createRigidDynamic(localTm);
	rcOrigin->setGlobalPose(localTm);
	rcOrigin->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	rcOrigin->attachShape(*originMarker);
	PxRigidBodyExt::updateMassAndInertia(*rcOrigin, 10.f);
	gScene->addActor(*rcOrigin);
	physicsProvider->AddEntity(rcOrigin, this->transform);
	originMarker->release();

	// Setting position of the direction marker
	PxTransform localTmDir(pos + beamOriginOffset + beamDirRaw);
	rcDirection = gPhysics->createRigidDynamic(localTmDir);
	rcDirection->setGlobalPose(localTmDir);
	rcDirection->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	rcDirection->attachShape(*directionMarker);
	PxRigidBodyExt::updateMassAndInertia(*rcDirection, 10.f);
	gScene->addActor(*rcDirection);
	physicsProvider->AddEntity(rcDirection, this->transform);
	directionMarker->release();
}

void RayBeam::updatePos(PxTransform vehiclePos, Transform* tf)
{
	this->vehiclePos = vehiclePos;

	PxQuat vehicleQuat = vehiclePos.q;
	PxVec3 vQuatImaginary = vehicleQuat.getImaginaryPart();
	PxMat44 vQuatMat(vehicleQuat);
	PxVec3  vRotation = vQuatMat.transform(PxVec3(1.f, 1.f, 1.f));
	vRotation.normalize();

	// Moves the beam origin marker
	beamRotation = PxTransform(PxTransform(beamOriginOffset).getInverse() * PxTransform(vehiclePos.q) * PxTransform(beamOriginOffset));
	PxTransform localPos(beamOriginOffset + vehiclePos.p);
	this->rcOrigin->setGlobalPose(localPos * beamRotation);
	this->transform = tf;

	// Moves the beam direction marker
	PxTransform beamDirRotation = PxTransform(PxTransform(beamOriginOffset + beamDirRaw).getInverse() * PxTransform(vehiclePos.q) * PxTransform(beamOriginOffset + beamDirRaw));
	PxTransform localDirPos(beamOriginOffset + vehiclePos.p + beamDirRaw);
	this->rcDirection->setGlobalPose(localDirPos * beamDirRotation);


	rayCastOrigin = rcOrigin->getGlobalPose().p;
	rayCastDirection = (rcDirection->getGlobalPose().p - rcOrigin->getGlobalPose().p).getNormalized();
}

std::string RayBeam::castRayBeam()
{
	alSourcePlay(raybeamFireSpeaker->Play(raybeamFireSound));
	std::string closestHit = "";

	PxRaycastBuffer hit;

	float f;
	PxVec3 carRotation = vehiclePos.q.getImaginaryPart();
	carRotation.normalize();

	bool hitStatus = gScene->raycast(rayCastOrigin, rayCastDirection, 200.f, hit);

	if (!canFire()) {
		hitStatus = false;
	}
	else {
		lastFireTime = timeKeep.getCurrentTime();
	}
	if (hitStatus)
	{
		closestHit = hit.block.actor->getName();
		if (regex_match(hit.block.actor->getName(), regex("(car)(.*)"))) {
			LogWriter::log("Raycast Hit: " + std::string(hit.block.actor->getName()));
			((AIVehicle*)ecs[closestHit])->applySlowdownEffect(5);
			alSourcePlay(raybeamHitSpeaker->Play(raybeamHitSound));
		}
	}
	return closestHit;
}

bool RayBeam::canFire() {
	double deltaT = timeKeep.getCurrentTime() - lastFireTime;
	if (deltaT < coolDownTime) {
		LogWriter::log("Cooldown time not elapsed. Only " + std::to_string(deltaT) + " out of " + std::to_string(coolDownTime) + " elapsed.");
		return false;
	}
	else {
		return true;
	}
}