#include "rtsmesh.h"
#include "trimesh2/XForm.h"
#include "trimesh2/TriMesh_algo.h"

namespace cxnd
{
	trimesh::vec3 moveMesh2BoxCenter(trimesh::TriMesh* mesh, const trimesh::box3& box)
	{
		trimesh::vec3 offset;
		if (!mesh)
			return offset;

		mesh->need_bbox();
		trimesh::vec3 origin = trimesh::vec3();
		if (box.valid)
			origin = box.center();

		offset = origin - mesh->bbox.center();
		trimesh::xform xf = trimesh::xform::trans(offset);
		trimesh::apply_xform(mesh, xf);
		return offset;
	}

	void moveMeshes2BoxCenter(std::vector<trimesh::TriMesh*> meshes, const trimesh::box3& box)
	{
		for (trimesh::TriMesh* mesh : meshes)
			moveMesh2BoxCenter(mesh, box);
	}
}