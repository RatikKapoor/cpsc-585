#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <PxPhysicsAPI.h>
#include "rapidjson/writer.h"

using namespace std;
using namespace rapidjson;

class LocationWriter
{
public:
	LocationWriter(string logFileName = "location_writer_output.json") {}

	static void write()
	{
		if (firstWriting)
		{
			LocationWriter::logFile.open(LocationWriter::logFileName);
			LocationWriter::firstWriting = false;
		}
		else
		{
			LocationWriter::logFile.open(LocationWriter::logFileName, std::ios_base::app);
		}

		StringBuffer buff;
		Writer<StringBuffer> w(buff);
		buff.Clear();
		w.StartArray();
		for (auto& state : states) {
			w.StartObject();
			w.String("brakes");
			w.Double(state.brakes[0]);
			w.String("steer");
			w.Double(state.steer);
			w.String("throttle");
			w.Double(state.throttle);
			w.EndObject();
		}
		w.EndArray();

		LocationWriter::logFile << buff.GetString() << endl;

		LocationWriter::logFile.close();
	}

	static void log(physx::vehicle2::PxVehicleCommandState s) {
		states.push_back(s);
	}

	static string logFileName;
	static bool firstWriting;

	static ofstream logFile;

	static std::vector<physx::vehicle2::PxVehicleCommandState> states;

private:
};
