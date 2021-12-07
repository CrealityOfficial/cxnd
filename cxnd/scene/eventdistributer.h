#ifndef CXND_EVENTDISTRIBUTER_1638843198347_H
#define CXND_EVENTDISTRIBUTER_1638843198347_H
#include "cxnd/interface.h"
#include <list>
#include <memory>

namespace cxnd
{
	class ClickEventChecker;
	class HoverEventHandler;
	class WheelEventHandler;
	class RightMouseEventHandler;
	class MidMouseEventHandler;
	class LeftMouseEventHandler;
	class ResizeEventHandler;
	class KeyEventHandler;

	class CXND_API EventDistributer
	{
	public:
		EventDistributer();
		virtual ~EventDistributer();

		void resizeEvent(int width, int height);
		void leftMousePressEvent(float x, float y, double time);
		void leftMouseMoveEvent(float x, float y, double time);
		void leftMouseReleaseEvent(float x, float y, double time);
		void middleMousePressEvent(float x, float y, double time);
		void middleMouseMoveEvent(float x, float y, double time);
		void middleMouseReleaseEvent(float x, float y, double time);
		void rightMousePressEvent(float x, float y, double time);
		void rightMouseMoveEvent(float x, float y, double time);
		void rightMouseReleaseEvent(float x, float y, double time);
		void wheelEvent(float delta, double time);
		void hoverMoveEvent(float x, float y, double time);
		void keyPressEvent(int key, double time);
		void keyReleaseEvent(int key, double time);

		void addResizeEventHandler(ResizeEventHandler* handler);
		void removeResizeEventHandler(ResizeEventHandler* handler);
		void closeResizeEventHandlers();

		void addHoverEventHandler(HoverEventHandler* handler);
		void removeHoverEventHandler(HoverEventHandler* handler);
		void closeHoverEventHandlers();

		void addWheelEventHandler(WheelEventHandler* handler);
		void removeWheelEventHandler(WheelEventHandler* handler);
		void closeWheelEventHandlers();

		void addRightMouseEventHandler(RightMouseEventHandler* handler);
		void removeRightMouseEventHandler(RightMouseEventHandler* handler);
		void closeRightMouseEventHandlers();

		void addMidMouseEventHandler(MidMouseEventHandler* handler);
		void removeMidMouseEventHandler(MidMouseEventHandler* handler);
		void closeMidMouseEventHandlers();

		void addLeftMouseEventHandler(LeftMouseEventHandler* handler);
		void removeLeftMouseEventHandler(LeftMouseEventHandler* handler);
		void closeLeftMouseEventHandlers();

		void addKeyEventHandler(KeyEventHandler* handler);
		void removeKeyEventHandler(KeyEventHandler* handler);
		void closeKeyEventHandlers();

		void closeHandlers();
	protected:
		std::unique_ptr<ClickEventChecker> m_clickEventChecker;

		std::list<ResizeEventHandler*> m_resizeEventHandlers;
		std::list<HoverEventHandler*> m_hoverEventHandlers;
		std::list<WheelEventHandler*> m_wheelEventHandlers;
		std::list<RightMouseEventHandler*> m_rightMouseEventHandlers;
		std::list<MidMouseEventHandler*> m_midMouseEventHandlers;
		std::list<LeftMouseEventHandler*> m_leftMouseEventHandlers;
		std::list<KeyEventHandler*> m_KeyEventHandlers;
		
		int m_width;
		int m_height;
	};
}

#endif // CXND_EVENTDISTRIBUTER_1638843198347_H