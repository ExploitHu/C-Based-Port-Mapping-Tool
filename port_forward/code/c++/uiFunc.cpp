#include "uiFunc.h"
#include "error.h"
#include "uiFuncSys.h"
#include "jansson.h" 
#include "toolFun.h"
#include "rsJson.h"
#include <string.h>
#include <iostream>

typedef int (*func_exec_ui)(const json_t* pInputJson, json_t** ppOutputJson);

struct platform_data
{
    string    szFunName;
    func_exec_ui    func;
};

static struct platform_data platform_func_table[] = 
{    
    {"unknown", NULL},
    {"Test", Test},
    {"PostPortForward", PostPortForward},
    {"ClosePortForward", ClosePortForward},
    {"RestartPortForward", RestartPortForward},
    {"UpdatePortForward", UpdatePortForward},
    {"GetPortForward", GetPortForward},
    {"SignIn", SignIn},
    {"Login", Login},
    {"RechargeWithCardCode", RechargeWithCardCode},
    {"BuyRes", BuyRes},
    {"GetUserBalance", GetUserBalance},
    {"GetResUnitPrice", GetResUnitPrice},
    {"GetResStatus", GetResStatus},
    {"GetBill", GetBill},
    {"UpdateUserPasswd", UpdateUserPasswd}
    

};


int GetCmdStringFromInJson(const char* szJsonBuf, json_t **pJsonIn)
{
    json_t *root = NULL;
    const char* p;
    json_error_t error;
    int resCode = SUCCESS;
    //字符串转json
    root = json_loads(szJsonBuf, 0, &error);
    if(root == NULL || !json_is_object(root)) 
    {
        resCode = WARN_NOT_OBEJECT;
        LOG_WARN(resCode,"str not json obeject, error text:" + (string)error.text);
        json_decref(root);
        return resCode;
    }

    *pJsonIn = root;
    return resCode;
}

RSL_UI_FUNC_API	int MYSTDCALL ExecUI(const char* szFuncName, const char* szIn, char** ppOut, int* pOutLen)
{
    cout << "进入--ExecUI***********************************************************************************************************\n";

    int resCode = SUCCESS;
    json_t *pJsonIn = NULL;
    json_t *pJsonOut = NULL;
    string functionName = string(szFuncName);
    time_t dwStartTime;
    time_t dwEndTime;
    bool bFound = false;
    dwStartTime = time(NULL);

    if (NULL != szIn) LOG_INFO(0, (string)"Function:" + string(szFuncName) + ",input json:" + string(szIn));

    //从szIn中提取传入json
    if(strcmp(szIn, ""))
    {
        resCode = GetCmdStringFromInJson(szIn, &pJsonIn);
        if(SUCCESS != resCode)
        {
            RsJson::CreateCustomsErrorRsPString(resCode, NULL, ppOut, pOutLen);
            if (NULL != pJsonIn) json_decref(pJsonIn);
            pJsonIn = NULL;
            return resCode;
        }
    }

    //调用对应的函数
    for(int i=0; i<sizeof(platform_func_table)/sizeof(platform_func_table[0]); i++)
    {
        if(platform_func_table[i].szFunName == functionName)
        {
            LOG_INFO(0, "start exec ui func:" + platform_func_table[i].szFunName);
            pJsonOut = json_object();
            resCode = (*(platform_func_table[i].func))(pJsonIn, &pJsonOut);
            LOG_INFO(resCode,"end exec ui func:" + platform_func_table[i].szFunName );
            bFound = true;
            break;
        }
    }

    //没有匹配的函数
    if(!bFound)
    {
        resCode = WARN_NOT_PAIR_FUNCTION; 
        LOG_WARN(resCode,"recv unknown cmd:"+ functionName);
        RsJson::CreateCustomsErrorRsPString(resCode, NULL, ppOut, pOutLen);
        if (NULL != pJsonIn) json_decref(pJsonIn); pJsonIn = NULL;
        return resCode;
    }
    
    if (NULL != pJsonOut)
    {
        char *pRetStr = json_dumps(pJsonOut, JSON_INDENT(4));
        if (NULL != pRetStr)
        {
            *pOutLen = strlen(pRetStr);
            *ppOut = new char[*pOutLen + 1];
            strcpy(*ppOut, pRetStr);
            free(pRetStr);
            dwEndTime = time(NULL);
            LOG_INFO(resCode,"ExecUI cmd[" + functionName + "] deal over, used " + to_string(dwEndTime-dwStartTime) + " seconds, ouput json:\n" + *ppOut);
        }
        json_decref(pJsonOut);
        return resCode;
    }
    else
    {
        resCode = WARN_JSONOUT_IS_NULL; 
        LOG_WARN(WARN_JSONOUT_IS_NULL,"pJsonOut is NULL, function name:"+ functionName);
        RsJson::CreateCustomsErrorRsPString(resCode, NULL, ppOut, pOutLen);
        if (NULL != pJsonIn) json_decref(pJsonIn); pJsonIn = NULL;
        return resCode;
    }

    return resCode;
}

RSL_UI_FUNC_API	int MYSTDCALL MyFreeMemory(char* pBuf)
{
    //string a = string(; 
	LOG_INFO(0, ("call func(MyFreeMemory),Buf address is:" + ToolFun::outPointerAddress((void*)pBuf)));
	if(NULL != pBuf) delete [] pBuf; pBuf = NULL;
	LOG_INFO(0, "func(MyFreeMemory) exec ok");
	
	return SUCCESS;
}