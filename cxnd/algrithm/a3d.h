#ifndef CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H
#define CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/Box.h"

namespace cxnd
{
	CXND_API bool rayIntersectTriangle(const trimesh::vec3& orig, const trimesh::vec3& dir,
	     trimesh::vec3& v0, trimesh::vec3& v1, trimesh::vec3& v2, float* t, float* u, float* v);
    CXND_API bool rayIntersectTriangle(const trimesh::vec3& orig, const trimesh::vec3& dir,
		trimesh::vec3& v0, trimesh::vec3& v1, trimesh::vec3& v2, trimesh::vec3& baryPosition);

	CXND_API bool rayIntersectPlane(const trimesh::vec3& orig, const trimesh::vec3& dir, trimesh::vec3& vertex, trimesh::vec3& normal, float& t);
	CXND_API bool PointinTriangle(trimesh::vec3 A, trimesh::vec3 B, trimesh::vec3 C, trimesh::vec3 P);

	CXND_API trimesh::box3 extendBox(const trimesh::box3& b, float r);
}
#endif // CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H