#ifndef CXND_CLICKEVENTCHECKER_1638843047771_H
#define CXND_CLICKEVENTCHECKER_1638843047771_H
#include "cxnd/interface.h"

namespace cxnd
{
	class CXND_API ClickEventChecker
	{
	public:
		ClickEventChecker();
		virtual ~ClickEventChecker();

		bool checkLeftButton(float x, float y, double time);
		bool checkRightButton(float x, float y, double time);
		bool checkMiddleButton(float x, float y, double time);
	
		void leftMousePressEvent(float x, float y, double time);
		void middleMousePressEvent(float x, float y, double time);
		void rightMousePressEvent(float x, float y, double time);

		void mouseMove(float x, float y, double time);

		void setXYDelta(float xyDelta);
		void setTimeDelta(double timeDelta);
	private:
		bool nullCheck();
		bool releaseCheck(float x, float y, double time);
	protected:
		int m_button;
		float m_pickX;
		float m_pickY;
		double m_pickTime;
	
		float m_xyDelta;
		double m_timeDelta;
	};
}

#endif // CXND_CLICKEVENTCHECKER_1638843047771_H