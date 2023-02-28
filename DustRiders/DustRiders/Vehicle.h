#pragma once

#include "PxPhysicsAPI.h"
#include "vehicle2/PxVehicleAPI.h"
#include "InputHandler.h"

#include "snippetvehicle2common/directdrivetrain/DirectDrivetrain.h"
#include "snippetvehicle2common/serialization/BaseSerialization.h"
#include "snippetvehicle2common/serialization/DirectDrivetrainSerialization.h"
#include "snippetvehicle2common/SnippetVehicleHelpers.h"
#include "PhysicsSystem.h"
using namespace snippetvehicle2;

using namespace physx;
using namespace physx::vehicle2;

// Give the vehicle a name so it can be identified in PVD.
const char gVehicleName[] = "directDrive";

enum CarAction
{
	ACCEL,
	HALF_ACCEL,
	BRAKE,
	LEFT,
	RIGHT,
	IDLE
};

// Commands are issued to the vehicle in a pre-choreographed sequence.
struct Command
{
	PxF32 brake;
	PxF32 throttle;
	PxF32 steer;
	PxF32 duration;
};

class Vehicle
{
public:
	Vehicle(PhysicsSystem &ps);

	bool initVehicle(PxVec3);
	void initMaterialFrictionTable();
	void stepPhysics(double, float, float);
	void stepPhysics(double timeStep, Joystick& js);

	PxPhysics *gPhysics;
	PxMaterial *gMaterial;
	PxScene *gScene;

	// The path to the vehicle json files to be loaded.
	const char *gVehicleDataPath = "./media/vehicledata";

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

private:
	PhysicsSystem *ps;
};
