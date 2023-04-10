#pragma once

#include "Provider.h"
#include "Model.h"
#include "RenderingSystem.h"

class ModelProvider : public Provider
{
public:
	static void initialize(RenderingSystem &);

	static Model *carModel;
	static Model *carFlames;
	static Model *rock1;
	static Model *rock2;
	static Model *rock3;
	static Model *rayBeam;
	static Model *straightPath;
	static Model *oneDividedPath;
	static Model *twiceDividedPath;
	static Model *archPath;
	static Model *curvedPath;
	static Model *speedupBlock;
	static Model *slowdownBlock;
};
