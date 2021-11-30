#ifndef CXND_DUMPLICATE_1638253317667_H
#define CXND_DUMPLICATE_1638253317667_H
#include "cxnd/interface.h"

namespace trimesh
{
	class TriMesh;
}

namespace ccglobal
{
	class Tracer;
}

namespace cxnd
{
	CXND_API void dumplicateMesh(trimesh::TriMesh* mesh, ccglobal::Tracer* tracer = nullptr);
}

#endif // CXND_DUMPLICATE_1638253317667_H