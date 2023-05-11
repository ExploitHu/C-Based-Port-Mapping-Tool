#ifndef ERROR_H
#define ERROR_H
#include <string.h>
#include <iostream>
#define SUCCESS  0
#define ERR_UNKNOWN -1


enum WarnErrorCode {
////////////////////////////////              Warn
	WARN = 0x1000,							//警告
	// ExecUI
	WARN_NOT_OBEJECT = 0x1100,				//字符串转json对象失败
	WARN_NOT_FIND_HEADER,					//找不到Header
	WARN_NOT_FIND_FUNCTION,					//找不到Function
	WARN_NOT_PAIR_FUNCTION,					//没有匹配的Function
	WARN_JSONOUT_IS_NULL,					//JsonOut is NULL
	WARN_INSUFFICIENT_PORTS,				//缺少端口，端口不足
	WARN_JSON_FORMAT,						//json格式错误
	WARN_USER_REGISTERED,					//用户已注册
	WARN_ACCOUNT_OR_PASSWORD,				//账号或密码错误
	WARN_CARD_SECRET,						//卡密错误
	WARN_CARD_SECRET_FAILURE,				//卡密失效
	WARN_PASSWORD,							//密码错误


////////////////////////////////            Error
    //  mysql
    ERR_MSQ_INSUFFICIENT_MEMORY = 0x10000, 	// 内存不足
    ERR_MSQ_FUNCTION_CALL_DEFEATED,       	// 调用函数失败
    ERR_MSQ_PTR_IS_NULL,                  	// 指针为NULL

    //  code 代码漏洞
	ERR_CREATE_RESPONSE_STRING = 0x10100, 	//创建响应string错误
	ERR_PTR_IS_NULL,                  	    // 指针为NULL

	//文件操作
	ERR_FILE_READ	           = 0x10200,	//读取文件错误	
	ERR_FILE_FORMAT,	          			//文件格式错误

	//命令执行
	ERR_EXEC_FAIL			   = 0x10300	//命令执行失败




};

static struct WarnErrorMsg{
	int dwErrorCode;
	const std::string szDesc;
}warn_error_string_map[] = {
	{SUCCESS,                                   "成功"},
	{ERR_UNKNOWN,                               "未知错误"},
	{WARN,                                 		"警告"},
	{WARN_NOT_OBEJECT,                     		"字符串转json对象失败"},
	{WARN_NOT_FIND_HEADER,                      "找不到Header"	},
	{WARN_NOT_FIND_FUNCTION,                    "找不到Function"},
	{WARN_NOT_PAIR_FUNCTION,                    "没有匹配的Function"},
	{WARN_JSONOUT_IS_NULL,                      "JsonOut is NULL"},
	{WARN_INSUFFICIENT_PORTS,                   "缺少端口，端口不足"},
	{WARN_JSON_FORMAT,							"json格式错误"},
	{WARN_USER_REGISTERED,						"用户已注册"},
	{WARN_ACCOUNT_OR_PASSWORD,					"账号或密码错误"},
	{WARN_CARD_SECRET,							"卡密错误"},
	{WARN_CARD_SECRET_FAILURE,					"卡密失效"},
	{WARN_PASSWORD,								"密码错误"},
	{ERR_MSQ_INSUFFICIENT_MEMORY,               "内存不足"},
	{ERR_MSQ_FUNCTION_CALL_DEFEATED,            "调用函数失败"},
	{ERR_MSQ_PTR_IS_NULL,                       "指针为NULL"},
	
	{ERR_CREATE_RESPONSE_STRING,                "创建响应string错误"},
	{ERR_PTR_IS_NULL,                  	    	"指针为NULL"},
	{ERR_FILE_READ,                             "读取文件错误"},
	{ERR_FILE_FORMAT,                           "文件格式错误"},
	{ERR_EXEC_FAIL,								"命令执行失败"}
};

std::string getWarnErrorMsg(int warnErrorCode);

#endif