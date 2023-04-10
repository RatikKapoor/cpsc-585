#include "ModelProvider.h"

Model *ModelProvider::carModel;
Model *ModelProvider::carFlames;
Model *ModelProvider::rock1;
Model *ModelProvider::rock2;
Model *ModelProvider::rock3;
Model *ModelProvider::rayBeam;
Model *ModelProvider::straightPath;
Model *ModelProvider::oneDividedPath;
Model *ModelProvider::twiceDividedPath;
Model *ModelProvider::archPath;
Model *ModelProvider::curvedPath;
Model *ModelProvider::speedupBlock;
Model *ModelProvider::slowdownBlock;

void ModelProvider::initialize(RenderingSystem &renderer)
{
	carModel = renderer.loadModelFromFile("TestCar", "./assets/models/car-model.obj");
	carFlames = renderer.loadModelFromFile("CarFlames", "./assets/models/car-flames.obj");
	rock1 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock1.obj");
	rock2 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock2.obj");
	rock3 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock3.obj");
	rayBeam = renderer.loadModelFromFile("RayBeam", "./assets/models/raygun-beam.obj");
	straightPath = renderer.loadModelFromFile("StraightPath", "./assets/models/chunk-straight.obj");
	oneDividedPath = renderer.loadModelFromFile("OneDividedPath", "./assets/models/chunk-1divide.obj");
	twiceDividedPath = renderer.loadModelFromFile("TwiceDividedPath", "./assets/models/chunk-2divides.obj");
	archPath = renderer.loadModelFromFile("SrchPath", "./assets/models/chunk-arch.obj");
	curvedPath = renderer.loadModelFromFile("CurvedPath", "./assets/models/chunk-curved.obj");
	speedupBlock = renderer.loadModelFromFile("SpeedupBlock", "./assets/models/speedup-block.obj");
	slowdownBlock = renderer.loadModelFromFile("SlowdownBlock", "./assets/models/slowdown-block.obj");
}
