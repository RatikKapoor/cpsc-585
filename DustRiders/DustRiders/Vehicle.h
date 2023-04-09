#pragma once

#include "PxPhysicsAPI.h"
#include "vehicle2/PxVehicleAPI.h"

#include "snippetvehicle2common/directdrivetrain/DirectDrivetrain.h"
#include "snippetvehicle2common/serialization/BaseSerialization.h"
#include "snippetvehicle2common/serialization/DirectDrivetrainSerialization.h"
#include "snippetvehicle2common/SnippetVehicleHelpers.h"

#include "InputHandler.h"
#include "PhysicsProvider.h"
#include "PhysicsEntity.h"
#include "RayBeam.h"
#include "ChunkHandler.h"

using namespace snippetvehicle2;

using namespace physx;
using namespace physx::vehicle2;

// Commands are issued to the vehicle in a pre-choreographed sequence.
struct Command
{
	PxF32 brake;
	PxF32 throttle;
	PxF32 steer;
	PxF32 duration;
};

class Vehicle : public PhysicsEntity
{
public:
	Vehicle(std::string,
		Model*,
		ShaderProgram*,
		glm::vec3,
		PhysicsProvider*,
		PxVec3,
		unsigned int,
		RayBeam*,
		Joystick*);

	virtual ~Vehicle() {}

	bool engineGased();
	void stepPhysics(double);

	float currentSpeed();

	void reloadTuning();

	void reset();
	void suspend();
	void restore();
	void updateRayBeamPos();
	void saveLocation();

	void applySlowdownEffect(double seconds);
	void applySpeedupEffect(double seconds);

	bool hitVisible();
	void setHitVisible(double seconds);

	void removeEffects();

protected:
	bool wasReset;
	void updateEffects(double deltaT);
	bool initVehicle(PxVec3);
	void initMaterialFrictionTable();
	void stepPhysics(double, float, float);

	double lastEffectTime;
	double visibleHit;

	double slowdownTimeRemaining;
	double speedupTimeRemaining;

	RayBeam* rayGunBeam;
	Joystick* js;

	// The path to the vehicle json files to be loaded.
	const char* gVehicleDataPath = "./media/vehicledata";

	// The vehicle with direct drivetrain
	DirectDriveVehicle gVehicle;

	// Vehicle simulation needs a simulation context
	// to store global parameters of the simulation such as
	// gravitational acceleration.
	PxVehiclePhysXSimulationContext gVehicleSimulationContext;

	// Gravitational acceleration
	const PxVec3 gGravity = PxVec3(0.0f, -9.81f, 0.0f);

	// The mapping between PxMaterial and friction.
	PxVehiclePhysXMaterialFriction gPhysXMaterialFrictions[16];
	PxU32 gNbPhysXMaterialFrictions = 1;
	PxReal gPhysXDefaultMaterialFriction = 1.0f;

	PxRigidBodyFlags initialFlags;
	PxActorFlags initialActorFlags;

	PxVec3 initPos;

private:
	float lastZ = 0.f;
};
