#ifndef CXND_CAMERA_1638527203040_H
#define CXND_CAMERA_1638527203040_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"
#include "trimesh2/XForm.h"
#include "trimesh2/Box.h"
#include "cxnd/struct/ray.h"

namespace cxnd
{
	enum class CameraProjectionType
	{
		ePerspective,
		eOrth
	};

    enum class CameraViewDirection
    {
        front = 0,
        back = 1,
        left = 2,
        right = 3,
        top = 4,
        bottom = 5
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
		void fittingBox(const trimesh::box3& box, bool resetDir = true, CameraViewDirection vd = CameraViewDirection::front);
		void fittingBoxDirection(const trimesh::box3& box, const trimesh::vec3& up, const trimesh::vec3& direction);
		void updateNearFar(const trimesh::box3& box);
		void setSize(int width, int height);
        float defaultFovy();
        
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
