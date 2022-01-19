#ifndef CXND_RAYMESH_1642582336569_H
#define CXND_RAYMESH_1642582336569_H
#include "cxnd/interface.h"
#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"
#include "cxnd/struct/ray.h"

namespace cxnd
{
	CXND_API bool rayMeshCheck(trimesh::TriMesh* mesh, const trimesh::fxform& matrix, int primitiveID, const cxnd::Ray& ray,
		trimesh::vec3& position, trimesh::vec3& normal);
}

#endif // CXND_RAYMESH_1642582336569_H