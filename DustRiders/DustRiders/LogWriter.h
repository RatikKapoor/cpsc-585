#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

  static string logFileName;
  static bool firstWriting;

  static ofstream logFile;

private:
};
