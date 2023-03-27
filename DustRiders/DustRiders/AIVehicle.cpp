#include "AIVehicle.h"
#include "RayBeam.h"
#include "LogWriter.h"

AIVehicle::AIVehicle(std::string name,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 pos,
	unsigned int matIdx,
	RayBeam* rb,
	std::string pathFile) : Vehicle(name, m, sp, s, pp, pos, matIdx, rb)
{
	this->shouldFindNewDest = true;
	dest = glm::vec3(0);

	if (pathFile.size() > 0) {
		locations = AIPathHandler::deserialize(pathFile);
		this->shouldFindNewDest = false;
	}
}

AIVehicle::~AIVehicle() {}

bool AIVehicle::isClose(glm::vec3 a, glm::vec3 b)
{
	auto threshold = 10;
	auto dx = abs(b.x - a.x);
	auto dy = abs(b.y - a.y);
	auto dz = abs(b.z - a.z);

	auto dist = sqrt(dx * dx + dy * dy + dz * dz);

	if (dist < threshold)
		return true;
	return false;
}

void AIVehicle::handlePathfind()
{
#pragma region Automatic Next Destination Handling
	if (transform->position.z > dest.z - Constants->aiCarNextPositionDistanceBuffer)
	{
		dest.z = transform->position.z;
		shouldFindNewDest = true;
	}

	if (shouldFindNewDest || wasReset)
	{
		if (wasReset) {
			LogWriter::log("was reset");
			dest.z = 0;
			nextLocPtr = 0;
		}

		if (locations.size() > nextLocPtr) {
			this->dest = locations[nextLocPtr++];
		}
		else
		{
			// Handle in case we're out of points
			float random = ((float)rand()) / (float)RAND_MAX;
			float r = random * Constants->aiCarRandomPositionWidth;
			float x = (transform->position.x < 0) ? transform->position.x + r : transform->position.x - r;

			this->dest = glm::vec3(x, 0.f, dest.z + Constants->aiCarRandomPositionSpacing);
		}
		wasReset = false;
		shouldFindNewDest = false;
	}
#pragma endregion

#pragma region Direction Calculations
	glm::quat rotation = transform->rotation;
	glm::mat4 rotationM = glm::toMat4(rotation);
	auto vanHeadingTemp = glm::vec3(rotationM * glm::vec4(0.f, 0.f, -1.f, 1.f));
	PxVec3 vanHeading = PxVec3(vanHeadingTemp.x, vanHeadingTemp.y, vanHeadingTemp.z);

	// Get vehicle position
	physx::PxVec3 pos = gVehicle.mPhysXState.physxActor.rigidBody->getGlobalPose().p;

	// Calculate vector between vehicle's current pos and waypoint
	physx::PxVec3 target;
	target.x = dest.x - pos.x;
	target.y = dest.y - pos.y;
	target.z = dest.z - pos.z;

	// Normalize vector
	target.normalize();

	// Check if vehicle is heading in right direction
	float dot = target.dot(vanHeading);
	if (sqrt(dot * dot) > 0.95f)
	{
		gVehicle.mCommandState.steer = 0.f;
	}
	else
	{
		physx::PxVec3 cross = vanHeading.cross(target);
		cross.normalize();
		if (cross.y < 0)
		{
			gVehicle.mCommandState.steer = 0.2f;
		}
		else
		{
			gVehicle.mCommandState.steer = -0.2f;
		}
	}
#pragma endregion

	auto accel = (double)std::rand() / RAND_MAX * 0.5 + 0.4;
	gVehicle.mCommandState.throttle = accel;

	if (isClose(transform->position, dest))
	{
		this->shouldFindNewDest = true;
	}

#pragma region Ray Gun

	if (flags["beamHit"])
	{
		flags["beamHit"] = false;
		brakeCounter = 30;
	}
	if (brakeCounter > 0) {
		gVehicle.mCommandState.brakes[0] = 1;
		gVehicle.mCommandState.throttle = 0;
		brakeCounter--;
	}

#pragma endregion
}

void AIVehicle::stepPhysics(double timeStep)
{
	handlePathfind();
	updateEffects(timeStep);
	gVehicle.mPhysXState.physxActor.rigidBody->setMaxLinearVelocity(Constants->vehicleInitialMaxLinearVelocity * pow(Constants->vehicleChunkAccelerationBaseExponent, ChunkHandler::chunkCounter));
	gVehicle.step(timeStep, gVehicleSimulationContext);
	handleShooting(timeStep);
}

void AIVehicle::handleShooting(double timeStep){

	// If raygunBeam does not exist
	if(rayGunBeam==NULL){
		return;
	}

	// If the raygun beam is active (primarily for graphics purposes)


	// If aim delay is not currently active
	// Check for target
	if(aimDelay<=0){
		targetSpotted = rayGunBeam->targetInRange();
		if(!targetSpotted){
			return;
		}else{
			aimDelay = Constants->aiRaygunShootingDelay;
			return;
		}
	}

	aimDelay-=timeStep;

	if(aimDelay<=0 && rayGunBeam->canFire()){
		if(((float)rand()/(float)RAND_MAX)>0.5){
		rayGunBeam->castRayBeam();
		}
	}

}
