#pragma once

#include <vector>
#include "Obstacle.h"

class ObstacleHandler {
public:
	static void renderObstacles();

private:
	static std::vector<Obstacle> obstacleList;
};
