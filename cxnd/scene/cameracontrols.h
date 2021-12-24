#ifndef CXND_CAMERACONTROLS_1638870835676_H
#define CXND_CAMERACONTROLS_1638870835676_H
#include "cxnd/interface.h"
#include "ccglobal/container.h"
#include "trimesh2/XForm.h"
#include "trimesh2/Box.h"
#include "cxnd/scene/eventhandlers.h"

namespace cxnd
{
	class CameraTracer
	{
	public:
		virtual ~CameraTracer() {}

		virtual void onViewMatrixChanged(const trimesh::fxform& xform) = 0;
		virtual void onProjectionMatrixChanged(const trimesh::fxform& xform) = 0;
		virtual void onScreenSizeChanged(float width, float height) = 0;
	};

	class Camera;
	class CXND_API CameraControls : public LContainer<CameraTracer>
		, public ResizeEventHandler
		, public WheelEventHandler
		, public LeftMouseEventHandler
		, public MidMouseEventHandler
		, public RightMouseEventHandler
		, public HoverEventHandler
		, public TouchEventHandler
	{
	public:
		CameraControls();
		virtual ~CameraControls();

		void setCamera(Camera* camera);
		bool rotateEnabled();
		bool scaleEnabled();
		bool translateEnabled();
		void setRotateEnabled(bool enabled);
		void setScaleEnabled(bool enabled);
		void setTranslateEnabled(bool enabled);
		void fittingBox(const trimesh::box3& box, bool resetDir = true);
		void updateNearFar(const trimesh::box3& box);
	protected:
		void notifyViewMatrix();
		void notifyProjectionMatrix();
		void notifyScreenSize(float width, float height);

		void onResize(float width, float height) override;

		void setCameraScreenSize(float width, float height);
		void setCameraNearFar(float fNear, float fFar);
		void setCameraPose(const trimesh::vec3& position, const trimesh::vec3& viewCenter,
			const trimesh::vec3& upVector);
		void scaleCamera(float factor);

		void translateCamera(const trimesh::vec3& trans);
		void screenTranslateCamera(float x, float y);
	protected:
		bool m_rotateEnabled;
		bool m_scaleEnabled;
		bool m_translateEnabled;

		float m_width;
		float m_height;

		Camera* m_camera;
		float m_maxFovy;
		float m_minFovy;
	};
}

#endif // CXND_CAMERACONTROLS_1638870835676_H