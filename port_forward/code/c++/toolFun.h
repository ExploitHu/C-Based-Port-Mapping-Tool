#ifndef TOOLFUN_H
#define TOOLFUN_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "jansson.h"
#include "utmConst.h"
//#define NAME(variable) (#variable)

using namespace std;



class ToolFun
{
private:
    ToolFun();
public:
    //string转其他类型
    static const char*  stringToChar(const string& str);
    static const int    stringToInt(const string& str);
    static const double stringTodouble(const string& str);
    static const float  stringTofloat(const string& str);

    //用符号隔开vector中各个元素
    static const string  joint(const vector<string> &vec, const string str = ",");

    //out指针地址
    static const string outPointerAddress(const void* ptr);

    //检查端口是否被使用
    static const bool UseCheckPort(int nPort);

    //检查startPort - endPort范围内的端口是否被使用，并将其加入到pUsePort里
    static const void UseCheckRangePort(int nStartPort, int nEndPort, std::vector<int>* pUsePort);

    //检查vector是否含有某个元素
    template <typename T>
    static const bool Contains(std::vector<T>& vec, const T& element);

    //system函数返回值判断
    /**
     * @param nRet              System函数返回值
     * @param nExpect           期望的System函数返回值
    */
    static const int ExecStateOfSystem(int nRet,int nExpect);

    static const void PrintJson(const json_t* pInput);

    static const bool GetStrFromJson(const json_t *pJson, char *str, int strMaxSize);

    static const bool GetStrFromJson(const json_t *pJson, string &szStr);

    static const bool GetIntFromJson(const json_t *pJson, int &num);

    static const bool GetDblFromJson(const json_t *pJson, double &num);

    static const string GetNohupCmd(const string szCmd, const string szNohupFolder = FILE_NOHUP);

    static const int killProcessUseCmd(const string szCmd);

};



#endif /* !MD5_H */