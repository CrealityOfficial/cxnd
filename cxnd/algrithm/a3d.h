#ifndef CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H
#define CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/Box.h"
#include "trimesh2/quaternion.h"

namespace cxnd
{
	CXND_API bool rayIntersectTriangle(const trimesh::vec3& orig, const trimesh::vec3& dir,
	     trimesh::vec3& v0, trimesh::vec3& v1, trimesh::vec3& v2, float* t, float* u, float* v);
    CXND_API bool rayIntersectTriangle(const trimesh::vec3& orig, const trimesh::vec3& dir,
		trimesh::vec3& v0, trimesh::vec3& v1, trimesh::vec3& v2, trimesh::vec3& baryPosition);

	CXND_API bool rayIntersectPlane(const trimesh::vec3& orig, const trimesh::vec3& dir, trimesh::vec3& vertex, trimesh::vec3& normal, float& t);

	CXND_API bool rayIntersectBoundingBox(const trimesh::dvec3 orig, const trimesh::dvec3 dir, const trimesh::dbox3& b);

	CXND_API bool PointinTriangle(trimesh::vec3 A, trimesh::vec3 B, trimesh::vec3 C, trimesh::vec3 P);

	CXND_API trimesh::box3 extendBox(const trimesh::box3& b, float r);

	CXND_API float angleOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2);
	CXND_API float radianOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2);

	CXND_API trimesh::quaternion quaternionFromVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2);

}
#endif // CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H