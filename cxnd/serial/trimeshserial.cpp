#include "trimeshserial.h"
#include "cxnd/serial/serial.h"

namespace cxnd
{
	trimesh::TriMesh* loadTrimesh(std::fstream& in)
	{
		trimesh::TriMesh* mesh = nullptr;
		int have = 0;
		cxndLoadT<int>(in, have);
		if (have == 1)
		{
			mesh = new trimesh::TriMesh();
			cxndLoadVectorT(in, mesh->vertices);
			cxndLoadVectorT(in, mesh->faces);
			cxndLoadVectorT(in, mesh->faceUVs);
			cxndLoadVectorT(in, mesh->colors);

			cxndLoadT(in, mesh->bbox.min);
			cxndLoadT(in, mesh->bbox.max);
			mesh->bbox.valid = true;
		}

		return mesh;
	}

	void saveTrimesh(std::fstream& out, trimesh::TriMesh* mesh)
	{
		int have = mesh ? 1 : 0;
		cxndSaveT<int>(out, have);
		if (have)
		{
			cxndSaveVectorT(out, mesh->vertices);
			cxndSaveVectorT(out, mesh->faces);
			cxndSaveVectorT(out, mesh->faceUVs);
			cxndSaveVectorT(out, mesh->colors);

			cxndSaveT(out, mesh->bbox.min);
			cxndSaveT(out, mesh->bbox.max);
		}
	}
}