#include "serial.h"
#include "cxnd/string/util.h"

#include "boost/nowide/cstdio.hpp"
#include "ccglobal/log.h"

namespace cxnd
{
	bool cxndSave(Serializeable& serializeable, const std::wstring& fileName, ccglobal::Tracer* tracer)
	{
		std::string name = wstring2String(fileName);
		FILE* f = boost::nowide::fopen(name.c_str(), "wb");

		if (!f)
		{
			LOGE("cxndSave error. [%s]", name.c_str());
			return false;
		}

		std::fstream out(f);
		int ver = serializeable.version();
		out.write((const char*)&ver, sizeof(int));
		bool result = serializeable.save(out, tracer);

		fclose(f);
		return result;
	}

	bool cxndLoad(Serializeable& serializeable, const std::wstring& fileName, ccglobal::Tracer* tracer)
	{
		std::string name = wstring2String(fileName);
		FILE* f = boost::nowide::fopen(name.c_str(), "rb");

		if (!f)
		{
			LOGE("cxndSave error. [%s]", name.c_str());
			return false;
		}

		std::fstream in(f);
		int ver = -1;
		in.read((char*)&ver, sizeof(int));
		bool result = serializeable.load(in, ver, tracer);

		fclose(f);
		return result;
	}
}