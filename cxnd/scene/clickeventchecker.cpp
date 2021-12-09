#include "clickeventchecker.h"
#include <math.h>

namespace cxnd
{
	ClickEventChecker::ClickEventChecker()
		: m_button(-1)
		, m_pickX(- 1.0f)
		, m_pickY(- 1.0f)
		, m_pickTime(- 0.0)
		, m_xyDelta(2.0f)
		, m_timeDelta(0.3)
	{
	}

	ClickEventChecker::~ClickEventChecker()
	{

	}

	bool ClickEventChecker::checkLeftButton(float x, float y, double time)
	{
		if (!nullCheck())
			return false;
		if (m_button != 0)
			return false;

		return releaseCheck(x, y, time);
	}

	bool ClickEventChecker::checkRightButton(float x, float y, double time)
	{
		if (!nullCheck()) 
			return false;
		if (m_button != 1)
			return false;

		return releaseCheck(x, y, time);
	}

	bool ClickEventChecker::checkMiddleButton(float x, float y, double time)
	{
		if (!nullCheck())
			return false;
		if (m_button != 2)
			return false;

		return releaseCheck(x, y, time);
	}

	void ClickEventChecker::leftMousePressEvent(float x, float y, double time)
	{
		m_pickX = x;
		m_pickY = y;
		m_pickTime = time;
		m_button = 0;
	}

	void ClickEventChecker::middleMousePressEvent(float x, float y, double time)
	{
		m_pickX = x;
		m_pickY = y;
		m_pickTime = time;
		m_button = 1;
	}

	void ClickEventChecker::rightMousePressEvent(float x, float y, double time)
	{
		m_pickX = x;
		m_pickY = y;
		m_pickTime = time;
		m_button = 2;
	}

	void ClickEventChecker::mouseMove(float x, float y, double time)
	{
		m_button = -1;
	}

	bool ClickEventChecker::nullCheck()
	{
		if ((m_pickX >= 0) && (m_pickY >= 0) && (m_pickTime > 0) && (m_button >= 0))
			return true;
		return false;
	}

	bool ClickEventChecker::releaseCheck(float x, float y, double time)
	{
		float dx = fabs(x - m_pickX);
		float dy = fabs(y - m_pickY);
		double dt = time - m_pickTime;

		if (dx <= m_xyDelta && dy <= m_xyDelta && dt < m_timeDelta && dt > 0.0)
			return true;

		return false;
	}

	void ClickEventChecker::setXYDelta(float xyDelta)
	{
		m_xyDelta = xyDelta;
	}

	void ClickEventChecker::setTimeDelta(double timeDelta)
	{
		m_timeDelta = timeDelta;
	}
}