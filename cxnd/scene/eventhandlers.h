#ifndef CXND_EVENTHANDLERS_1638843047772_H
#define CXND_EVENTHANDLERS_1638843047772_H
#include "cxnd/interface.h"

namespace cxnd
{
	//0 left button
	//1 middle button
	//2 right button

	class CXND_API SnapBase
	{
	public:
		SnapBase(double time);
		virtual ~SnapBase();

		bool processed();
		void setProcessed(bool processed);

		double time();
	protected:
		bool m_processed;
		double t;
	};

	class MouseSnap : public SnapBase
	{
	public:
		MouseSnap()
			:SnapBase(0.0)
			, x(0.0f)
			, y(0.0f)
		{

		}
		MouseSnap(float _x, float _y, double time)
			:SnapBase(time)
			, x(_x)
			, y(_y)
		{
		}

		bool operator == (const MouseSnap& snap) const
		{
			return x == snap.x && y == snap.y;
		}

		float x;
		float y;
	};

	class KeySnap : public SnapBase
	{
	public:
		KeySnap(int _key, double time)
			:SnapBase(time)
			, key(_key)
		{
		}

		int key;
	};

	class WheelSnap : public SnapBase
	{
	public:
		WheelSnap(float _delta, double time)
			:SnapBase(time)
			, delta(_delta)
		{
		}

		float delta;  //
	};

	class ResizeEventHandler
	{
	public:
		virtual ~ResizeEventHandler() {}

		virtual void onResize(float width, float height) = 0;
	};

	class HoverEventHandler
	{
	public:
		virtual ~HoverEventHandler() {}

		virtual void onHoverMove(MouseSnap& mouse) = 0;
	};

	class WheelEventHandler
	{
	public:
		virtual ~WheelEventHandler() {}

		virtual void onWheelEvent(WheelSnap& snap) = 0;
	};

	class LeftMouseEventHandler
	{
	public:
		virtual ~LeftMouseEventHandler() {}

		virtual void onLeftMouseButtonPress(MouseSnap& mouse) = 0;
		virtual void onLeftMouseButtonRelease(MouseSnap& mouse) = 0;
		virtual void onLeftMouseButtonMove(MouseSnap& mouse) = 0;
		virtual void onLeftMouseButtonClick(MouseSnap& mouse) = 0;
	};

	class MidMouseEventHandler
	{
	public:
		virtual ~MidMouseEventHandler() {}

		virtual void onMidMouseButtonPress(MouseSnap& mouse) = 0;
		virtual void onMidMouseButtonRelease(MouseSnap& mouse) = 0;
		virtual void onMidMouseButtonMove(MouseSnap& mouse) = 0;
		virtual void onMidMouseButtonClick(MouseSnap& mouse) = 0;
	};

	class RightMouseEventHandler
	{
	public:
		virtual ~RightMouseEventHandler() {}

		virtual void onRightMouseButtonPress(MouseSnap& mouse) = 0;
		virtual void onRightMouseButtonRelease(MouseSnap& mouse) = 0;
		virtual void onRightMouseButtonMove(MouseSnap& mouse) = 0;
		virtual void onRightMouseButtonClick(MouseSnap& mouse) = 0;
	};

	class KeyEventHandler
	{
	public:
		virtual ~KeyEventHandler() {}
		virtual void onKeyPress(KeySnap& key) = 0;
		virtual void onKeyRelease(KeySnap& key) = 0;
	};
}

#endif // CXND_EVENTHANDLERS_1638843047772_H