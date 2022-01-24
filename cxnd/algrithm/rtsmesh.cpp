#include "rtsmesh.h"
#include "trimesh2/XForm.h"
#include "trimesh2/TriMesh_algo.h"

namespace cxnd
{
	void moveMesh2BoxCenter(trimesh::TriMesh* mesh, const trimesh::box3& box)
	{
		if (!mesh)
			return;

		mesh->need_bbox();
		trimesh::vec3 origin = trimesh::vec3();
		if (box.valid)
			origin = box.center();

		trimesh::xform xf = trimesh::xform::trans(origin - mesh->bbox.center());
		trimesh::apply_xform(mesh, xf);
	}

	void moveMeshes2BoxCenter(std::vector<trimesh::TriMesh*> meshes, const trimesh::box3& box)
	{
		for (trimesh::TriMesh* mesh : meshes)
			moveMesh2BoxCenter(mesh, box);
	}
}