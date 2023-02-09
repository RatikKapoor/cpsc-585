#include "Vehicle.h"

Command gCommands[] =
{
	{0.5f, 0.0f, 0.0f, 2.0f},		//brake on and come to rest for 2 seconds
	{0.0f, 0.5f, 0.0f, 5.0f},		//throttle for 5 seconds
	{0.5f, 0.0f, 0.0f, 5.0f},		//brake for 5 seconds
	{0.0f, 0.5f, 0.0f, 5.0f},		//throttle for 5 seconds
	{0.0f, 0.1f, 0.5f, 5.0f},		//light throttle and steer for 5 seconds.
	{0.5f, 0.0f, 0.0f, 2.f}			//brake for 2s
};
const PxU32 gNbCommands = sizeof(gCommands) / sizeof(Command);
PxReal gCommandTime = 0.0f;			//Time spent on current command
PxU32 gCommandProgress = 0;			//The id of the current command.

Vehicle::Vehicle(PhysicsSystem &ps) {
	this->gPhysics = ps.gPhysics;
	this->gMaterial = ps.gMaterial;
	this->gScene = ps.gScene;

	this->ps = &ps;
}

void Vehicle::initMaterialFrictionTable()
{
	//Each physx material can be mapped to a tire friction value on a per tire basis.
	//If a material is encountered that is not mapped to a friction value, the friction value used is the specified default value.
	//In this snippet there is only a single material so there can only be a single mapping between material and friction.
	//In this snippet the same mapping is used by all tires.
	gPhysXMaterialFrictions[0].friction = 1.0f;
	gPhysXMaterialFrictions[0].material = gMaterial;
	gPhysXDefaultMaterialFriction = 1.0f;
	gNbPhysXMaterialFrictions = 1;
}

bool Vehicle::initVehicle() {
	initMaterialFrictionTable();

	//Load the params from json or set directly.
	readBaseParamsFromJsonFile(gVehicleDataPath, "Base.json", gVehicle.mBaseParams);
	setPhysXIntegrationParams(gVehicle.mBaseParams.axleDescription,
		gPhysXMaterialFrictions, gNbPhysXMaterialFrictions, gPhysXDefaultMaterialFriction,
		gVehicle.mPhysXParams);
	readDirectDrivetrainParamsFromJsonFile(gVehicleDataPath, "DirectDrive.json",
		gVehicle.mBaseParams.axleDescription, gVehicle.mDirectDriveParams);

	//Set the states to default.
	if (!gVehicle.initialize(*gPhysics, PxCookingParams(PxTolerancesScale()), *gMaterial))
	{
		return false;
	}

	gVehicle.mTransmissionCommandState.gear = PxVehicleDirectDriveTransmissionCommandState::eFORWARD;

	//Apply a start pose to the physx actor and add it to the physx scene.
	PxTransform pose(PxVec3(-5.0f, 0.5f, 0.0f), PxQuat(PxIdentity));
	gVehicle.setUpActor(*gScene, pose, gVehicleName);
	this->ps->rigidDynamicList.push_back((PxRigidDynamic*)gVehicle.mPhysXState.physxActor.rigidBody);
	this->ps->transformList.emplace_back(new Transform());
	//gVehicle.mBaseState.;

	//Set up the simulation context.
	//The snippet is set up with
	//a) z as the longitudinal axis
	//b) x as the lateral axis
	//c) y as the vertical axis.
	//d) metres  as the lengthscale.
	gVehicleSimulationContext.setToDefault();
	gVehicleSimulationContext.frame.lngAxis = PxVehicleAxes::ePosZ;
	gVehicleSimulationContext.frame.latAxis = PxVehicleAxes::ePosX;
	gVehicleSimulationContext.frame.vrtAxis = PxVehicleAxes::ePosY;
	gVehicleSimulationContext.scale.scale = 1.0f;
	gVehicleSimulationContext.gravity = gGravity;
	gVehicleSimulationContext.physxScene = gScene;
	gVehicleSimulationContext.physxActorUpdateMode = PxVehiclePhysXActorUpdateMode::eAPPLY_ACCELERATION;
	return true;
}

void Vehicle::stepPhysics(double timestep, CarAction a)
{
	if (gNbCommands == gCommandProgress)
		return;

	//Apply the brake, throttle and steer to the command state of the direct drive vehicle.
	//const Command& command = gCommands[gCommandProgress];
	gVehicle.mCommandState.brakes[0] = (a == BRAKE) ? 1 : 0;
	gVehicle.mCommandState.nbBrakes = 1;
	gVehicle.mCommandState.throttle = (a == ACCEL) ? 1 : 0;
	if (a == LEFT)
		gVehicle.mCommandState.steer = -1;
	else if (a == RIGHT)
		gVehicle.mCommandState.steer = 1;
	else
		gVehicle.mCommandState.steer = 0;

	//Forward integrate the vehicle by a single timestep.
	gVehicle.step(timestep, gVehicleSimulationContext);

	//Forward integrate the phsyx scene by a single timestep.
	//gScene->simulate(timestep);
	//gScene->fetchResults(true);

	//Increment the time spent on the current command.
	//Move to the next command in the list if enough time has lapsed.
	//gCommandTime += timestep;
	//if (gCommandTime > gCommands[gCommandProgress].duration)
	//{
	//	gCommandProgress++;
	//	gCommandTime = 0.0f;
	//}
}