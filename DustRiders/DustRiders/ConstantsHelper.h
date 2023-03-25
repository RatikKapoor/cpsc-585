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
	float vehicleAngularDampening = 0.05;
	float vehicleMassSpaceInertiaTensor = 1.0f;

	float rocksStartingZ = 30.0;
	float rocksMaxGenerationDistance = 1000.5;
	float rocksSpacing = 20;

	float aiCarNextPositionDistanceBuffer = 20.0;
	float aiCarRandomPositionWidth = 20.0;
	float aiCarRandomPositionSpacing = 100.0;
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
		c->vehicleAngularDampening = doc["vehicleAngularDampening"].GetDouble();
		c->vehicleMassSpaceInertiaTensor = doc["vehicleMassSpaceInertiaTensor"].GetDouble();
		
		c->rocksStartingZ = doc["rocksStartingZ"].GetDouble();
		c->rocksMaxGenerationDistance = doc["rocksMaxGenerationDistance"].GetDouble();
		c->rocksSpacing = doc["rocksSpacing"].GetDouble();

		c->aiCarNextPositionDistanceBuffer = doc["aiCarNextPositionDistanceBuffer"].GetDouble();
		c->aiCarRandomPositionWidth = doc["aiCarRandomPositionWidth"].GetDouble();
		c->aiCarRandomPositionSpacing = doc["aiCarRandomPositionSpacing"].GetDouble();

		loaded = true;
	}
};
