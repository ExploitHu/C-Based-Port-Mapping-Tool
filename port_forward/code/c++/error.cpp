#include "error.h"

std::string getWarnErrorMsg(int warnErrorCode) {
    for (int i = 0; i < sizeof(warn_error_string_map) / sizeof(WarnErrorMsg); i++) {
        if (warn_error_string_map[i].dwErrorCode == warnErrorCode)
		{
			return warn_error_string_map[i].szDesc;
		}
    }
    return "未定义错误";
}