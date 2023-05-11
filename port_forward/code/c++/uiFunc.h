//对外接口
#ifndef UIFUNC_H
#define UIFUNC_H

#include "log.h"

#ifdef WIN32
#ifdef RSL_UI_FUNC_EXPORTS
#define RSL_UI_FUNC_API __declspec(dllexport)
#else
#define RSL_UI_FUNC_API __declspec(dllimport)
#endif
//#define MYSTDCALL __stdcall
#define MYSTDCALL  __cdecl
#else
#define RSL_UI_FUNC_API  
#define MYSTDCALL 
#endif

extern "C" {
    RSL_UI_FUNC_API	int MYSTDCALL ExecUI(const char* szFuncName, const char* szIn, char** ppOut, int* pOutLen);		//执行应用命令
    RSL_UI_FUNC_API	int MYSTDCALL MyFreeMemory(char* pBuf);			//清理内存
}

//int ExecUI(const char* szIn, char** ppOut, int* pOutLen);		//执行应用命令
#endif //UI_FUNC_H