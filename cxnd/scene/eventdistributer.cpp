#include "eventdistributer.h"
#include "clickeventchecker.h"
#include "eventhandlers.h"

#include "ccglobal/container.h"

namespace cxnd
{
	EventDistributer::EventDistributer()
		: m_width(0)
		, m_height(0)
	{
		m_clickEventChecker.reset(new ClickEventChecker());
	}

	EventDistributer::~EventDistributer()
	{
	}

	void EventDistributer::resizeEvent(int width, int height)
	{
		if (m_width == width && m_height == height)
			return;

		m_width = width;
		m_height = height;

		for (ResizeEventHandler* handler : m_resizeEventHandlers)
			handler->onResize(m_width, m_height);
	}

	void EventDistributer::leftMousePressEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (LeftMouseEventHandler* handler : m_leftMouseEventHandlers)
		{
			handler->onLeftMouseButtonPress(snap);
			if (snap.processed())
				break;
		}

		m_clickEventChecker->leftMousePressEvent(x, y, time);
	}

	void EventDistributer::leftMouseMoveEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (LeftMouseEventHandler* handler : m_leftMouseEventHandlers)
		{
			handler->onLeftMouseButtonMove(snap);
			if (snap.processed())
				break;
		}

		m_clickEventChecker->mouseMove(x, y, time);
	}

	void EventDistributer::leftMouseReleaseEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (LeftMouseEventHandler* handler : m_leftMouseEventHandlers)
		{
			handler->onLeftMouseButtonRelease(snap);
			if (snap.processed())
				break;
		}

		if (m_clickEventChecker->checkLeftButton(x, y, time))
		{
			MouseSnap snap1(x, y, time);
			for (LeftMouseEventHandler* handler : m_leftMouseEventHandlers)
			{
				handler->onLeftMouseButtonClick(snap1);
				if (snap1.processed())
					break;
			}
		}
	}

	void EventDistributer::middleMousePressEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (MidMouseEventHandler* handler : m_midMouseEventHandlers)
		{
			handler->onMidMouseButtonPress(snap);
			if (snap.processed())
				break;
		}

		m_clickEventChecker->middleMousePressEvent(x, y, time);
	}

	void EventDistributer::middleMouseMoveEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (MidMouseEventHandler* handler : m_midMouseEventHandlers)
		{
			handler->onMidMouseButtonMove(snap);
			if (snap.processed())
				break;
		}

		m_clickEventChecker->mouseMove(x, y, time);
	}

	void EventDistributer::middleMouseReleaseEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (MidMouseEventHandler* handler : m_midMouseEventHandlers)
		{
			handler->onMidMouseButtonRelease(snap);
			if (snap.processed())
				break;
		}

		if (m_clickEventChecker->checkMiddleButton(x, y, time))
		{
			MouseSnap snap1(x, y, time);
			for (MidMouseEventHandler* handler : m_midMouseEventHandlers)
			{
				handler->onMidMouseButtonClick(snap1);
				if (snap1.processed())
					break;
			}
		}
	}

	void EventDistributer::rightMousePressEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (RightMouseEventHandler* handler : m_rightMouseEventHandlers)
		{
			handler->onRightMouseButtonPress(snap);
			if (snap.processed())
				break;
		}

		m_clickEventChecker->rightMousePressEvent(x, y, time);
	}

	void EventDistributer::rightMouseMoveEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (RightMouseEventHandler* handler : m_rightMouseEventHandlers)
		{
			handler->onRightMouseButtonMove(snap);
			if (snap.processed())
				break;
		}

		m_clickEventChecker->mouseMove(x, y, time);
	}

	void EventDistributer::rightMouseReleaseEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (RightMouseEventHandler* handler : m_rightMouseEventHandlers)
		{
			handler->onRightMouseButtonRelease(snap);
			if (snap.processed())
				break;
		}

		if (m_clickEventChecker->checkRightButton(x, y, time))
		{
			MouseSnap snap1(x, y, time);
			for (RightMouseEventHandler* handler : m_rightMouseEventHandlers)
			{
				handler->onRightMouseButtonClick(snap1);
				if (snap1.processed())
					break;
			}
		}
	}

	void EventDistributer::wheelEvent(float delta, double time)
	{
		WheelSnap snap(delta, time);
		for (WheelEventHandler* handler : m_wheelEventHandlers)
		{
			handler->onWheelEvent(snap);
			if (snap.processed())
				break;
		}
	}

	void EventDistributer::hoverMoveEvent(float x, float y, double time)
	{
		MouseSnap snap(x, y, time);
		for (HoverEventHandler* handler : m_hoverEventHandlers)
		{
			handler->onHoverMove(snap);
			if (snap.processed())
				break;
		}
	}

	void EventDistributer::keyPressEvent(int key, double time)
	{
		KeySnap snap(key, time);
		for (KeyEventHandler* handler : m_KeyEventHandlers)
		{
			handler->onKeyPress(snap);
			if (snap.processed())
				break;
		}
	}

	void EventDistributer::keyReleaseEvent(int key, double time)
	{
		KeySnap snap(key, time);
		for (KeyEventHandler* handler : m_KeyEventHandlers)
		{
			handler->onKeyRelease(snap);
			if (snap.processed())
				break;
		}
	}

	void EventDistributer::touchTranslate(float deltaX, float deltaY)
	{
		TouchSnap snap(deltaX, deltaY, 0.0);
		for (TouchEventHandler* handler : m_touchEventHandlers)
		{
			handler->onTouchTranslate(snap);
			if (snap.processed())
				break;
		}
	}

	void EventDistributer::touchScale(float scale)
	{
		TouchSnap snap(scale, 0.0f, 0.0);
		for (TouchEventHandler* handler : m_touchEventHandlers)
		{
			handler->onTouchScale(snap);
			if (snap.processed())
				break;
		}
	}

	void EventDistributer::oneTouchBegin(float x, float y)
	{
		TouchSnap snap(x, y, 0.0);
		for (TouchEventHandler* handler : m_touchEventHandlers)
		{
			handler->onOneTouchBegin(snap);
			if (snap.processed())
				break;
		}
	}

	void EventDistributer::oneTouchMove(float x, float y)
	{
		TouchSnap snap(x, y, 0.0);
		for (TouchEventHandler* handler : m_touchEventHandlers)
		{
			handler->onOneTouchMove(snap);
			if (snap.processed())
				break;
		}
	}

	void EventDistributer::oneTouchEnd()
	{
		for (TouchEventHandler* handler : m_touchEventHandlers)
		{
			handler->onOneTouchEnd();
		}
	}

	void EventDistributer::addResizeEventHandler(ResizeEventHandler* handler, bool front)
	{
		if (front)
		{
			LIST_ADD(m_resizeEventHandlers, handler);
		}
		else
		{
			LIST_ADD_BACK(m_resizeEventHandlers, handler);
		}
	}

	void EventDistributer::removeResizeEventHandler(ResizeEventHandler* handler)
	{
		LIST_REMOVE(m_resizeEventHandlers, handler);
	}

	void EventDistributer::closeResizeEventHandlers()
	{
		m_resizeEventHandlers.clear();
	}

	void EventDistributer::addHoverEventHandler(HoverEventHandler* handler)
	{
		LIST_ADD(m_hoverEventHandlers, handler);
	}

	void EventDistributer::removeHoverEventHandler(HoverEventHandler* handler)
	{
		LIST_REMOVE(m_hoverEventHandlers, handler);
	}

	void EventDistributer::closeHoverEventHandlers()
	{
		m_hoverEventHandlers.clear();
	}

	void EventDistributer::addWheelEventHandler(WheelEventHandler* handler)
	{
		LIST_ADD(m_wheelEventHandlers, handler);
	}

	void EventDistributer::removeWheelEventHandler(WheelEventHandler* handler)
	{
		LIST_REMOVE(m_wheelEventHandlers, handler);
	}

	void EventDistributer::closeWheelEventHandlers()
	{
		m_wheelEventHandlers.clear();
	}

	void EventDistributer::addRightMouseEventHandler(RightMouseEventHandler* handler)
	{
		LIST_ADD(m_rightMouseEventHandlers, handler);
	}

	void EventDistributer::removeRightMouseEventHandler(RightMouseEventHandler* handler)
	{
		LIST_REMOVE(m_rightMouseEventHandlers, handler);
	}

	void EventDistributer::closeRightMouseEventHandlers()
	{
		m_rightMouseEventHandlers.clear();
	}

	void EventDistributer::addMidMouseEventHandler(MidMouseEventHandler* handler)
	{
		LIST_ADD(m_midMouseEventHandlers, handler);
	}

	void EventDistributer::removeMidMouseEventHandler(MidMouseEventHandler* handler)
	{
		LIST_REMOVE(m_midMouseEventHandlers, handler);
	}

	void EventDistributer::closeMidMouseEventHandlers()
	{
		m_midMouseEventHandlers.clear();
	}

	void EventDistributer::addLeftMouseEventHandler(LeftMouseEventHandler* handler)
	{
		LIST_ADD(m_leftMouseEventHandlers, handler);
	}

	void EventDistributer::removeLeftMouseEventHandler(LeftMouseEventHandler* handler)
	{
		LIST_REMOVE(m_leftMouseEventHandlers, handler);
	}

	void EventDistributer::closeLeftMouseEventHandlers()
	{
		m_leftMouseEventHandlers.clear();
	}

	void EventDistributer::addKeyEventHandler(KeyEventHandler* handler)
	{
		LIST_ADD(m_KeyEventHandlers, handler);
	}

	void EventDistributer::removeKeyEventHandler(KeyEventHandler* handler)
	{
		LIST_REMOVE(m_KeyEventHandlers, handler);
	}

	void EventDistributer::closeKeyEventHandlers()
	{
		m_KeyEventHandlers.clear();
	}

	void EventDistributer::addTouchEventHandler(TouchEventHandler* handler)
	{
		LIST_ADD(m_touchEventHandlers, handler);
	}

	void EventDistributer::removeTouchEventHandler(TouchEventHandler* handler)
	{
		LIST_REMOVE(m_touchEventHandlers, handler);
	}

	void EventDistributer::closeTouchEventHandlers()
	{
		m_touchEventHandlers.clear();
	}

	void EventDistributer::closeHandlers()
	{
		closeLeftMouseEventHandlers();
		closeRightMouseEventHandlers();
		closeWheelEventHandlers();
		closeHoverEventHandlers();
		closeResizeEventHandlers();
		closeKeyEventHandlers();
		closeTouchEventHandlers();
	}
}