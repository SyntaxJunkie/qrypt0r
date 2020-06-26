#include "StrUtils.h"
#include <codecvt>

std::string StrUtils::pad_with_number(const std::string & str, unsigned int num) {
	std::string res(4, 0); //make string with 4 null bytes
	res.append(str); //append str parameter
	*reinterpret_cast<unsigned int*>(&res[0]) = num; //fill in the null bytes with the num parameter
	return res;
}

std::wstring StrUtils::str_to_wstr(std::string str) {
	std::wstring widestr = std::wstring(str.begin(), str.end());
	return widestr;
}

std::string StrUtils::wstr_to_str(std::wstring str) {
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	std::string converted_str = converter.to_bytes(str);
	return converted_str;
}

std::string StrUtils::interpret_wstr_as_str(std::wstring wstr) {
	std::string res(reinterpret_cast<char*>(&wstr[0]), reinterpret_cast<char*>(&wstr[0] + wstr.size()));
	return res;
}

std::wstring StrUtils::interpret_str_as_wstr(std::string sstr) {
	std::wstring res(reinterpret_cast<wchar_t*>(&sstr[0]), reinterpret_cast<wchar_t*>(&sstr[0] + sstr.size()));
	return res;
}