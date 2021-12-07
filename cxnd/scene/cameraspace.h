#ifndef CXND_CAMERASPACE_1638867081868_H
#define CXND_CAMERASPACE_1638867081868_H
#include "cxnd/scene/camera.h"
#include "cxnd/struct/ray.h"

namespace cxnd
{
	CXND_API float viewportRatio(int x, int w);
	CXND_API trimesh::vec2 viewportRatio(const trimesh::ivec2& p, const trimesh::ivec2& size);
	CXND_API trimesh::vec2 viewportRatio(const trimesh::vec2& p, const trimesh::vec2& size);

	CXND_API Ray screenRay(const Camera& camera, const trimesh::ivec2& point, const trimesh::ivec2& size);
	CXND_API Ray screenRay(const Camera& camera, const trimesh::vec2& point, const trimesh::vec2& size);
	CXND_API Ray screenRay(const Camera& camera, const trimesh::vec2& point);
	CXND_API Ray screenRayOrthographic(const Camera& camera, const trimesh::vec2& point);
}

#endif // CXND_CAMERASPACE_1638867081868_H