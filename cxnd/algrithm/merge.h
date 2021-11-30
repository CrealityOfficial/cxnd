#ifndef CXND_MERGE_1638255427266_H
#define CXND_MERGE_1638255427266_H
#include "cxnd/interface.h"
#include <vector>

namespace trimesh
{
	class TriMesh;
}

namespace cxnd
{
	CXND_API trimesh::TriMesh* mergeMeshes(std::vector<trimesh::TriMesh*>& inputs, bool releaseInput = false);
}

#endif // CXND_MERGE_1638255427266_H