#ifndef CXND_TRIMESHSERIAL_1691656350835_H
#define CXND_TRIMESHSERIAL_1691656350835_H
#include "cxnd/interface.h"
#include "trimesh2/TriMesh.h"

namespace cxnd
{
	CXND_API trimesh::TriMesh* loadTrimesh(std::fstream& in);
	CXND_API void saveTrimesh(std::fstream& out, trimesh::TriMesh* mesh);
}

#endif // CXND_TRIMESHSERIAL_1691656350835_H