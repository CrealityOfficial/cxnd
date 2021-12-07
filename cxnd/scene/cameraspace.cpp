#include "cameraspace.h"
#include <math.h>

namespace cxnd
{
	float viewportRatio(int x, int w)
	{
		return (float)x / (float)w - 0.5f;
	}

	trimesh::vec2 viewportRatio(const trimesh::ivec2& p, const trimesh::ivec2& size)
	{
		trimesh::vec2 pf;
		pf.x = ((float)p.x / (float)size.x - 0.5f);
		pf.y = ((float)(size.y - p.y) / (float)size.y - 0.5f);
		return pf;
	}

	trimesh::vec2 viewportRatio(const trimesh::vec2& p, const trimesh::vec2& size)
	{
		trimesh::vec2 pf;
		pf.x = (p.x / size.x - 0.5f);
		pf.y = ((size.y - p.y) / size.y - 0.5f);
		return pf;
	}

	Ray screenRay(const Camera& camera, const trimesh::ivec2& point, const trimesh::ivec2& size)
	{
		trimesh::vec2 p(0.0, 0.0);
		p = viewportRatio(point, size);

		if (camera.type == CameraProjectionType::ePerspective)
			return screenRay(camera, p);
		else if (camera.type == CameraProjectionType::eOrth)
			return screenRayOrthographic(camera, p);
		else
			return screenRay(camera, p);
	}

	Ray screenRay(const Camera& camera, const trimesh::vec2& point, const trimesh::vec2& size)
	{
		trimesh::vec2 p(0.0, 0.0);
		p = viewportRatio(point, size);

		if (camera.type == CameraProjectionType::ePerspective)
			return screenRay(camera, p);
		else if (camera.type == CameraProjectionType::eOrth)
			return screenRayOrthographic(camera, p);
		else
			return screenRay(camera, p);
	}

	Ray screenRay(const Camera& camera, const trimesh::vec2& point)
	{
		trimesh::vec3 dir = trimesh::normalized(camera.viewCenter - camera.position);
		trimesh::vec3 nearCenter = camera.position + dir * camera.fNear;
		trimesh::vec3 left = dir TRICROSS camera.upVector;
		trimesh::normalize(left);
		float near_height = 2.0f * camera.fNear * tanf(camera.fovy * M_PIf / 180.0f / 2.0f);
		float near_width = near_height * camera.aspectRatio;
		trimesh::vec3 nearPoint = nearCenter + camera.upVector * point.y * near_height + left * point.x * near_width;

		Ray ray;
		ray.start = camera.position;
		ray.dir = nearPoint - ray.start;
		trimesh::normalize(ray.dir);
		return ray;
	}

	Ray screenRayOrthographic(const Camera& camera, const trimesh::vec2& point)
	{
		trimesh::vec3 dir = trimesh::normalized(camera.viewCenter - camera.position);
		trimesh::vec3 nearCenter = camera.position + dir * camera.fNear;

		float near_width = fabs(camera.right - camera.left) * point.x;
		float near_height = fabs(camera.top - camera.bottom) * point.y;
		trimesh::vec3 right = dir TRICROSS camera.upVector;
		trimesh::normalize(right);
		trimesh::vec3 nearPoint = nearCenter + camera.upVector * near_height + right * near_width;

		Ray ray;
		ray.dir = dir;// direction vector
		ray.start = nearPoint - dir * camera.fNear;//start vector
		return ray;
	}
}