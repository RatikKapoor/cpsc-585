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
#include "ChunkHandler.h"

#ifdef _MSC_VER
#undef GetObject
#endif
#include <map>
#include <set>


using namespace std;
using namespace rapidjson;
using vec3 = glm::vec3;
struct vec3compare {
	bool operator() (const vec3& lhs, const vec3& rhs) const {
		return lhs.z < rhs.z;
	}
};
using chunkMapLocationSave = std::map<std::string, std::set<vec3, vec3compare>>;
using chunkMapLocation = std::map<std::string, std::vector<vec3>>;

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

		w.StartObject();
		for (auto& pair : locations)
		{
			w.Key(pair.first.c_str());
			{
				w.StartArray();
				for (auto& location : pair.second)
				{
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
			}
		}
		w.EndObject();

		AIPathHandler::logFile << buff.GetString() << endl;

		AIPathHandler::logFile.close();
	}

	static void log(vec3 s) {
		auto chunkType = ChunkHandler::getChunkForZ(s.z);
		locations[chunkType].insert({ s.x, s.y, std::fmod(s.z + HALF_CHUNK_SIZE, CHUNK_SIZE) });
	}

	static chunkMapLocation deserialize(std::string filePath) {
		std::ifstream f(filePath);
		std::stringstream buffer;
		buffer << f.rdbuf();

		chunkMapLocation result;

		Document doc;
		doc.Parse(buffer.str().c_str());

		if (doc.HasParseError()) {
			LogWriter::log("Error parsing JSON");
			return result;
		}

		const std::vector<std::string> modelNames = { CHUNK_STRAIGHT, CHUNK_ONE_DIVIDED, CHUNK_TWICE_DIVIDED, CHUNK_ARCH, CHUNK_CURVED };
		for (auto& name : modelNames) {
			if (!doc.HasMember(name.c_str()))
				continue;
			auto locList = doc.GetObject()[name.c_str()].GetArray();
			for (auto itr = locList.Begin(); itr != locList.End(); ++itr) {
				vec3 v;
				v.x = itr->GetObject()["x"].GetDouble();
				v.y = itr->GetObject()["y"].GetDouble();
				v.z = itr->GetObject()["z"].GetDouble();

				result[name].push_back(v);
			}
		}

		return result;
	}

	static void clear() {
		locations.clear();
	}

	static string logFileName;
	static bool firstWriting;

	static ofstream logFile;

	static chunkMapLocationSave locations;

private:
};
