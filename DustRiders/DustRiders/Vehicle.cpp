#include "Vehicle.h"

Command gCommands[] =
		{
				{0.5f, 0.0f, 0.0f, 2.0f}, // brake on and come to rest for 2 seconds
				{0.0f, 0.5f, 0.0f, 5.0f}, // throttle for 5 seconds
				{0.5f, 0.0f, 0.0f, 5.0f}, // brake for 5 seconds
				{0.0f, 0.5f, 0.0f, 5.0f}, // throttle for 5 seconds
				{0.0f, 0.1f, 0.5f, 5.0f}, // light throttle and steer for 5 seconds.
				{0.5f, 0.0f, 0.0f, 2.f}		// brake for 2s
};
const PxU32 gNbCommands = sizeof(gCommands) / sizeof(Command);
PxReal gCommandTime = 0.0f; // Time spent on current command
PxU32 gCommandProgress = 0; // The id of the current command.

Vehicle::Vehicle(PhysicsSystem &ps)
{
	this->gPhysics = ps.gPhysics;
	this->gMaterial = ps.gMaterial;
	this->gScene = ps.gScene;

	this->ps = &ps;
}

void Vehicle::initMaterialFrictionTable()
{
	// Each physx material can be mapped to a tire friction value on a per tire basis.
	// If a material is encountered that is not mapped to a friction value, the friction value used is the specified default value.
	// In this snippet there is only a single material so there can only be a single mapping between material and friction.
	// In this snippet the same mapping is used by all tires.
	gPhysXMaterialFrictions[0].friction = 1.0f;
	gPhysXMaterialFrictions[0].material = gMaterial;
	gPhysXDefaultMaterialFriction = 1.0f;
	gNbPhysXMaterialFrictions = 1;
}

bool Vehicle::initVehicle(PxVec3 p)
{
	initMaterialFrictionTable();

	// Load the params from json or set directly.
	readBaseParamsFromJsonFile(gVehicleDataPath, "Base.json", gVehicle.mBaseParams);
	setPhysXIntegrationParams(gVehicle.mBaseParams.axleDescription,
														gPhysXMaterialFrictions, gNbPhysXMaterialFrictions, gPhysXDefaultMaterialFriction,
														gVehicle.mPhysXParams);
	readDirectDrivetrainParamsFromJsonFile(gVehicleDataPath, "DirectDrive.json",
																				 gVehicle.mBaseParams.axleDescription, gVehicle.mDirectDriveParams);

	// Set the states to default.
	if (!gVehicle.initialize(*gPhysics, PxCookingParams(PxTolerancesScale()), *gMaterial))
	{
		return false;
	}

	gVehicle.mTransmissionCommandState.gear = PxVehicleDirectDriveTransmissionCommandState::eFORWARD;

	// Apply a start pose to the physx actor and add it to the physx scene.
	PxTransform pose(p, PxQuat(PxIdentity));
	gVehicle.setUpActor(*gScene, pose, gVehicleName);
	this->ps->rigidDynamicList.push_back((PxRigidDynamic *)gVehicle.mPhysXState.physxActor.rigidBody);
	this->ps->transformList.emplace_back(new Transform());
	// gVehicle.mBaseState.;

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

	auto shapes = gVehicle.mPhysXState.physxActor.rigidBody->getNbShapes();
	for (physx::PxU32 i = 0; i < shapes; i++)
	{
		physx::PxShape* shape = NULL;
		gVehicle.mPhysXState.physxActor.rigidBody->getShapes(&shape, 1, i);

		shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	return true;
}

void Vehicle::stepPhysics(double timestep, float gas, float steer)
{
	if (gNbCommands == gCommandProgress)
		return;

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

void Vehicle::stepPhysics(double timeStep, Joystick js)
{

	float axisThreshold = 0.15f;
	const float *analogs = js.getAnalogs();
	const unsigned char *buttons = js.getButtons();

	gVehicle.mCommandState.nbBrakes = 1;

	// TODO Add method of checking if button was previously pressed to stop "weapon fired" from being called
	// every time stepPhysics is called.
	if (buttons[XBOX_A])
	{

		std::cout << "Weapon Fired" << std::endl;
	}

	if (gNbCommands == gCommandProgress)
		return;

	float currentSpeed = gVehicle.mBaseState.tireSpeedStates->speedStates[0];
	physx::vehicle2::PxVehicleDirectDriveTransmissionCommandState::Enum gearState = gVehicle.mTransmissionCommandState.gear;
	gVehicle.mTransmissionCommandState.gear = gearState;

	// By default, no throttle and turn on the brakes to slow down (and stop) the vehicle
	float throttle = 0.f;
	int brake = 1;

	if (buttons[XBOX_LB])
	{
		gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eREVERSE;
	}
	else
	{
		gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eFORWARD;
	}

	throttle = analogs[XBOX_R_TRIGGER] + 1.f;
	throttle /= 2.f;
	brake = (int)(analogs[XBOX_L_TRIGGER] > -1.f);

	gVehicle.mCommandState.throttle = throttle;
	gVehicle.mCommandState.brakes[0] = brake;

	gVehicle.mCommandState.steer = -1.f * analogs[XBOX_L_XAXIS]; // Need to flip the direction of the input
	gVehicle.step(timeStep, gVehicleSimulationContext);
}