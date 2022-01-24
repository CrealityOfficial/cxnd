#ifndef RC_MULTISLICE_RTSMESH_1643018358328_H
#define RC_MULTISLICE_RTSMESH_1643018358328_H
#include "cxnd/interface.h"
#include "trimesh2/TriMesh.h"
#include "trimesh2/Box.h"

namespace cxnd
{
	//if box is invalid, move meshes to origin
	CXND_API void moveMesh2BoxCenter(trimesh::TriMesh* mesh, const trimesh::box3& box = trimesh::box3());
	CXND_API void moveMeshes2BoxCenter(std::vector<trimesh::TriMesh*> meshes, const trimesh::box3& box = trimesh::box3());

}

#endif // RC_MULTISLICE_RTSMESH_1643018358328_H