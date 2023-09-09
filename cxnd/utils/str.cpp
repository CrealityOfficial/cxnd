#include "str.h"
#include <locale>
#include <codecvt>

namespace cxnd
{
	std::string wstring2String(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

	std::string wchar2String(const wchar_t* wc)
	{
		std::wstring tmp = std::wstring(wc);
		return wstring2String(tmp);
	}
}