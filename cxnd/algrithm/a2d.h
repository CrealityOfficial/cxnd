#ifndef CXND_A2D_1650515944541_H
#define CXND_A2D_1650515944541_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"
#include <vector>

namespace cxnd
{
	inline double crossValue(const trimesh::dvec2& v10, const trimesh::dvec2& v12)
	{
		return v10.x * v12.y - v10.y * v12.x;
	}

	CXND_API double dotValue(const trimesh::dvec2& v10, const trimesh::dvec2& v12);
	CXND_API double angle(const trimesh::dvec2& v10, const trimesh::dvec2& v12);
	CXND_API bool insideTriangle(const trimesh::dvec2& va, const trimesh::dvec2& vb, const trimesh::dvec2& vc,
		const trimesh::dvec2& vp);
	CXND_API bool insideTriangleEx(const trimesh::dvec2& va, const trimesh::dvec2& vb, const trimesh::dvec2& vc,
		const trimesh::dvec2& vp);   // not in edges

	CXND_API double polygonArea(std::vector<int>& polygon, std::vector<trimesh::dvec2>& points);
}

#endif // CXND_A2D_1650515944541_H