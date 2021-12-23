#ifndef CXND_IDBASE_1640174057749_H
#define CXND_IDBASE_1640174057749_H
#include "cxnd/interface.h"

namespace cxnd
{
	CXND_API int generateID();

	class CXND_API IDBase
	{
	public:
		IDBase();
		virtual ~IDBase();

		int ID();
	protected:
		int m_ID;
	};
}

#endif // CXND_IDBASE_1640174057749_H