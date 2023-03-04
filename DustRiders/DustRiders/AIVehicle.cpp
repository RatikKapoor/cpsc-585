#include "AIVehicle.h"

AIVehicle::AIVehicle(std::string name,
	Transform* t,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 pos,
	unsigned int matIdx,
	NavMesh* navMesh) : Vehicle(name, t, m, sp, s, pp, pos, matIdx)
{
	//this->aiJS = new Joystick(-1000, true);
	this->pathfinder = new Pathfinder(navMesh);
}

AIVehicle::~AIVehicle() {
	//delete this->aiJS;
	delete this->pathfinder;
}

bool AIVehicle::isClose(glm::vec3 a, glm::vec3 b) {
	auto threshold = 10;

	if (abs(b.x - a.x) < threshold &&
		abs(b.y - a.y) < threshold &&
		abs(b.z - a.z) < threshold)
		return true;
	return false;
}

void AIVehicle::handlePathfind() {
	dest = this->path.front();

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
			gVehicle.mCommandState.steer = 1.f;
		}
		else
		{
			gVehicle.mCommandState.steer = -1.f;
		}
	}

	auto accel = (double)std::rand() / RAND_MAX * 0.5 + 0.2;
	gVehicle.mCommandState.throttle = accel;

	if (isClose(transform->position, dest) && this->path.size() > 1) {
		this->path.erase(this->path.begin());
	}
	if (this->path.size() < 2) {
		gVehicle.mCommandState.steer = 0.f;
	}
}

void AIVehicle::stepPhysics(double timeStep) {
	handlePathfind();

	//float axisThreshold = 0.15f;
	//const float *analogs = this->aiJS->getAnalogs();

	//gVehicle.mCommandState.nbBrakes = 1;

	//if (aiJS->getButton(Xbox::Button::XBOX_A))
	//{

	//	std::cout << "Weapon Fired" << std::endl;
	//	if (aiJS->isPseudo())
	//		aiJS->releaseEnter();
	//}

	//if (aiJS->getButton(Xbox::Button::XBOX_UP))
	//{
	//	if (aiJS->isPseudo())
	//	{
	//		aiJS->releaseR();
	//	}
	//}

	//float currentSpeed = gVehicle.mBaseState.tireSpeedStates->speedStates[0];
	//physx::vehicle2::PxVehicleDirectDriveTransmissionCommandState::Enum gearState = gVehicle.mTransmissionCommandState.gear;
	//gVehicle.mTransmissionCommandState.gear = gearState;

	//// By default, no throttle and turn on the brakes to slow down (and stop) the vehicle
	//float throttle = 0.f;
	//int brake = 1;

	//if (aiJS->getButtonRaw(Xbox::Button::XBOX_LB))
	//{
	//	gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eREVERSE;
	//}
	//else
	//{
	//	gVehicle.mTransmissionCommandState.gear = gVehicle.mTransmissionCommandState.eFORWARD;
	//}

	//throttle = analogs[Xbox::Analog::XBOX_R_TRIGGER] + 1.f;
	//throttle /= 2.f;
	//brake = (int)(analogs[Xbox::Analog::XBOX_L_TRIGGER] > -1.f);

	//gVehicle.mCommandState.throttle = throttle;
	//gVehicle.mCommandState.brakes[0] = brake;

	//gVehicle.mCommandState.steer = -1.f * analogs[Xbox::Analog::XBOX_L_XAXIS]; // Need to flip the direction of the input
	gVehicle.step(timeStep, gVehicleSimulationContext);
}
