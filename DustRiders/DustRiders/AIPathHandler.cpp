#include "AIPathHandler.h"

bool AIPathHandler::firstWriting = true;
std::string AIPathHandler::logFileName = "location_writer_output.json";
std::ofstream AIPathHandler::logFile;
std::vector<vec3> AIPathHandler::locations;