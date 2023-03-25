#include "Vehicle.h"
#include "RayBeam.h"
#include "SoundSource.h"
#include "SoundBuffer.h"

#include "LogWriter.h"
#include "AIPathHandler.h"

// Command gCommands[] =
//		{
//				{0.5f, 0.0f, 0.0f, 2.0f}, // brake on and come to rest for 2 seconds
//				{0.0f, 0.5f, 0.0f, 5.0f}, // throttle for 5 seconds
//				{0.5f, 0.0f, 0.0f, 5.0f}, // brake for 5 seconds
//				{0.0f, 0.5f, 0.0f, 5.0f}, // throttle for 5 seconds
//				{0.0f, 0.1f, 0.5f, 5.0f}, // light throttle and steer for 5 seconds.
//				{0.5f, 0.0f, 0.0f, 2.f}		// brake for 2s
// };
// const PxU32 gNbCommands = sizeof(gCommands) / sizeof(Command);
// PxReal gCommandTime = 0.0f; // Time spent on current command
// PxU32 gCommandProgress = 0; // The id of the current command.

Vehicle::Vehicle(std::string n,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 startingPos = { 0.f, 0.f, 0.f },
	unsigned int mat = 0, RayBeam* rayGunBeam = NULL) : PhysicsEntity(n, m, sp, s, pp, startingPos, mat), rayGunBeam(rayGunBeam)
{
	initVehicle(startingPos);
}

void Vehicle::initMaterialFrictionTable()
{
	// Each physx material can be mapped to a tire friction value on a per tire basis.
	// If a material is encountered that is not mapped to a friction value, the friction value used is the specified default value.
	// In this snippet there is only a single material so there can only be a single mapping between material and friction.
	// In this snippet the same mapping is used by all tires.
	gPhysXMaterialFrictions[0].friction = 2.0f;
	gPhysXMaterialFrictions[0].material = this->gMaterial;
	gPhysXDefaultMaterialFriction = 1.0f;
	gNbPhysXMaterialFrictions = 1;
}

bool Vehicle::initVehicle(PxVec3 p)
{
	initMaterialFrictionTable();
	initPos = p;

	this->flags["beamHit"] = false;

	// Load the params from json or set directly.
	readBaseParamsFromJsonFile(gVehicleDataPath, "Base.json", gVehicle.mBaseParams);
	setPhysXIntegrationParams(gVehicle.mBaseParams.axleDescription,
		gPhysXMaterialFrictions, gNbPhysXMaterialFrictions, gPhysXDefaultMaterialFriction,
		gVehicle.mPhysXParams);
	readDirectDrivetrainParamsFromJsonFile(gVehicleDataPath, "DirectDrive.json",
		gVehicle.mBaseParams.axleDescription, gVehicle.mDirectDriveParams);

	// Set the states to default.
	if (!gVehicle.initialize(*(physicsProvider->GetPxPhysics()), PxCookingParams(PxTolerancesScale()), *(physicsProvider->GetPxMaterial())))
	{
		return false;
	}

	gVehicle.mTransmissionCommandState.gear = PxVehicleDirectDriveTransmissionCommandState::eFORWARD;

	// Apply a start pose to the physx actor and add it to the physx scene.
	PxTransform pose(p, PxQuat(PxIdentity));
	gVehicle.setUpActor(*gScene, pose, gVehicleName);
	physicsProvider->AddEntity((PxRigidDynamic*)gVehicle.mPhysXState.physxActor.rigidBody, this->transform);
	gVehicle.mPhysXState.physxActor.rigidBody->setName(this->name.c_str());

	// Set up the simulation context.
	// The snippet is set up with
	// a) z as the longitudinal axis
	// b) x as the lateral axis
	// c) y as the vertical axis.
	// d) metres  as the lengthscale.
	gVehicleSimulationContext.setToDefault();
	gVehicleSimulationContext.frame.lngAxis = PxVehicleAxes::ePosZ;
	gVehicleSimulationContext.frame.latAxis = PxVehicleAxes::ePosX;
	gVehicleSimulationContext.frame.vrtAxis = PxVehicleAxes::ePosY;
	gVehicleSimulationContext.scale.scale = 1.0f;
	gVehicleSimulationContext.gravity = gGravity;
	gVehicleSimulationContext.physxScene = gScene;
	gVehicleSimulationContext.physxActorUpdateMode = PxVehiclePhysXActorUpdateMode::eAPPLY_ACCELERATION;
	gVehicle.mPhysXState.physxActor.rigidBody->setMaxLinearVelocity(ConstantsHelper::getConstants().vehicleInitialMaxLinearVelocity);

	auto shapes = gVehicle.mPhysXState.physxActor.rigidBody->getNbShapes();
	for (physx::PxU32 i = 0; i < shapes; i++)
	{
		physx::PxShape* shape = NULL;
		gVehicle.mPhysXState.physxActor.rigidBody->getShapes(&shape, 1, i);

		shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	initialFlags = gVehicle.mPhysXState.physxActor.rigidBody->getRigidBodyFlags();
	initialActorFlags = gVehicle.mPhysXState.physxActor.rigidBody->getActorFlags();

	gVehicle.mPhysXState.physxActor.rigidBody->setAngularDamping(ConstantsHelper::getConstants().vehicleAngularDampening);
	gVehicle.mPhysXState.physxActor.rigidBody->setMassSpaceInertiaTensor(PxVec3(ConstantsHelper::getConstants().test));

	if (rayGunBeam != NULL)
	{
		rayGunBeam->updatePos(gVehicle.mPhysXState.physxActor.rigidBody->getGlobalPose(), this->transform);
	}

	return true;
}

void Vehicle::stepPhysics(double timestep, float gas, float steer)
{
	float analogThreshold = 0.1f;
	gVehicle.mCommandState.nbBrakes = 1;

	// Apply the brake, throttle and steer to the command state of the direct drive vehicle.
	// const Command& command = gCommands[gCommandProgress];

	if (gas > analogThreshold)
	{
		gVehicle.mCommandState.brakes[0] = gas;
		gVehicle.mCommandState.throttle = 0;
	}
	else if (gas < -1.f * analogThreshold)
	{
		gVehicle.mCommandState.brakes[0] = 0;
		gVehicle.mCommandState.throttle = -1 * gas;
	}
	else
	{
		gVehicle.mCommandState.throttle = 0;
		gVehicle.mCommandState.brakes[0] = 0;
	}

	if (steer > analogThreshold)
	{
		gVehicle.mCommandState.steer = -1.f * steer;
	}
	else if (steer < -1.f * analogThreshold)
	{
		gVehicle.mCommandState.steer = -1.f * steer;
	}
	else
	{
		gVehicle.mCommandState.steer = 0;
	}
	// Forward integrate the vehicle by a single timestep.
	gVehicle.step(timestep, gVehicleSimulationContext);
}

bool Vehicle::stepPhysics(double timeStep, Joystick& js)
{
	bool gunFired = false;
	float axisThreshold = 0.15f;
	const float* analogs = js.getAnalogs();
	const unsigned char* b = js.getButtons();

	gVehicle.mCommandState.nbBrakes = 1;

	if (js.getButtonRaw(Xbox::Button::XBOX_A))
	{
		if (!rayGunBeam->isActive)
		{
			gunFired = true;
			if (rayGunBeam != NULL)
			{
				rayGunBeam->shouldRender = true;
			}
			rayGunBeam->isActive = true;
		}
		else
		{
			if (rayGunBeam != NULL)
			{
				rayGunBeam->shouldRender = false;
			}
		}
	}
	else
	{
		rayGunBeam->isActive = false;
	}

	if (js.getButton(Xbox::Button::XBOX_UP))
	{
		if (js.isPseudo())
		{
			js.releaseR();
		}
	}

	float currentSpeed = gVehicle.mBaseState.tireSpeedStates->speedStates[0];
	physx::vehicle2::PxVehicleDirectDriveTransmissionCommandState::Enum gearState = gVehicle.mTransmissionCommandState.gear;
	gVehicle.mTransmissionCommandState.gear = gearState;

	// By default, no throttle and turn on the brakes to slow down (and stop) the vehicle
	float throttle = 0.f;
	int brake = 1;

	if (js.getButtonRaw(Xbox::Button::XBOX_LB))
	{
		gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eREVERSE;
	}
	else
	{
		gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eFORWARD;
	}

	throttle = analogs[Xbox::Analog::XBOX_R_TRIGGER] + 1.f;
	throttle /= 2.f;
	brake = (int)(analogs[Xbox::Analog::XBOX_L_TRIGGER] > -1.f);

	gVehicle.mCommandState.throttle = throttle;
	gVehicle.mCommandState.brakes[0] = brake;

	gVehicle.mCommandState.steer = -1.f * analogs[Xbox::Analog::XBOX_L_XAXIS]; // Need to flip the direction of the input
	gVehicle.step(timeStep, gVehicleSimulationContext);

	return gunFired;
}

void Vehicle::stepPhysics(double timeStep)
{
	auto accel = (double)std::rand() / RAND_MAX * 0.5 + 0.2;
	stepPhysics(timeStep, -accel, 0);
}

void Vehicle::saveLocation() {
	if (transform->position.z - lastZ > 5.f) {	// Every 5 units log position
		AIPathHandler::log(transform->position);
		lastZ = transform->position.z;
	}
}

void Vehicle::reloadTuning()
{
	readBaseParamsFromJsonFile(gVehicleDataPath, "Base.json", gVehicle.mBaseParams);
	setPhysXIntegrationParams(gVehicle.mBaseParams.axleDescription,
		gPhysXMaterialFrictions, gNbPhysXMaterialFrictions, gPhysXDefaultMaterialFriction,
		gVehicle.mPhysXParams);
	readDirectDrivetrainParamsFromJsonFile(gVehicleDataPath, "DirectDrive.json",
		gVehicle.mBaseParams.axleDescription, gVehicle.mDirectDriveParams);

	gVehicle.mPhysXState.physxActor.rigidBody->setMaxLinearVelocity(ConstantsHelper::getConstants().vehicleInitialMaxLinearVelocity);
	gVehicle.mPhysXState.physxActor.rigidBody->setAngularDamping(ConstantsHelper::getConstants().vehicleAngularDampening);
	gVehicle.mPhysXState.physxActor.rigidBody->setMassSpaceInertiaTensor(PxVec3(ConstantsHelper::getConstants().vehicleMassSpaceInertiaTensor));

	return;
}

void Vehicle::reset()
{
	wasReset = true;
	shouldRender = true; // Allow rendering to screen

	initMaterialFrictionTable();

	// Load the params from json or set directly.
	readBaseParamsFromJsonFile(gVehicleDataPath, "Base.json", gVehicle.mBaseParams);
	setPhysXIntegrationParams(gVehicle.mBaseParams.axleDescription,
		gPhysXMaterialFrictions, gNbPhysXMaterialFrictions, gPhysXDefaultMaterialFriction,
		gVehicle.mPhysXParams);
	readDirectDrivetrainParamsFromJsonFile(gVehicleDataPath, "DirectDrive.json",
		gVehicle.mBaseParams.axleDescription, gVehicle.mDirectDriveParams);

	// Apply a start pose to the physx actor and add it to the physx scene.
	PxTransform pose(initPos, PxQuat(PxIdentity));
	gVehicle.setUpActor(*gScene, pose, gVehicleName);

	// Set the body to be kinematic (has to be manually moved)
	gVehicle.mPhysXState.physxActor.rigidBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	// Set all velocities (including previous velocities) to 0
	gVehicle.mBaseState.rigidBodyState.linearVelocity = PxVec3(0.f, 0.f, 0.f);
	gVehicle.mBaseState.rigidBodyState.previousLinearVelocity = PxVec3(0.f, 0.f, 0.f);
	gVehicle.mBaseState.rigidBodyState.angularVelocity = PxVec3(0.f, 0.f, 0.f);
	gVehicle.mBaseState.rigidBodyState.previousAngularVelocity = PxVec3(0.f, 0.f, 0.f);

	// Restore previous rigid body flags
	gVehicle.mPhysXState.physxActor.rigidBody->setRigidBodyFlags(initialFlags);

	if (rayGunBeam != NULL)
	{
		rayGunBeam->updatePos(gVehicle.mPhysXState.physxActor.rigidBody->getGlobalPose(), this->transform);
	}
}

void Vehicle::suspend()
{
	shouldRender = false; // Don't allow rendering to screen
	if (rayGunBeam != NULL)
	{
		rayGunBeam->shouldRender = false;
	}
}

void Vehicle::restore()
{
	shouldRender = true; // Allow rendering to screen
											 // if (rayGunBeam != NULL)
											 // {
											 // 	rayGunBeam->isActive = false;
											 // }
}
void Vehicle::updateRayBeamPos()
{

	if (rayGunBeam != NULL)
	{
		rayGunBeam->updatePos(gVehicle.mPhysXState.physxActor.rigidBody->getGlobalPose(), this->transform);
	}
}