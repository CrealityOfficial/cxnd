#include "serial.h"
#include "cxnd/string/util.h"

#include "boost/nowide/cstdio.hpp"
#include "ccglobal/log.h"

namespace cxnd
{
	bool cxndSave(Serializeable& serializeable, const std::string& fileName, ccglobal::Tracer* tracer)
	{
		std::fstream out(fileName, std::ios::out | std::ios::binary);
		if (!out.is_open())
		{
			LOGE("cxndSave error. [%s]", fileName.c_str());
			out.close();
			return false;
		}


		int ver = serializeable.version();
		out.write((const char*)&ver, sizeof(int));
		bool result = serializeable.save(out, tracer);

		out.close();
		return result;
	}

	bool cxndLoad(Serializeable& serializeable, const std::string& fileName, ccglobal::Tracer* tracer)
	{
		std::fstream in(fileName, std::ios::in | std::ios::binary);
		if (!in.is_open())
		{
			LOGE("cxndSave error. [%s]", fileName.c_str());
			return false;
		}

		int ver = -1;
		in.read((char*)&ver, sizeof(int));
		bool result = serializeable.load(in, ver, tracer);

		in.close();
		return result;
	}
}