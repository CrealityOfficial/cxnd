#ifndef CXND_INTERFACE_1604911737496_H
#define CXND_INTERFACE_1604911737496_H
#include "ccglobal/export.h"

#if USE_CXND_DLL
#define CXND_API CC_DECLARE_IMPORT
#elif USE_CXND_STATIC
#define CXND_API CC_DECLARE_STATIC
#else
#if CXND_DLL
#define CXND_API CC_DECLARE_EXPORT
#else
#define CXND_API CC_DECLARE_STATIC
#endif
#endif

#include <string>
#include <memory>
#include <fstream>
#include <vector>

#endif // CXND_INTERFACE_1604911737496_H