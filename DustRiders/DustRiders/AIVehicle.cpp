#include "AIVehicle.h"
#include "RayBeam.h"

AIVehicle::AIVehicle(std::string name,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 pos,
	unsigned int matIdx, NavMesh* navMesh, RayBeam* rb) : Vehicle(name, m, sp, s, pp, pos, matIdx, rb)
{
	this->pathfinder = new Pathfinder(navMesh);
	this->shouldFindNewDest = true;
}

AIVehicle::~AIVehicle()
{
	// delete this->aiJS;
	delete this->pathfinder;
}

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
	if (transform->position.z > dest.z - 20) {
		dest.z = transform->position.z;
		shouldFindNewDest = true;
	}

	if (shouldFindNewDest || wasReset) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float r = random * 20.f;
		float x = (transform->position.x < 0) ? transform->position.x + r : transform->position.x - r;

		this->dest = glm::vec3(x, 0.f, wasReset ? 100 : dest.z + 100);
		wasReset = false;
		shouldFindNewDest = false;
	}

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

	auto accel = (double)std::rand() / RAND_MAX * 0.5 + 0.4;
	gVehicle.mCommandState.throttle = accel;

	if (isClose(transform->position, dest))
	{
		this->shouldFindNewDest = true;
	}
}

void AIVehicle::stepPhysics(double timeStep)
{
	handlePathfind();
	gVehicle.step(timeStep, gVehicleSimulationContext);
}
