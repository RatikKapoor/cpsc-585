#include "TimeKeeper.h"

bool TimeKeeper::initialized = false;
bool   TimeKeeper::paused = false;
double TimeKeeper::currentTime = 0.0;
double TimeKeeper::lastTime = 0.0;
double TimeKeeper::deltaT = 0.0;
double TimeKeeper::counter = 0.0;