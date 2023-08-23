#ifndef CXND_TRIMESHSERIAL_1691656350835_H
#define CXND_TRIMESHSERIAL_1691656350835_H
#include "cxnd/interface.h"
#include "cxnd/serial/serial.h"
#include "trimesh2/TriMesh.h"

namespace cxnd
{
	typedef std::vector<trimesh::vec3> CXNDPolygon;
	typedef std::vector<CXNDPolygon> CXNDPolygons;

	CXND_API trimesh::TriMesh* loadTrimesh(boost::nowide::fstream& in);
	CXND_API void saveTrimesh(boost::nowide::fstream& out, trimesh::TriMesh* mesh);

	CXND_API bool loadTrimesh(boost::nowide::fstream& in, trimesh::TriMesh& mesh);
	CXND_API void saveTrimesh(boost::nowide::fstream& out, const trimesh::TriMesh& mesh);

	class CXND_API CXNDPolygonsWrapper : public cxnd::Serializeable
	{
	public:
		CXNDPolygonsWrapper();
		~CXNDPolygonsWrapper();

		CXNDPolygons polys;

	protected:
		int version() override;
		bool save(boost::nowide::fstream& out, ccglobal::Tracer* tracer) override;
		bool load(boost::nowide::fstream& in, int ver, ccglobal::Tracer* tracer) override;
	};

	CXND_API void loadPolys(boost::nowide::fstream& in, CXNDPolygons& polys);
	CXND_API void savePolys(boost::nowide::fstream& out, const CXNDPolygons& polys);
}

#endif // CXND_TRIMESHSERIAL_1691656350835_H