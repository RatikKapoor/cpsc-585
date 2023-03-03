#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class LogWriter
{
public:
  LogWriter(string logFileName = "latest_output.log") : logFileName(logFileName), firstWriting(true) {}

  void log(string logMessage)
  {
    if (firstWriting)
    {
      logFile.open(logFileName);
    }
    else
    {
      logFile.open(logFileName, std::ios_base::app);
    }
    logFile << logMessage << endl;

    logFile.close();
  }

private:
  string logFileName;
  ofstream logFile;
  bool firstWriting;
};