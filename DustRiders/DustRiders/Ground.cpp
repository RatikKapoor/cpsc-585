#include "Ground.h"
#include "LogWriter.h"

Ground::Ground(std::string n,
	Model* m,
	ShaderProgram* sp,
	glm::vec3 s,
	PhysicsProvider* pp,
	PxVec3 pos = { 0.f, 0.f, 0.f },
	unsigned int mat = 0) : PhysicsEntity(n, m, sp, s, pp, pos, mat)
{
	initGround(pos);
}

void Ground::initGround(PxVec3 pos)
{
	auto shape = setupTriangleMesh();

	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

	PxTransform localTm(pos);
	PxRigidStatic* body = gPhysics->createRigidStatic(localTm);
	body->setGlobalPose(localTm);
	body->attachShape(*shape);
	body->setName((this->name).c_str());
	gScene->addActor(*body);
	physicsProvider->AddEntity(body, this->transform);

	shape->release();
}

PxShape* Ground::setupTriangleMesh()
{
	LogWriter::log("START: creating triangle mesh for ground.");

	physx::PxTriangleMeshDesc groundDesc;
	groundDesc.setToDefault();

	// Convert mesh verts to PxVec3
	// Convert mesh indices to PxU32
	std::vector<PxVec3> meshVerts;
	std::vector<PxU32> meshInds;
	for (int i = 0; i < model->meshes.size(); i++)
	{
		for (int j = 0; j < model->meshes[i].verts.size(); j++)
		{
			meshVerts.push_back(toPxVec3(model->meshes[i].verts[j]));
		}
		for (int j = 0; j < model->meshes[i].indices.size(); j++)
		{
			meshInds.push_back(PxU32(model->meshes[i].indices[j]));
		}
	}

	// Need the values to be in a contiguous array, which vectors were not providing.
	// The unique ptr here makes sure that as soon as the method is out of scope (and the mesh is cooked) that it is deleted.
	std::unique_ptr<PxVec3> vertArr = std::unique_ptr<PxVec3>(new PxVec3[meshVerts.size()]);
	std::copy(meshVerts.begin(), meshVerts.end(), vertArr.get());
	std::unique_ptr<PxU32> indArr = std::unique_ptr<PxU32>(new PxU32[meshInds.size()]);
	std::copy(meshInds.begin(), meshInds.end(), indArr.get());

	LogWriter::log("NumVerts: " + std::to_string(meshVerts.size()) + ", NumInds: " + std::to_string(meshInds.size()));
	LogWriter::log("NumFaces: " + std::to_string(meshInds.size() / 3));

	groundDesc.points.count = meshVerts.size();
	groundDesc.points.stride = sizeof(PxVec3);
	groundDesc.points.data = vertArr.get();

	groundDesc.triangles.count = meshInds.size() / 3;
	groundDesc.triangles.stride = sizeof(PxU32) * 3;
	groundDesc.triangles.data = indArr.get();

	PxDefaultMemoryOutputStream writeBuf;
	PxTriangleMeshCookingResult::Enum result;

	this->gCooking->cookTriangleMesh(groundDesc, writeBuf, &result);

	PxDefaultMemoryInputData readBuf(writeBuf.getData(), writeBuf.getSize());
	PxTriangleMesh* groundMesh = gPhysics->createTriangleMesh(readBuf);

	PxTriangleMeshGeometry groundGeom = PxTriangleMeshGeometry(groundMesh, PxMeshScale(1.0));

	return gPhysics->createShape(groundGeom, *gMaterial, true);
}