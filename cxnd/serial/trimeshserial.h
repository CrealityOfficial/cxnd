#ifndef CXND_TRIMESHSERIAL_1691656350835_H
#define CXND_TRIMESHSERIAL_1691656350835_H
#include "cxnd/interface.h"
#include "cxnd/serial/serial.h"
#include "trimesh2/TriMesh.h"

namespace cxnd
{
	typedef std::vector<trimesh::vec3> CXNDPolygon;
	typedef std::vector<CXNDPolygon> CXNDPolygons;

	CXND_API trimesh::TriMesh* loadTrimesh(std::fstream& in);
	CXND_API void saveTrimesh(std::fstream& out, trimesh::TriMesh* mesh);

	CXND_API bool loadTrimesh(std::fstream& in, trimesh::TriMesh& mesh);
	CXND_API void saveTrimesh(std::fstream& out, const trimesh::TriMesh& mesh);

	class CXND_API CXNDPolygonsWrapper : public cxnd::Serializeable
	{
	public:
		CXNDPolygonsWrapper();
		~CXNDPolygonsWrapper();

		CXNDPolygons polys;

	protected:
		int version() override;
		bool save(std::fstream& out, ccglobal::Tracer* tracer) override;
		bool load(std::fstream& in, int ver, ccglobal::Tracer* tracer) override;
	};

	CXND_API void loadPolys(std::fstream& in, CXNDPolygons& polys);
	CXND_API void savePolys(std::fstream& out, const CXNDPolygons& polys);
}

#endif // CXND_TRIMESHSERIAL_1691656350835_H