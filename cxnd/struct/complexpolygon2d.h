#ifndef CXND_COMPLEXPOLYGON2D_1650510819037_H
#define CXND_COMPLEXPOLYGON2D_1650510819037_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/TriMesh.h"
#include <vector>

namespace cxnd
{
	class CXND_API ComplexPolygon2D
	{
	public:
		ComplexPolygon2D();
		ComplexPolygon2D(const ComplexPolygon2D& polygon);
		~ComplexPolygon2D();

		std::vector<std::vector<int>> polygons;
		std::vector<trimesh::dvec2> points;
	};

	CXND_API void polygonSave(const std::string& fileName, const ComplexPolygon2D& polygon);
	CXND_API void polygonLoad(const std::string& fileName, ComplexPolygon2D& polygon);

	typedef trimesh::TriMesh::Face TriangleFace;
	typedef std::vector<trimesh::TriMesh::Face> TriangleFaces2D;
}

#endif // CXND_COMPLEXPOLYGON2D_1650510819037_H