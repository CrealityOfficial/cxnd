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

	std::vector<std::string> splitString(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> elems;
		size_t pos = 0;
		size_t len = str.length();
		size_t delim_len = delim.length();
		if (delim_len == 0)
			return elems;
		while (pos < len)
		{
			int find_pos = str.find(delim, pos);
			if (find_pos < 0)
			{
				std::string t = str.substr(pos, len - pos);
				if (!t.empty())
					elems.push_back(t);
				break;
			}
			std::string t = str.substr(pos, find_pos - pos);
			if (!t.empty())
				elems.push_back(t);
			pos = find_pos + delim_len;
		}
		return elems;
	}
}