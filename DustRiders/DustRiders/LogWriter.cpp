#include "LogWriter.h"

bool LogWriter::firstWriting = true;
std::string LogWriter::logFileName = "latest_output.log";
std::ofstream LogWriter::logFile;
