#include "orbitcontrols.h"
#include "camera.h"
#include "cameraspace.h"

#include "trimesh2/quaternion.h"

#include <math.h>

namespace cxnd
{
	OrbitControls::OrbitControls()
		:m_operationMode(0)
	{

	}

	OrbitControls::~OrbitControls()
	{

	}

	void OrbitControls::onWheelEvent(WheelSnap& snap)
	{
		if (m_operationMode == 0 && scaleEnabled())
		{
			float delta = snap.delta > 0.0f ? 0.05f : -0.05f;

			float fScaleFactor = 1.0f + delta;
			scaleCamera(fScaleFactor);
		}
	}

	void OrbitControls::onLeftMouseButtonPress(MouseSnap& mouse)
	{
		if (m_operationMode == 0)
		{
			m_operationMode = 1;
			m_lastSnap = mouse;
		}
	}

	void OrbitControls::onLeftMouseButtonRelease(MouseSnap& mouse)
	{
		if (m_operationMode == 1)
			m_operationMode = 0;
	}

	void OrbitControls::onLeftMouseButtonMove(MouseSnap& mouse)
	{

	}

	void OrbitControls::onLeftMouseButtonClick(MouseSnap& mouse)
	{

	}

	void OrbitControls::onMidMouseButtonPress(MouseSnap& mouse)
	{
		if (m_operationMode == 0)
		{
			m_operationMode = 2;
			m_lastSnap = mouse;
		}
	}

	void OrbitControls::onMidMouseButtonRelease(MouseSnap& mouse)
	{
		if (m_operationMode == 2)
			m_operationMode = 0;
	}

	void OrbitControls::onMidMouseButtonMove(MouseSnap& mouse)
	{
		if (m_operationMode == 2 && translateEnabled() && !(mouse == m_lastSnap))
		{
			performTranslate(m_lastSnap, mouse);
			m_lastSnap = mouse;
		}
	}

	void OrbitControls::onMidMouseButtonClick(MouseSnap& mouse)
	{

	}

	void OrbitControls::onRightMouseButtonPress(MouseSnap& mouse)
	{
		if (m_operationMode == 0)
		{
			m_operationMode = 3;
			m_lastSnap = mouse;
		}
	}

	void OrbitControls::onRightMouseButtonRelease(MouseSnap& mouse)
	{
		if (m_operationMode == 3)
			m_operationMode = 0;
	}

	void OrbitControls::onRightMouseButtonMove(MouseSnap& mouse)
	{
		if (m_operationMode == 3 && rotateEnabled() && !(mouse == m_lastSnap))
		{
			performRotate(m_lastSnap, mouse);
			m_lastSnap = mouse;
		}
	}

	void OrbitControls::onRightMouseButtonClick(MouseSnap& mouse)
	{

	}

	void OrbitControls::onHoverMove(MouseSnap& mouse)
	{
		m_operationMode = 0;
	}

	void OrbitControls::onTouchTranslate(TouchSnap& snap)
	{
		if (translateEnabled())
			screenTranslateCamera(snap.touch1, snap.touch2);
	}

	void OrbitControls::onTouchScale(TouchSnap& snap)
	{
		if(scaleEnabled())
		{
			//LOGI("OrbitControls scale %f", snap.touch1);
			float fScaleFactor = snap.touch1;
			scaleCamera(fScaleFactor);
		}
	}

	void OrbitControls::onOneTouchBegin(TouchSnap& snap)
	{
		//LOGI("OrbitControls onOneTouchBegin");
		m_lastTouch = snap;
	}

	void OrbitControls::onOneTouchMove(TouchSnap& snap)
	{
		//LOGI("OrbitControls onOneTouchMove");
		if (rotateEnabled() && !(snap == m_lastTouch))
		{
			MouseSnap m1(m_lastTouch.touch1, m_lastTouch.touch2, 0.0);
			MouseSnap m2(snap.touch1, snap.touch2, 0.0);
			performRotate(m1, m2);
			m_lastTouch = snap;
		}
	}

	void OrbitControls::onOneTouchEnd()
	{
		//LOGI("OrbitControls onOneTouchEnd");
	}

	void OrbitControls::performTranslate(MouseSnap& snap1, MouseSnap& snap2)
	{
		trimesh::vec3 viewCenter = m_camera->viewCenter;
		trimesh::vec3 position = m_camera->position;

		trimesh::vec3 dir = viewCenter - position;
		trimesh::normalize(dir);

		Ray ray0 = screenRay(*m_camera, trimesh::vec2(snap1.x, snap1.y), trimesh::vec2(m_width, m_height));
		Ray ray = screenRay(*m_camera, trimesh::vec2(snap2.x, snap2.y), trimesh::vec2(m_width, m_height));

		trimesh::vec3 c0, c;
		bool b0 = ray0.collidePlane(viewCenter, dir, c0);
		bool b = ray.collidePlane(viewCenter, dir, c);

		if (!b0 || !b)
			return;

		trimesh::vec3 delta = c0 - c;
		trimesh::vec3 newPosition = position + delta;
		trimesh::vec3 newViewCenter = viewCenter + delta;
		trimesh::vec3 newUpVector = m_camera->upVector;

		setCameraPose(newPosition, newViewCenter, newUpVector);

		updateNearFar(m_visualBox);
	}

	void OrbitControls::performRotate(MouseSnap& snap1, MouseSnap& snap2)
	{
        if (m_rotateLimit) {
            performRotateLimit(snap1, snap2);
        } else {
            performRotateFreedom(snap1, snap2);
        }
	}

    void OrbitControls::performRotateLimit(MouseSnap &snap1, MouseSnap &snap2)
    {
        trimesh::vec2 delta = trimesh::vec2(snap2.x - snap1.x, snap2.y - snap1.y);

        float hangle = -0.1f * (float)delta.x;
        trimesh::quaternion hq = trimesh::quaternion::fromAxisAndAngle(trimesh::vec3(0.0f, 0.0f, 1.0f), hangle);

        trimesh::vec3 viewCenter = m_camera->viewCenter;
        trimesh::vec3 position = m_camera->position;
        trimesh::vec3 horizontal = m_camera->horizontal();

        trimesh::vec3 h = hq * horizontal;
        trimesh::normalize(h);

        float vangle = m_camera->verticalAngle() + 0.003f * (float)delta.y;
        if (vangle < 0.0f) vangle = 0.0f;
        if (vangle > M_PI) vangle = (float)M_PI;

        trimesh::vec3 dir;
        trimesh::vec3 right = h;
        if (vangle > 0.0f && vangle < M_PI)
        {
            dir = trimesh::vec3(-h.y, h.x, 0.0f);
            float z = trimesh::len(dir) / (tanf(vangle));
            dir.z = z;
            trimesh::normalize(dir);
        }
        else if (vangle <= 0.0f)
        {
            dir = trimesh::vec3(0.0f, 0.0f, 1.0f);
        }
        else if (vangle >= M_PI)
        {
            dir = trimesh::vec3(0.0f, 0.0f, -1.0f);
        }

        trimesh::vec3 saveDir = viewCenter - position;
        float distance = trimesh::len(saveDir);

        trimesh::vec3 newPosition = viewCenter - dir * distance;
        trimesh::vec3 up = right TRICROSS dir;
        
        if (up.x == 0.0f && up.y == 0.0f && up.z == 0.0f)
            return;

        setCameraPose(newPosition, viewCenter, up);

        updateNearFar(m_visualBox);
    }

    void OrbitControls::performRotateFreedom(MouseSnap& snap1, MouseSnap& snap2)
    {
        trimesh::vec2 delta = trimesh::vec2(snap2.x - snap1.x, snap2.y - snap1.y);
        delta = trimesh::vec2(delta.x, -delta.y);
        
        trimesh::vec3 viewCenter = m_camera->viewCenter;
        trimesh::vec3 position = m_camera->position;
        trimesh::vec3 horizontal = m_camera->horizontal();
        
        trimesh::vec3 worldCenter = viewCenter;
        trimesh::vec3 target = position - worldCenter;
        float radius = trimesh::length(target);
        trimesh::normalize(target);
        
        float sensitive = 0.004f * radius;
        
        trimesh::vec3 right = m_camera->upVector TRICROSS target;
        trimesh::normalize(right);
        
        trimesh::vec3 up = target TRICROSS right;
        trimesh::normalize(up);
        
        trimesh::vec3 offset = (up * delta.y + right * delta.x) * sensitive;
        
        float len = trimesh::length(offset);
        
        float angle = -len / radius;
        trimesh::normalize(offset);
        
        float cr = cos(angle) * radius, sr = sin(angle) * radius;
        
        trimesh::vec3 newPosition = worldCenter + trimesh::vec3(target.x * cr + offset.x * sr,
                                                   target.y * cr + offset.y * sr,
                                                   target.z * cr + offset.z * sr);
        setCameraPose(newPosition, viewCenter, up);

        updateNearFar(m_visualBox);
    }

    
}
