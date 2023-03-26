#pragma once

#include "PxPhysicsAPI.h"

#include "PhysicsEntity.h"
#include "PhysicsProvider.h"

using namespace physx;

class Ground : public PhysicsEntity
{
public:
	Ground(std::string,
				 Model *,
				 ShaderProgram *,
				 glm::vec3,
				 PhysicsProvider *,
				 PxVec3,
				 unsigned int);
	virtual ~Ground() {}

protected:
	void initGround(PxVec3);
	PxShape *setupTriangleMesh();

	PxVec3 toPxVec3(Vertex &v)
	{
		return PxVec3(v.position.x, v.position.y, v.position.z);
	}

private:
	static std::map<Model*, PxTriangleMeshGeometry*> meshCache;
};
