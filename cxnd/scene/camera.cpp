#include "camera.h"
#include "cameraspace.h"

namespace cxnd
{
	Camera::Camera()
		: fovy(30.0f)
		, fNear(1.0f)
		, fFar(100.0f)
		, aspectRatio(1.0f)
		, type(CameraProjectionType::ePerspective)
		, top(1.0f)
		, bottom(-1.0f)
		, left(-1.0f)
		, right(1.0f)
		, width(10.0f)
		, height(10.0f)   //fit with aspectRatio
	{

	}

	Camera::~Camera()
	{

	}

	trimesh::fxform Camera::viewMatrix()
	{
		return trimesh::fxform::lookat(position, viewCenter, upVector);
	}

	trimesh::fxform Camera::posMatrix()
	{
		trimesh::fxform view = viewMatrix();
		return trimesh::inv(view);
	}

	trimesh::fxform Camera::projectionMatrix()
	{
		trimesh::fxform xf = trimesh::fxform::identity();
		if (type == CameraProjectionType::ePerspective) {
			xf = trimesh::fxform::perspective(fovy, aspectRatio, fNear, fFar);
		}
		else {
			xf = trimesh::fxform::ortho(left, right, bottom, top, fNear, fFar);
		}
		return xf;
	}

	trimesh::vec3 Camera::horizontal()
	{
		trimesh::vec3 dir = viewCenter - position;
		trimesh::normalize(dir);
		trimesh::vec3 up = upVector;

		trimesh::vec3 h = dir TRICROSS up;
		h.z = 0.0f;
		if (trimesh::len(h) > 0.0f)
		{
			trimesh::normalize(h);
		}
		if (trimesh::len(h) == 0.0f)
			return trimesh::vec3(1.0f, 0.0f, 0.0f);

		return h;
	}

	trimesh::vec3 Camera::vertical()
	{
		trimesh::vec3 v = trimesh::vec3(0.0f, 1.0f, 0.0f);
		trimesh::vec3 dir = viewCenter - position;
		float y = v DOT dir;
		float z = sqrtf(trimesh::len2(dir) - y * y);
		trimesh::vec3 d(0.0f, y, z);
		if (trimesh::len2(d) > 0.0f)
			v = trimesh::normalized(d);
		return v;
	}

	trimesh::vec3 Camera::direction()
	{
		trimesh::vec3 dir = viewCenter - position;
		trimesh::normalize(dir);
		return dir;
	}

	float Camera::verticalAngle()
	{
		trimesh::vec3 v = trimesh::vec3(0.0f, 0.0f, 1.0f);
		trimesh::vec3 dir = viewCenter - position;
		trimesh::normalize(dir);
		float angle = acosf(v DOT dir);
		return angle;
	}

	Ray Camera::screenRay(const trimesh::vec2& pixel)
	{
		return cxnd::screenRay(*this, pixel, trimesh::vec2(width, height));
	}
}