#include "ModelProvider.h"

Model* ModelProvider::carModel;
Model* ModelProvider::rock1;
Model* ModelProvider::rock2;
Model* ModelProvider::rock3;
Model* ModelProvider::rayBeam;
Model* ModelProvider::straightPath;
Model* ModelProvider::dividedPath;
Model* ModelProvider::speedupBlock;
Model* ModelProvider::slowdownBlock;


void ModelProvider::initialize(RenderingSystem& renderer) {
	carModel = renderer.loadModelFromFile("TestCar", "./assets/models/car-model.obj");
	rock1 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock1.obj");
	rock2 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock2.obj");
	rock3 = renderer.loadModelFromFile("TestRock", "./assets/models/test-rock3.obj");
	rayBeam = renderer.loadModelFromFile("RayBeam", "./assets/models/raygun-beam.obj");
	straightPath = renderer.loadModelFromFile("GroundPlane", "./assets/models/DesertPathStraight.obj");
	dividedPath = renderer.loadModelFromFile("GroundPlane", "./assets/models/DesertPathDivide.obj");
	speedupBlock = renderer.loadModelFromFile("SpeedupBlock", "./assets/models/speedup-block.obj");
	slowdownBlock = renderer.loadModelFromFile("SlowdownBlock", "./assets/models/slowdown-block.obj");
}
