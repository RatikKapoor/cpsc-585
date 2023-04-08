#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "rapidjson/document.h"

#include "LogWriter.h"

using namespace rapidjson;

struct Constants
{
	float vehicleInitialMaxLinearVelocity = 28;
	float vehicleMaxAngularVelocity = 100;
	float vehicleAngularDampening = 0.05;
	float vehicleMaxContactImpulse = PX_MAX_F32;
	float vehicleMassSpaceInertiaTensorX = 3200.0f;
	float vehicleMassSpaceInertiaTensorY = 3414.0f;
	float vehicleMassSpaceInertiaTensorZ = 750.0f;
	float vehicleTireFriction = 4.0f;
	float vehicleChunkAccelerationBaseExponent = 1.05f;

	float rocksStartingZ = 30.0;
	float rocksMaxGenerationDistance = 1000.5;
	float rocksSpacing = 20;

	float aiCarNextPositionDistanceBuffer = 20.0;
	float aiCarRandomPositionWidth = 20.0;
	float aiCarRandomPositionSpacing = 100.0;
	float aiRaygunAimingRange = 30.0;
	float aiRaygunShootingDelay = 1.0;
	float aiRaygunShootingChance = 0.5;

	float raygunEffectSeconds = 2;
	float raygunHitVisibleSeconds = 0.5;
	float speedUpEffectSeconds = 5;
	float slowdownEffectSeconds = 5;
	float slowDownForce = 10000.f;
	float speedUpForce = 5000.f;
	float effectDeltaVelocity = 3.f;
	float maxEffectTime = 5.f;
};

class ConstantsHelper {
public:
	ConstantsHelper(std::string filePath = "./assets/constants.json") {};

	static Constants* getConstants() {
		if (!loaded || c == nullptr)
			loadConstants();
		return c;
	}

	static void refreshConstants() {
		loadConstants();
	}

private:
	static std::string filePath;
	static bool loaded;
	static Constants* c;

	static void loadConstants() {
		if (c == nullptr) {
			c = new Constants();
		}

		std::ifstream f(filePath);
		std::stringstream buffer;
		buffer << f.rdbuf();

		Document doc;
		doc.Parse(buffer.str().c_str());

		if (doc.HasParseError()) {
			LogWriter::log("Error parsing JSON");
			loaded = true;
			return;
		}

		c->vehicleInitialMaxLinearVelocity = doc["vehicleInitialMaxLinearVelocity"].GetDouble();
		c->vehicleMaxAngularVelocity = doc["vehicleMaxAngularVelocity"].GetDouble();
		c->vehicleAngularDampening = doc["vehicleAngularDampening"].GetDouble();
		c->vehicleMaxContactImpulse = doc["vehicleMaxContactImpulse"].GetDouble();
		c->vehicleMassSpaceInertiaTensorX = doc["vehicleMassSpaceInertiaTensorX"].GetDouble();
		c->vehicleMassSpaceInertiaTensorY = doc["vehicleMassSpaceInertiaTensorY"].GetDouble();
		c->vehicleMassSpaceInertiaTensorZ = doc["vehicleMassSpaceInertiaTensorZ"].GetDouble();
		c->vehicleTireFriction = doc["vehicleTireFriction"].GetDouble();
		c->vehicleChunkAccelerationBaseExponent = doc["vehicleChunkAccelerationBaseExponent"].GetDouble();

		c->rocksStartingZ = doc["rocksStartingZ"].GetDouble();
		c->rocksMaxGenerationDistance = doc["rocksMaxGenerationDistance"].GetDouble();
		c->rocksSpacing = doc["rocksSpacing"].GetDouble();

		c->aiCarNextPositionDistanceBuffer = doc["aiCarNextPositionDistanceBuffer"].GetDouble();
		c->aiCarRandomPositionWidth = doc["aiCarRandomPositionWidth"].GetDouble();
		c->aiCarRandomPositionSpacing = doc["aiCarRandomPositionSpacing"].GetDouble();
		c->aiRaygunAimingRange = doc["aiRaygunAimingRange"].GetDouble();
		c->aiRaygunShootingDelay = doc["aiRaygunShootingDelay"].GetDouble();
		c->aiRaygunShootingChance = doc["aiRaygunShootingChance"].GetDouble();

		c->raygunEffectSeconds = doc["raygunEffectSeconds"].GetDouble();
		c->raygunHitVisibleSeconds = doc["raygunHitVisibleSeconds"].GetDouble();
		c->speedUpEffectSeconds = doc["speedUpEffectSeconds"].GetDouble();
		c->slowdownEffectSeconds = doc["slowdownEffectSeconds"].GetDouble();
		c->speedUpForce = doc["speedUpForce"].GetDouble();
		c->slowDownForce = doc["slowDownForce"].GetDouble();
		c->effectDeltaVelocity = doc["effectDeltaVelocity"].GetDouble();
		c->maxEffectTime = doc["maxEffectTime"].GetDouble();

		loaded = true;
	}
};
