#ifndef CXND_INTERFACE_1604911737496_H
#define CXND_INTERFACE_1604911737496_H
#include "ccglobal/export.h"

#ifdef CXND_DLL
	#define CXND_API CC_DECLARE_EXPORT
#else
	#define CXND_API CC_DECLARE_IMPORT
#endif
#endif // CXND_INTERFACE_1604911737496_H