#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class TimeKeeper {
  public:

    TimeKeeper(){
      if(TimeKeeper::initialized == false){
      TimeKeeper::currentTime = glfwGetTime();
      TimeKeeper::deltaT = TimeKeeper::lastTime-TimeKeeper::currentTime;
      TimeKeeper::paused = false;
      TimeKeeper::initialized = true;
      }
    }

    void updateTime(){
      TimeKeeper::lastTime = TimeKeeper::currentTime;
      TimeKeeper::currentTime = glfwGetTime();
        TimeKeeper::deltaT= TimeKeeper::currentTime-TimeKeeper::lastTime;
      if(TimeKeeper::deltaT>0.1){
          TimeKeeper::deltaT = 0.1;
      }
    }

    double getCurrentTime(){
      return TimeKeeper::currentTime;
    }

    double getLastTime(){
      return TimeKeeper::lastTime;
    }

    double getDeltaT(){
      if(paused){
        return 0.0;
      }else{
         return TimeKeeper::deltaT;
      }
    }

    double getDeltaTRaw(){
      return TimeKeeper::deltaT;
    }

    void pauseTime(){
      TimeKeeper::paused = true;
    }

    void playTime(){
      TimeKeeper::paused = false;
    }

    bool isPaused(){
      return TimeKeeper::paused;
    }

    void setDeltaT(double newDeltaT){
      TimeKeeper::deltaT = newDeltaT;
    }

private:
  static bool paused;
  static bool initialized;
  static double currentTime;
  static double lastTime;
  static double deltaT;
};

bool TimeKeeper::initialized = false;
bool   TimeKeeper::paused = false;
double TimeKeeper::currentTime = 0.0;
double TimeKeeper::lastTime = 0.0;
double TimeKeeper::deltaT = 0.0;