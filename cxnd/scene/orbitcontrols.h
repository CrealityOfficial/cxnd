#ifndef CXND_ORBITCONTROLS_1638870835676_H
#define CXND_ORBITCONTROLS_1638870835676_H
#include "cxnd/scene/cameracontrols.h"

namespace cxnd
{
	class CXND_API OrbitControls : public CameraControls
	{
	public:
		OrbitControls();
		virtual ~OrbitControls();

	protected:
		void onWheelEvent(WheelSnap& snap) override;
		void onLeftMouseButtonPress(MouseSnap& mouse) override;
		void onLeftMouseButtonRelease(MouseSnap& mouse) override;
		void onLeftMouseButtonMove(MouseSnap& mouse) override;
		void onLeftMouseButtonClick(MouseSnap& mouse) override;
		void onMidMouseButtonPress(MouseSnap& mouse) override;
		void onMidMouseButtonRelease(MouseSnap& mouse) override;
		void onMidMouseButtonMove(MouseSnap& mouse) override;
		void onMidMouseButtonClick(MouseSnap& mouse) override;
		void onRightMouseButtonPress(MouseSnap& mouse) override;
		void onRightMouseButtonRelease(MouseSnap& mouse) override;
		void onRightMouseButtonMove(MouseSnap& mouse) override;
		void onRightMouseButtonClick(MouseSnap& mouse) override;
		void onHoverMove(MouseSnap& mouse) override;
		void onTouchTranslate(TouchSnap& snap) override;
		void onTouchScale(TouchSnap& snap) override;
		void onOneTouchBegin(TouchSnap& snap) override;
		void onOneTouchMove(TouchSnap& snap) override;
		void onOneTouchEnd() override;

		void performTranslate(MouseSnap& snap1, MouseSnap& snap2);
		void performRotate(MouseSnap& snap1, MouseSnap& snap2);
	protected:
		MouseSnap m_lastSnap;
		TouchSnap m_lastTouch;
		int m_operationMode;
	};
}

#endif // CXND_ORBITCONTROLS_1638870835676_H