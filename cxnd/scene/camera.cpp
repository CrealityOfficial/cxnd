#include "camera.h"
#include "cameraspace.h"

const float DEFAULT_FOVY = 30.0;

namespace cxnd
{
	trimesh::fxform CameraMeta::viewMatrix()
	{
		return trimesh::fxform::lookat(position, viewCenter, upVector);
	}

	trimesh::fxform CameraMeta::posMatrix()
	{
		trimesh::fxform view = viewMatrix();
		return trimesh::inv(view);
	}

	trimesh::fxform CameraMeta::projectionMatrix()
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

	void createCameraPoints(CameraMeta* meta, std::vector<trimesh::vec3>& positions)
	{
		positions.resize(9);
		positions.at(0) = trimesh::vec3(0.0f, 0.0f, 0.0f);
		float nZ = -meta->fNear;
		float fZ = -meta->fFar;
		float nY = meta->fNear * tanf(meta->fovy * M_PIf / 360.0f);
		float fY = meta->fFar * tanf(meta->fovy * M_PIf / 360.0f);
		float nX = meta->aspectRatio * nY;
		float fX = meta->aspectRatio * fY;

		positions.at(1) = trimesh::vec3(nX, nY, nZ);
		positions.at(2) = trimesh::vec3(-nX, nY, nZ);
		positions.at(3) = trimesh::vec3(-nX, -nY, nZ);
		positions.at(4) = trimesh::vec3(nX, -nY, nZ);
		positions.at(5) = trimesh::vec3(fX, fY, fZ);
		positions.at(6) = trimesh::vec3(-fX, fY, fZ);
		positions.at(7) = trimesh::vec3(-fX, -fY, fZ);
		positions.at(8) = trimesh::vec3(fX, -fY, fZ);
	}

	Camera::Camera()
		: fovy(DEFAULT_FOVY)
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
		upVector = trimesh::vec3(1.0f, 0.0f, 0.0f);
	}

	Camera::~Camera()
	{

	}

	CameraMeta Camera::traitMeta()
	{
		CameraMeta meta;

		meta.type = type;
		meta.viewCenter = viewCenter;
		meta.upVector = upVector;
		meta.position = position;

		meta.fNear = fNear;
		meta.fFar = fFar;
		meta.fovy = fovy;
		meta.aspectRatio = aspectRatio;

		meta.top = top;
		meta.bottom = bottom;
		meta.left = left;
		meta.right = right;

		return meta;
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

	void Camera::fittingBoxDirection(const trimesh::box3& box, const trimesh::vec3& up, const trimesh::vec3& direction)
	{
		trimesh::vec3 center = box.center();
		trimesh::vec3 size = box.size();

		float _fovy = this->fovy * M_PIf / 180.0f;

		auto f = [](trimesh::vec3 size, float fovy)->float {
			float r = trimesh::len(size) / 2.0f;
			return r / sinf(fovy / 2.0f);
		};

		float len1 = f(size, _fovy);
		float len2 = f(size, 2.0f * atanf(this->aspectRatio * tanf(_fovy / 2.0f)));
		float len = len1 > len2 ? len1 : len2;

		trimesh::vec3 dir = direction;

		trimesh::vec3 eye = center - dir * len;

		this->position = eye;
		this->viewCenter = center;
		this->upVector = up;

		updateNearFar(box);
	}

	void Camera::fittingBox(const trimesh::box3& box, bool resetDir, CameraViewDirection vd)
	{
		trimesh::vec3 center = box.center();
		trimesh::vec3 size = box.size();

		float _fovy = this->fovy * M_PIf / 180.0f;

		auto f = [](trimesh::vec3 size, float fovy)->float {
			float r = trimesh::len(size) / 2.0f;
			return r / sinf(fovy / 2.0f);
		};

		float len1 = f(size, _fovy);
		float len2 = f(size, 2.0f * atanf(this->aspectRatio * tanf(_fovy / 2.0f)));
		float len = len1 > len2 ? len1 : len2;

        trimesh::vec3 up = trimesh::vec3(0.0f, 0.0f, 1.0f);
        trimesh::vec3 dir = trimesh::vec3(0.0f, -1.0f, 0.0f);
        switch (vd) {
            case CameraViewDirection::front:
				up = trimesh::vec3(0.0f, 0.0f, 1.0f);
				dir = trimesh::vec3(0.0f, -1.0f, 0.0f);
                break;
            
            case CameraViewDirection::top:
                up = trimesh::vec3(0.0f, 1.0f, 0.0f);
                dir = trimesh::vec3(0.0f, 0.0f, 1.0f);
                break;
            
			case CameraViewDirection::back:
				up = trimesh::vec3(0.0f, 0.0f, 1.0f);
				dir = trimesh::vec3(0.0f, 1.0f, 0.0f);
            default:
                break;
        }
		
		if (!resetDir)
		{
			up = this->upVector;
			dir = -this->direction();
		}
		trimesh::vec3 eye = center + dir * len;

		this->position = eye;
		this->viewCenter = center;
		this->upVector = up;

		updateNearFar(box);
	}

	void Camera::updateNearFar(const trimesh::box3& box)
	{
		trimesh::vec3 cameraPosition = this->position;
		trimesh::vec3 cameraCenter = this->viewCenter;
		trimesh::vec3 cameraView = cameraCenter - cameraPosition;
		trimesh::normalize(cameraView);

		trimesh::vec3 center = box.center();
		float r = trimesh::len(box.size()) / 2.0f;
		float d = cameraView DOT(center - cameraPosition);
		float dmin = d - 1.2f * r;
		float dmax = d + 1.2f * r;

		float nearpos = dmin < 1.0f ? (2.0f * r > 1.0f ? 0.1f : dmin) : dmin;
		float farpos = dmax > 0.0f ? dmax : 3000.0f;

		this->fNear = nearpos;
		this->fFar = farpos;
	}

	void Camera::setSize(int w, int h)
	{
		this->width = (float)w;
		this->height = (float)h;
		this->aspectRatio = this->width / this->height;
	}
    
    float Camera::defaultFovy()
    {
        return DEFAULT_FOVY;
    }
}
