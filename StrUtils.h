#ifndef HEADER_STR_UTILS
#define HEADER_STR_UTILS
#include <string>

class StrUtils {
public:
	static std::string pad_with_number(const std::string & str, unsigned int num);
	static std::wstring str_to_wstr(std::string str);
	static std::string wstr_to_str(std::wstring str);
	static std::string interpret_wstr_as_str(std::wstring wstr);
	static std::wstring interpret_str_as_wstr(std::string sstr);
};

#endif // HEADER_STR_UTILS