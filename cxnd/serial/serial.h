#ifndef CXND_SERIAL_1691646227051_H
#define CXND_SERIAL_1691646227051_H
#include "cxnd/interface.h"
#include "ccglobal/tracer.h"
#include "boost/nowide/fstream.hpp"

namespace cxnd
{
	class Serializeable
	{
	public:
		virtual ~Serializeable() {}

		virtual int version() = 0;
		virtual bool save(boost::nowide::fstream& out, ccglobal::Tracer* tracer) = 0;
		virtual bool load(boost::nowide::fstream& in, int ver, ccglobal::Tracer* tracer) = 0;
	};

	CXND_API bool cxndSave(Serializeable& serializeable, const std::wstring& fileName, ccglobal::Tracer* tracer = nullptr);
	CXND_API bool cxndLoad(Serializeable& serializeable, const std::wstring& fileName, ccglobal::Tracer* tracer = nullptr);

	template<class T>
	void cxndLoadT(boost::nowide::fstream& in, T& t)
	{
		in.read((char*)&t, sizeof(T));
	}

	template<class T>
	void cxndSaveT(boost::nowide::fstream& out, const T& t)
	{
		out.write((const char*)&t, sizeof(T));
	}

	template<class T>
	void cxndLoadVectorT(boost::nowide::fstream& in, std::vector<T>& vecs)
	{
		int num = 0;
		cxndLoadT(in, num);
		if (num > 0)
		{
			vecs.resize(num);
			in.read((char*)&vecs.at(0), num * sizeof(T));
		}
	}

	template<class T>
	void cxndSaveVectorT(boost::nowide::fstream& out, const std::vector<T>& vecs)
	{
		int num = (int)vecs.size();
		cxndSaveT(out, num);
		if (num > 0)
			out.write((const char*)&vecs.at(0), num * sizeof(T));
	}
}

#endif // CXND_SERIAL_1691646227051_H