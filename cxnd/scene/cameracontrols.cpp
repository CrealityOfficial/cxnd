#include "cameracontrols.h"
#include "cxnd/scene/camera.h"

namespace cxnd
{
	CameraControls::CameraControls()
		: m_rotateEnabled(true)
		, m_scaleEnabled(true)
		, m_translateEnabled(true)
		, m_width(1.0f)
		, m_height(1.0f)
		, m_camera(nullptr)
		, m_minFovy(2.0f)
		, m_maxFovy(50.0f)
	{

	}

	CameraControls::~CameraControls()
	{

	}

	void CameraControls::setCamera(Camera* camera)
	{
		m_camera = camera;
	}
    
    Camera* CameraControls::getCamera()
    {
        return m_camera;
    }

	void CameraControls::onResize(float width, float height)
	{
		m_width = width;
		m_height = height;

		if (m_camera)
		{
			m_camera->aspectRatio = m_width / m_height;
			m_camera->width = m_width;
			m_camera->height = m_height;
		}

		notifyScreenSize(m_width, m_height);
		notifyProjectionMatrix();
	}

	bool CameraControls::rotateEnabled()
	{
		return m_rotateEnabled && m_camera;
	}

	bool CameraControls::scaleEnabled()
	{
		return m_scaleEnabled && m_camera;
	}

	bool CameraControls::translateEnabled()
	{
		return m_translateEnabled && m_camera;
	}

	void CameraControls::setRotateEnabled(bool enabled)
	{
		m_rotateEnabled = enabled;
	}

	void CameraControls::setScaleEnabled(bool enabled)
	{
		m_scaleEnabled = enabled;
	}

	void CameraControls::setTranslateEnabled(bool enabled)
	{
		m_translateEnabled = enabled;
	}

	void CameraControls::notifyViewMatrix()
	{
		if (!m_camera)
			return;

		trimesh::fxform xform = m_camera->viewMatrix();

		notify([&xform](CameraTracer* tracer) {
			tracer->onViewMatrixChanged(xform);
			});
	}

	void CameraControls::notifyProjectionMatrix()
	{
		if (!m_camera)
			return;

		trimesh::fxform xform = m_camera->projectionMatrix();

		notify([&xform](CameraTracer* tracer) {
			tracer->onProjectionMatrixChanged(xform);
			});
	}

	void CameraControls::notifyScreenSize(float width, float height)
	{
		if (!m_camera)
			return;

		notify([&width, &height](CameraTracer* tracer) {
			tracer->onScreenSizeChanged(width, height);
			});
	}

	void CameraControls::setCameraScreenSize(float width, float height)
	{
		if (!m_camera)
			return;

		m_camera->aspectRatio = width / height;
	}

	void CameraControls::setCameraNearFar(float fNear, float fFar)
	{
		if (!m_camera)
			return;

		m_camera->fNear = fNear;
		m_camera->fFar = fFar;
	}

	void CameraControls::setCameraPose(const trimesh::vec3& position, const trimesh::vec3& viewCenter,
		const trimesh::vec3& upVector)
	{
		if (!m_camera)
			return;

		m_camera->position = position;
		m_camera->viewCenter = viewCenter;
		m_camera->upVector = upVector;

		notifyViewMatrix();
	}

	void CameraControls::scaleCamera(float factor)
	{
		if (!m_camera)
			return;

		float maxFovy = m_maxFovy;
		float minFovy = m_minFovy;
		float fovy = m_camera->fovy;
		fovy /= factor;
		if (fovy >= minFovy && fovy <= maxFovy)
			m_camera->fovy = fovy;

		notifyProjectionMatrix();
	}

	void CameraControls::fittingBox(const trimesh::box3& box, bool resetDir)
	{
		if (!m_camera)
			return;

		m_camera->fittingBox(box, resetDir);
		notifyViewMatrix();
		notifyProjectionMatrix();

		//trimesh::vec3 center = box.center();
		//trimesh::vec3 size = box.size();
		//
		//float fovy = m_camera->fovy * M_PIf / 180.0f;
		//
		//auto f = [](float z, float x, float fovy)->float {
		//	float r = sqrtf(x * x + z * z) / 2.0f;
		//	return r / sinf(fovy / 2.0f);
		//};
		//
		//float len1 = f(size.z, size.y, fovy);
		//float len2 = f(size.x, size.y, 2.0f * atanf(m_camera->aspectRatio * tanf(fovy / 2.0f)));
		//float len = len1 > len2 ? len1 : len2;
		//
		//LOGI("CameraControls fittingBox %f %f %f", m_camera->aspectRatio, len1, len2);
		//trimesh::vec3 up = trimesh::vec3(0.0f, 0.0f, 1.0f);
		//trimesh::vec3 dir = trimesh::vec3(0.0f, -1.0f, 0.0f);
		//if (!resetDir)
		//{
		//	up = m_camera->upVector;
		//	dir = - m_camera->direction();
		//}
		//trimesh::vec3 eye = center + dir * len;
		//
		//setCameraPose(eye, center, up);
		//updateNearFar(box);
		//notifyViewMatrix();
	}

	void CameraControls::updateNearFar(const trimesh::box3& box)
	{
		if (!m_camera)
			return;

		trimesh::vec3 cameraPosition = m_camera->position;
		trimesh::vec3 cameraCenter = m_camera->viewCenter;
		trimesh::vec3 cameraView = cameraCenter - cameraPosition;
		trimesh::normalize(cameraView);

		trimesh::vec3 center = box.center();
		float r = trimesh::len(box.size()) / 2.0f;
		float d = cameraView DOT(center - cameraPosition);
		float dmin = d - 1.2f * r;
		float dmax = d + 1.2f * r;

		float nearpos = dmin < 1.0f ? (2.0f * r > 1.0f ? 0.1f : dmin) : dmin;
		float farpos = dmax > 0.0f ? dmax : 3000.0f;

		setCameraNearFar(nearpos, farpos);
		notifyProjectionMatrix();
	}

	void CameraControls::translateCamera(const trimesh::vec3& delta)
	{
		trimesh::vec3 viewCenter = m_camera->viewCenter;
		trimesh::vec3 position = m_camera->position;

		trimesh::vec3 newPosition = position + delta;
		trimesh::vec3 newViewCenter = viewCenter + delta;
		trimesh::vec3 newUpVector = m_camera->upVector;

		setCameraPose(newPosition, newViewCenter, newUpVector);
	}

	void CameraControls::screenTranslateCamera(float x, float y)
	{
		trimesh::vec2 t;
		t.x = x / (float)m_width;
		t.y = y / (float)m_height;

		trimesh::vec3 cameraPosition = m_camera->position;
		trimesh::vec3 viewCenter = m_camera->viewCenter;
		trimesh::vec3 dir = viewCenter - cameraPosition;
		float D = trimesh::len(dir);
		trimesh::normalize(dir);
		trimesh::vec3 left = dir TRICROSS m_camera->upVector;

		D *= tan(M_PI_180f * m_camera->fovy / 2.0f) * 2.0f;
		float pixelY = D * t.y;
		float pixelX = D * m_camera->aspectRatio * t.x;

		trimesh::vec3 delta = pixelY * m_camera->upVector - pixelX * left;

		translateCamera(delta);
	}
}