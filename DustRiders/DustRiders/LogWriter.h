#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "PxPhysicsAPI.h"

using namespace std;
using namespace physx;


class LogWriter
{
public:
  LogWriter(string logFileName = "latest_output.log") {}

  static void log(string logMessage)
  {
    if (firstWriting)
    {
      LogWriter::logFile.open(LogWriter::logFileName);
      LogWriter::firstWriting = false;
    }
    else
    {
      LogWriter::logFile.open(LogWriter::logFileName, std::ios_base::app);
    }
    LogWriter::logFile << logMessage << endl;

    LogWriter::logFile.close();
  }

  static string to_string(PxVec3 vec){
    return std::string("["+ std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + "]");
}

  static string logFileName;
  static bool firstWriting;

  static ofstream logFile;

private:
};
