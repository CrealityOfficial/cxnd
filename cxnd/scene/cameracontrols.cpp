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

	void CameraControls::onResize(float width, float height)
	{
		m_width = width;
		m_height = height;
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
	}
}