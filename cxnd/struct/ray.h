#ifndef CXND_RAY_1638866681924_H
#define CXND_RAY_1638866681924_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"

namespace cxnd
{
	class CXND_API Ray
	{
	public:
		Ray();
		Ray(const trimesh::vec3& vstart);
		Ray(const trimesh::vec3& vstart, const trimesh::vec3& ndir);

		~Ray();

		bool collidePlane(const trimesh::vec3& planeCenter, const trimesh::vec3& planeDir, trimesh::vec3& collide);

		trimesh::vec3 start;
		trimesh::vec3 dir;
	};
}

#endif // CXND_RAY_1638866681924_H