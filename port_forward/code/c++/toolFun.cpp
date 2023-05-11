#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <algorithm>
#include <vector>
#include <algorithm>
#include "toolFun.h"
#include <string.h>
#include "error.h"

ToolFun::ToolFun() {}

const char*  ToolFun::stringToChar(const string &str) { return str.c_str(); }
const int    ToolFun::stringToInt(const string &str) { return atoi(ToolFun::stringToChar(str)); }
const double ToolFun::stringTodouble(const string &str) { return atof(ToolFun::stringToChar(str)); }
const float  ToolFun::stringTofloat(const string &str) { return (float)ToolFun::stringTodouble(str); }

const string ToolFun::joint(const vector<string> &vec, const string str){
    string res;

    for (int i = 0; i < vec.size(); i++)
    {
        if(0 != i) res += str;
        res += vec[i];
    }
    
    return res;
}

const string ToolFun::outPointerAddress(const void* ptr)
{
    // 将地址转换为字符串
    stringstream ss;
    ss << (void*)ptr;
    return ss.str();
}


const bool ToolFun::UseCheckPort(int nPort) 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(nPort);

    int status = bind(sock, (struct sockaddr *) &addr, sizeof(addr));

    if (status < 0)
    {
        close(sock);
        return true;
    }
  
    close(sock);
    return false;
}

const void ToolFun::UseCheckRangePort(int nStartPort, int nEndPort, std::vector<int>* pUsePort) 
{
    for (int tmpPort = nStartPort; tmpPort <= nEndPort; tmpPort++) 
        if(UseCheckPort(tmpPort)) pUsePort->push_back(tmpPort);
}

template <typename T>
const bool ToolFun::Contains(std::vector<T>& vec, const T& element) 
{
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}
//显式实例化，避免链接错误
template const bool ToolFun::Contains<int>(std::vector<int>&, const int&);

const int ToolFun::ExecStateOfSystem(int nRet,int nExpect)
{
    //printf("nRet:%d | nExpect:%d \n", nRet, nExpect);
    //printf("WIFEXITED(nRet) %d | WEXITSTATUS(nRet) %d  | nExpect:%d \n",WIFEXITED(nRet), WEXITSTATUS(nRet), nExpect);
    //printf("(WEXITSTATUS(nRet) == nExpect) : %d\n", ((int)WEXITSTATUS(nRet) == nExpect));
    if (WIFEXITED(nRet) && ((int)WEXITSTATUS(nRet) == nExpect))
    {

        return 0;
    }
    return -1;
}

const void ToolFun::PrintJson(const json_t* pInput)
{
    std::cout << json_dumps(pInput, JSON_INDENT(4));
}


const bool ToolFun::GetStrFromJson(const json_t *pJson, char *str, int strMaxSize)
{
    if (pJson && json_string_value(pJson))
    {
        if (0 == strMaxSize)
        {
            strcpy(str, json_string_value(pJson));
        }
        else
        {
            strncpy(str, json_string_value(pJson), strMaxSize - 1);
            str[strMaxSize - 1] = 0;
        }
        
        return true;
    }
    else
    {
        strcpy(str, "");
        return false;
    }
}

const bool ToolFun::GetStrFromJson(const json_t *pJson, string &szStr)
{
    if (pJson && json_string_value(pJson))
    {
        szStr = json_string_value(pJson);
        return true;
    }
    else
    {
        szStr = "";
        return false;
    }
}

const bool ToolFun::GetIntFromJson(const json_t *pJson, int &num)
{
    if (pJson)
    {
        num = json_integer_value(pJson);
        return true;
    }
    else
    {
        return false;
    }
}

const bool ToolFun::GetDblFromJson(const json_t *pJson, double &num)
{
    if (pJson)
    {
        num = json_real_value(pJson);
        return true;
    }
    else
    {
        return false;
    }
}

const string ToolFun::GetNohupCmd(const string szCmd, const string szNohupFolder)
{
    return "nohup " + szCmd + " > " + szNohupFolder + " 2>&1 &";
}

const int ToolFun::killProcessUseCmd(const string szCmd)
{
    if ("" == szCmd) return ERR_UNKNOWN;
    string szKillCmd = "kill `pgrep -f \"" + szCmd + "\"`";
    if (ToolFun::ExecStateOfSystem(::system(szKillCmd.c_str()), 0)) return ERR_EXEC_FAIL;

    return SUCCESS;
}