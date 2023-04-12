#include "Vehicle.h"
#include "RayBeam.h"
#include "SoundSource.h"
#include "SoundBuffer.h"

#include "LogWriter.h"
#include "AIPathHandler.h"

Vehicle::Vehicle(std::string n,
								 Model *m,
								 ShaderProgram *sp,
								 glm::vec3 s,
								 PhysicsProvider *pp,
								 PxVec3 startingPos = {0.f, 0.f, 0.f},
								 unsigned int mat = 0,
								 RayBeam *rayGunBeam = NULL,
								 Joystick *js = NULL) : PhysicsEntity(n, m, sp, s, pp, startingPos, mat), rayGunBeam(rayGunBeam), js(js)
{
	initVehicle(startingPos);
}

void Vehicle::initMaterialFrictionTable()
{
	// Each physx material can be mapped to a tire friction value on a per tire basis.
	// If a material is encountered that is not mapped to a friction value, the friction value used is the specified default value.
	// In this snippet there is only a single material so there can only be a single mapping between material and friction.
	// In this snippet the same mapping is used by all tires.
	gPhysXMaterialFrictions[0].friction = Constants->vehicleTireFriction;
	gPhysXMaterialFrictions[0].material = this->gMaterial;
	gPhysXDefaultMaterialFriction = 1.0f;
	gNbPhysXMaterialFrictions = 1;
}

bool Vehicle::initVehicle(PxVec3 p)
{
	initMaterialFrictionTable();

	initPos = p;
	visibleHit = 0.0;

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
	gVehicle.mPhysXState.physxActor.rigidBody->setName(name.c_str());

	// Apply a start pose to the physx actor and add it to the physx scene.
	PxTransform pose(p, PxQuat(PxIdentity));
	gVehicle.setUpActor(*gScene, pose, name.c_str());
	physicsProvider->AddEntity((PxRigidDynamic *)gVehicle.mPhysXState.physxActor.rigidBody, this->transform);

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
	gVehicle.mPhysXState.physxActor.rigidBody->setMaxLinearVelocity(Constants->vehicleInitialMaxLinearVelocity);
	gVehicle.mPhysXState.physxActor.rigidBody->setMaxAngularVelocity(Constants->vehicleMaxAngularVelocity);
	gVehicle.mPhysXState.physxActor.rigidBody->setAngularDamping(Constants->vehicleAngularDampening);
	gVehicle.mPhysXState.physxActor.rigidBody->setMassSpaceInertiaTensor(PxVec3(Constants->vehicleMassSpaceInertiaTensorX, Constants->vehicleMassSpaceInertiaTensorY, Constants->vehicleMassSpaceInertiaTensorZ));
	gVehicle.mPhysXState.physxActor.rigidBody->setMaxContactImpulse(Constants->vehicleMaxContactImpulse);

	// auto tbt = gVehicle.mPhysXState.physxActor.rigidBody;
	// LogWriter::log("Max Linear Velocity: " + std::to_string(tbt->getMaxLinearVelocity()));
	// LogWriter::log("Max Angular Velocity: " + std::to_string(tbt->getMaxAngularVelocity()));
	// LogWriter::log("Angular Dampening: " + std::to_string(tbt->getAngularDamping()));
	// LogWriter::log("Mass Space Inertia X: " + std::to_string(tbt->getMassSpaceInertiaTensor().x));
	// LogWriter::log("Mass Space Inertia Y: " + std::to_string(tbt->getMassSpaceInertiaTensor().y));
	// LogWriter::log("Mass Space Inertia Z: " + std::to_string(tbt->getMassSpaceInertiaTensor().z));
	// LogWriter::log("Contact Impulse: " + std::to_string(tbt->getMaxContactImpulse()));

	auto shapes = gVehicle.mPhysXState.physxActor.rigidBody->getNbShapes();
	for (physx::PxU32 i = 0; i < shapes; i++)
	{
		physx::PxShape *shape = NULL;
		gVehicle.mPhysXState.physxActor.rigidBody->getShapes(&shape, 1, i);

		shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	initialFlags = gVehicle.mPhysXState.physxActor.rigidBody->getRigidBodyFlags();
	initialActorFlags = gVehicle.mPhysXState.physxActor.rigidBody->getActorFlags();

	if (rayGunBeam != NULL)
	{
		rayGunBeam->updatePos(gVehicle.mPhysXState.physxActor.rigidBody->getGlobalPose(), this->transform);
	}
	flames = NULL;

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

bool Vehicle::engineGased()
{
	if (js == nullptr)
		return true;

	const float *analogs = js->getAnalogs();

	if (analogs[Xbox::Analog::XBOX_R_TRIGGER] > 0.0f)
	{

		return true;
	}
	else
	{
		return false;
	}
}

void Vehicle::stepPhysics(double timeStep)
{
	if (js == nullptr)
	{
		throw std::exception("Tried to step physics on player vehicle with no joystick");
	}

	float axisThreshold = 0.15f;
	const float *analogs = js->getAnalogs();
	const unsigned char *b = js->getButtons();

	gVehicle.mCommandState.nbBrakes = 1;

	if (js->getButtonRaw(Xbox::Button::XBOX_A))
	{
		if (!rayGunBeam->isActive)
		{
			if (rayGunBeam != NULL && rayGunBeam->canFire())
			{
				rayGunBeam->castRayBeam();
			}
			rayGunBeam->isActive = true;
		}
		else
		{
			if (rayGunBeam != NULL)
			{
			}
		}
	}
	else
	{
		rayGunBeam->isActive = false;
	}

	if (js->getButton(Xbox::Button::XBOX_UP))
	{
		if (js->isPseudo())
		{
			js->releaseR();
		}
	}

	float currentSpeed = gVehicle.mBaseState.tireSpeedStates->speedStates[0];

	physx::vehicle2::PxVehicleDirectDriveTransmissionCommandState::Enum gearState = gVehicle.mTransmissionCommandState.gear;
	gVehicle.mTransmissionCommandState.gear = gearState;

	// By default, no throttle and turn on the brakes to slow down (and stop) the vehicle
	float throttle = 0.f;
	int brake = 1;

	// If either trigger is engaged
	if (analogs[Xbox::Analog::XBOX_R_TRIGGER] > -0.99f || analogs[Xbox::Analog::XBOX_L_TRIGGER] > -0.99f)
	{
		// If the vehicle is currently heading forward
		if (gVehicle.mTransmissionCommandState.gear == gVehicle.mTransmissionCommandState.eFORWARD)
		{
			// check if vehicle is moving
			if (currentSpeed > 0.5f)
			{
				throttle = analogs[Xbox::Analog::XBOX_R_TRIGGER] + 1.f;
				throttle /= 2.f;
				brake = (int)(analogs[Xbox::Analog::XBOX_L_TRIGGER] > -1.f);
			}
			else
			{
				if (analogs[Xbox::Analog::XBOX_R_TRIGGER] > -1.0f)
				{
					throttle = analogs[Xbox::Analog::XBOX_R_TRIGGER] + 1.f;
					throttle /= 2.f;
					brake = (int)(analogs[Xbox::Analog::XBOX_L_TRIGGER] > -1.f);
				}
				else
				{
					gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eREVERSE;
					throttle = analogs[Xbox::Analog::XBOX_L_TRIGGER] + 1.f;
					throttle /= 2.f;
					brake = (int)(analogs[Xbox::Analog::XBOX_R_TRIGGER] > -1.f);
				}
			}
		}
		// If the vehicle is currently in reverse
		else if (gVehicle.mTransmissionCommandState.gear == gVehicle.mTransmissionCommandState.eREVERSE)
		{
			// Check if vehicle is moving
			if (currentSpeed > 0.5f)
			{
				throttle = analogs[Xbox::Analog::XBOX_L_TRIGGER] + 1.f;
				throttle /= 2.f;
				brake = (int)(analogs[Xbox::Analog::XBOX_R_TRIGGER] > -1.f);
			}
			else
			{
				if (analogs[Xbox::Analog::XBOX_L_TRIGGER] > -1.0f)
				{
					throttle = analogs[Xbox::Analog::XBOX_L_TRIGGER] + 1.f;
					throttle /= 2.f;
					brake = (int)(analogs[Xbox::Analog::XBOX_R_TRIGGER] > -1.f);
				}
				else
				{
					gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eFORWARD;
					throttle = analogs[Xbox::Analog::XBOX_R_TRIGGER] + 1.f;
					throttle /= 2.f;
					brake = (int)(analogs[Xbox::Analog::XBOX_L_TRIGGER] > -1.f);
				}
			}
		}
		else
		{
			gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eFORWARD;

			throttle = analogs[Xbox::Analog::XBOX_R_TRIGGER] + 1.f;
			throttle /= 2.f;
			brake = (int)(analogs[Xbox::Analog::XBOX_L_TRIGGER] > -1.f);
		}
	}
	else // If no triggers are engaged, then turn on the brakes
	{
		throttle = 0.f;
		brake = 1;
	}

	if (flames != NULL)
	{
		flames->setScaleFactor(throttle);
	}

	gVehicle.mCommandState.throttle = throttle;
	gVehicle.mCommandState.brakes[0] = brake;

	gVehicle.mCommandState.steer = -1.f * analogs[Xbox::Analog::XBOX_L_XAXIS]; // Need to flip the direction of the input

	updateEffects(timeStep);
	gVehicle.step(timeStep, gVehicleSimulationContext);
}

// void Vehicle::stepPhysics(double timeStep)
//{
//	auto accel = (double)std::rand() / RAND_MAX * 0.5 + 0.2;
//	updateEffects(timeStep);
//	stepPhysics(timeStep, -accel, 0);
// }

void Vehicle::saveLocation()
{
	if (transform->position.z - lastZ > 5.f)
	{ // Every 5 units log position
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

	gVehicle.mPhysXState.physxActor.rigidBody->setMaxLinearVelocity(Constants->vehicleInitialMaxLinearVelocity);
	gVehicle.mPhysXState.physxActor.rigidBody->setMaxAngularVelocity(Constants->vehicleMaxAngularVelocity);
	gVehicle.mPhysXState.physxActor.rigidBody->setAngularDamping(Constants->vehicleAngularDampening);
	gVehicle.mPhysXState.physxActor.rigidBody->setMassSpaceInertiaTensor(PxVec3(Constants->vehicleMassSpaceInertiaTensorX, Constants->vehicleMassSpaceInertiaTensorY, Constants->vehicleMassSpaceInertiaTensorZ));
	gVehicle.mPhysXState.physxActor.rigidBody->setMaxContactImpulse(Constants->vehicleMaxContactImpulse);

	removeEffects();

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
	gVehicle.setUpActor(*gScene, pose, name.c_str());

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
	if (flames != NULL)
	{
		flames->updatePos(this->transform, false);
	}

	removeEffects();
}

void Vehicle::suspend()
{
	shouldRender = false; // Don't allow rendering to screen
	if (rayGunBeam != NULL)
	{
		rayGunBeam->shouldRender = false;
	}
	if (flames != NULL)
	{
		flames->shouldRender = false;
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
	if (flames != NULL)
	{
		flames->updatePos(this->transform, this->engineGased());
	}
}

float Vehicle::currentSpeed()
{
	return gVehicle.mBaseState.tireSpeedStates->speedStates[0];
}

void Vehicle::applySlowdownEffect(double seconds)
{
	slowdownTimeRemaining += seconds;
	if (slowdownTimeRemaining > Constants->maxEffectTime)
	{
		slowdownTimeRemaining = Constants->maxEffectTime;
	}
}
void Vehicle::applySpeedupEffect(double seconds)
{
	speedupTimeRemaining += seconds;
	if (speedupTimeRemaining > Constants->maxEffectTime)
	{
		speedupTimeRemaining = Constants->maxEffectTime;
	}
}

void Vehicle::removeEffects()
{
	slowdownTimeRemaining = 0.0;
	speedupTimeRemaining = 0.0;
}

void Vehicle::setFlames(Flames *flames)
{
	this->flames = flames;
}

void Vehicle::updateEffects(double deltaT)
{
	if (visibleHit > 0.0)
	{
		visibleHit -= deltaT;
		if (visibleHit < 0.0)
		{
			visibleHit = 0.0;
		}
	}

	if (slowdownTimeRemaining > 0.0)
	{
		gVehicle.mPhysXState.physxActor.rigidBody->addForce(PxVec3(0.f, 0.f, -1.f * Constants->slowDownForce));
		gVehicle.mPhysXState.physxActor.rigidBody->setMaxLinearVelocity(Constants->vehicleInitialMaxLinearVelocity * pow(Constants->vehicleChunkAccelerationBaseExponent, ChunkHandler::chunkCounter) - Constants->effectDeltaVelocity);
	}
	else if (speedupTimeRemaining > 0.0)
	{
		gVehicle.mPhysXState.physxActor.rigidBody->setMaxLinearVelocity(Constants->vehicleInitialMaxLinearVelocity * pow(Constants->vehicleChunkAccelerationBaseExponent, ChunkHandler::chunkCounter) + Constants->effectDeltaVelocity);
		gVehicle.mPhysXState.physxActor.rigidBody->addForce(PxVec3(0.f, 0.f, Constants->speedUpForce));
	}
	else
	{
		gVehicle.mPhysXState.physxActor.rigidBody->setMaxLinearVelocity(Constants->vehicleInitialMaxLinearVelocity * pow(Constants->vehicleChunkAccelerationBaseExponent, ChunkHandler::chunkCounter));
	}
	speedupTimeRemaining -= deltaT;
	slowdownTimeRemaining -= deltaT;
	if (speedupTimeRemaining < 0.0)
	{
		speedupTimeRemaining = 0.0;
	}
	if (slowdownTimeRemaining < 0.0)
	{
		slowdownTimeRemaining = 0.0;
	}
}

bool Vehicle::hitVisible()
{
	if (visibleHit > 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Vehicle::setHitVisible(double seconds)
{
	visibleHit = seconds;
}

bool Vehicle::wasRaybeamJustCharged()
{
	return rayGunBeam->wasJustCharged();
}
