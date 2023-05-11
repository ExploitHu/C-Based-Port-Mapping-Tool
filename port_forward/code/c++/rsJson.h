#ifndef REJSON_H
#define REJSON_H

#include <iostream>
#include "jansson.h"

using namespace std;

class RsJson
{
private:
    RsJson();
    /**
     * 创建一个创建响应错误的响应char指针
     * @param ppOut              将要响应给客户端的字符串
     * @param pOutLen            将要响应给客户端的字符串长度
     **/
    static const void CreateRsErrRsPString(char** ppOut, int* pOutLen);
    static const void CreateStatuMsgJson(json_t* pStatuJson, const int nRetCode, const char* szCustomStatuMsg = NULL);
public:

    /**
     * 创建一个基础的响应Json指针
     * @param ppOutputJson 将要响应给客户端的Json
     * @param nRetCode     状态码
     * @param pOutDataJson 返回的DataJson
     * @param pPagingJson  分页Json
     **/
    static const void CreateGenRsPJson(json_t** ppOutputJson, const int nRetCode, json_t *pOutDataJson = NULL, json_t *pPagingJson = NULL);
    /**
     * 创建一个自定义的响应char指针
     * @param ppOutputJson     将要响应给客户端的Json
     * @param nRetCode         状态码
     * @param szCustomStatuMsg 自定义状态信息
     **/
    static const void CreateCustomsErrorRsPJson(json_t** ppOutputJson, const int nRetCode, const char* szCustomErrorMsg);
    /**
     * 创建一个自定义的响应char指针
     * @param pOutputJson           将要响应给客户端的Json
     * @param ppOut              将要响应给客户端的字符串
     * @param pOutLen            将要响应给客户端的字符串长度
     **/
    static const void CreateGenRsPString(const json_t* pOutputJson, char** ppOut, int* pOutLen);
    /**
     * 创建一个自定义的响应char指针
     * @param nRetCode           将要响应给客户端的状态码
     * @param szCustomStatuMsg   自定义状态信息
     * @param ppOut              将要响应给客户端的字符串
     * @param pOutLen            将要响应给客户端的字符串长度
     **/
    static const void CreateCustomsErrorRsPString(const int nRetCode, const char* szCustomStatuMsg, char** ppOut, int* pOutLen);

  
};


#endif
