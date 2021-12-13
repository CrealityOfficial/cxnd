#ifndef CXND_CAMERA_1638527203040_H
#define CXND_CAMERA_1638527203040_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/XForm.h"
#include "cxnd/struct/ray.h"

namespace cxnd
{
	enum class CameraProjectionType
	{
		ePerspective,
		eOrth
	};

	class CXND_API Camera
	{
	public:
		Camera();
		~Camera();

		trimesh::fxform viewMatrix();
		trimesh::fxform posMatrix();
		trimesh::fxform projectionMatrix();
		trimesh::vec3 horizontal();
		trimesh::vec3 vertical();
		trimesh::vec3 direction();
		float verticalAngle();
		Ray screenRay(const trimesh::vec2& pixel);

		CameraProjectionType type;

		trimesh::vec3 viewCenter;
		trimesh::vec3 upVector;
		trimesh::vec3 position;

		float fNear;
		float fFar;
		float fovy;
		float aspectRatio;

		float top;
		float bottom;
		float left;
		float right;

		float width;
		float height;
	};
}

#endif // CXND_CAMERA_1638527203040_H