#ifndef CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H
#define CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/Box.h"
#include "trimesh2/quaternion.h"
#include "trimesh2/XForm.h"
#include "cxnd/struct/ray.h"

namespace cxnd
{
	CXND_API bool rayIntersectTriangle(const trimesh::vec3& orig, const trimesh::vec3& dir,
	     trimesh::vec3& v0, trimesh::vec3& v1, trimesh::vec3& v2, float* t, float* u, float* v);
    CXND_API bool rayIntersectTriangle(const trimesh::vec3& orig, const trimesh::vec3& dir,
		trimesh::vec3& v0, trimesh::vec3& v1, trimesh::vec3& v2, trimesh::vec3& baryPosition);

	CXND_API bool rayIntersectPlane(const trimesh::vec3& orig, const trimesh::vec3& dir, trimesh::vec3& vertex, trimesh::vec3& normal, float& t);

	CXND_API bool rayIntersectBoundingBox(const trimesh::dvec3 orig, const trimesh::dvec3 dir, const trimesh::dbox3& b);

	CXND_API bool PointinTriangle(trimesh::vec3 A, trimesh::vec3 B, trimesh::vec3 C, trimesh::vec3 P);

	CXND_API trimesh::box3 extendBox(const trimesh::box3& b, float r);

	CXND_API float angleOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2);
	CXND_API float radianOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2);

	CXND_API trimesh::quaternion quaternionFromVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2);
	
	CXND_API void partScale(const trimesh::vec3& center, const trimesh::box3& part,
		const trimesh::vec3& scale, trimesh::vec3& outScale, trimesh::vec3& outOffset);

	CXND_API trimesh::box3 extendBoxXY(const trimesh::box3& box, const trimesh::vec3& center);

	CXND_API trimesh::fxform applyBox(const trimesh::box3& tbox, const trimesh::box3& sbox, bool sameScale = true);

	CXND_API trimesh::vec3 point3DFromVector2D(const trimesh::vec2& point, const trimesh::vec2& center, const trimesh::vec2& size, bool skipz);

	CXND_API trimesh::vec3 point3DFromVector2D(const trimesh::vec2& point, const trimesh::vec2& center, float width, float height, bool skipz);

	CXND_API void boxFittingBox(const trimesh::box3& baseBounding, const trimesh::box3& initBox, trimesh::vec3& translate, trimesh::vec3& scale);

	CXND_API trimesh::box3 transformBox(const trimesh::fxform& matrix, const trimesh::box3& box);

	CXND_API bool lineCollidePlane(const trimesh::vec3& planeCenter, const trimesh::vec3& planeDir, const cxnd::Ray& ray, trimesh::vec3& collide);

	CXND_API trimesh::fxform layMatrixFromPositionNormal(const trimesh::vec3& position, const trimesh::vec3& normal, const trimesh::vec3& scale, const trimesh::vec3& originNormal);

	CXND_API trimesh::fxform layArrowMatrix(const trimesh::vec3& position, const trimesh::vec3& normal, const trimesh::vec3& scale);

	CXND_API trimesh::fxform layArrowMatrix2(const trimesh::vec3& position, const trimesh::vec3& normal, const trimesh::vec3& size, float ratios = 60.0f);

	CXND_API int adjustY(int y, const trimesh::vec2& size);
}
#endif // CREATIVE_KERNEL_ALGRITHM3D_1593580656409_H