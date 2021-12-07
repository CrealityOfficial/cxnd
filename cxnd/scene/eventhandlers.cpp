#include "eventhandlers.h"

namespace cxnd
{
	SnapBase::SnapBase(double time)
		: m_processed(false)
		, t(time)
	{

	}

	SnapBase::~SnapBase()
	{

	}

	bool SnapBase::processed()
	{
		return m_processed;
	}

	void SnapBase::setProcessed(bool prod)
	{
		m_processed = prod;
	}

	double SnapBase::time()
	{
		return t;
	}
}
