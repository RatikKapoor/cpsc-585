#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "LogWriter.h"
#include "glm/glm.hpp"

#ifdef _MSC_VER
#undef GetObject
#endif

using namespace std;
using namespace rapidjson;
using vec3 = glm::vec3;

class AIPathHandler
{
public:
	AIPathHandler(string logFileName = "location_writer_output.json") {}

	static void write()
	{
		if (firstWriting)
		{
			AIPathHandler::logFile.open(AIPathHandler::logFileName);
			AIPathHandler::firstWriting = false;
		}
		else
		{
			AIPathHandler::logFile.open(AIPathHandler::logFileName, std::ios_base::app);
		}

		StringBuffer buff;
		Writer<StringBuffer> w(buff);
		buff.Clear();
		w.StartArray();
		for (auto& location : locations) {
			w.StartObject();
			w.String("x");
			w.Double(location.x);
			w.String("y");
			w.Double(location.y);
			w.String("z");
			w.Double(location.z);
			w.EndObject();
		}
		w.EndArray();

		AIPathHandler::logFile << buff.GetString() << endl;

		AIPathHandler::logFile.close();
	}

	static void log(vec3 s) {
		locations.push_back(s);
	}

	static std::vector<vec3> deserialize(std::string filePath) {
		std::ifstream f(filePath);
		std::stringstream buffer;
		buffer << f.rdbuf();

		std::vector<vec3> result;

		Document doc;
		doc.Parse(buffer.str().c_str());

		if (doc.HasParseError()) {
			LogWriter::log("Error parsing JSON");
			return result;
		}

		for (auto itr = doc.Begin(); itr != doc.End(); ++itr) {
			vec3 v;
			v.x = itr->GetObject()["x"].GetDouble();
			v.y = itr->GetObject()["y"].GetDouble();
			v.z = itr->GetObject()["z"].GetDouble();

			result.push_back(v);
		}

		return result;
	}

	static void clear() {
		locations.clear();
	}

	static string logFileName;
	static bool firstWriting;

	static ofstream logFile;

	static std::vector<vec3> locations;

private:
};
