#include "idbase.h"
#include <mutex>

namespace cxnd
{
	int globalID = 0;
	std::mutex globalMutex;
	int generateID()
	{
		globalMutex.lock();
		int id = globalID++;
		globalMutex.unlock();
		return id;
	}

	IDBase::IDBase()
		:m_ID(generateID())
	{

	}

	IDBase::~IDBase()
	{

	}

	int IDBase::ID()
	{
		return m_ID;
	}
}