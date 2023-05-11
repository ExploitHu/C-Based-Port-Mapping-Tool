#include "rsJson.h"
#include "error.h"


const void RsJson::CreateStatuMsgJson(json_t* pStatuJson, const int nRetCode, const char* szCustomStatuMsg)
{
    if(!pStatuJson) return;

    json_object_set_new(pStatuJson, "StatusCode", json_integer(nRetCode));
    if(!szCustomStatuMsg) 
        json_object_set_new(pStatuJson, "Msg2Client", json_string(getWarnErrorMsg(nRetCode).c_str()));
	else 
        json_object_set_new(pStatuJson, "Msg2Client", json_string(szCustomStatuMsg));
}

const void RsJson::CreateRsErrRsPString(char** ppOut, int* pOutLen)
{
    const int nRet = ERR_CREATE_RESPONSE_STRING;
    json_t *pJsonStatus = NULL;
    json_t *ppOutputJson = NULL;
	
	pJsonStatus = json_object();
    CreateStatuMsgJson(pJsonStatus, nRet);
	
	ppOutputJson = json_object();
	json_object_set_new(ppOutputJson, "Status", pJsonStatus);

    char *pRetStr = json_dumps(pJsonStatus, JSON_INDENT(4));
    if (NULL != pRetStr)
    {
        *pOutLen = strlen(pRetStr);
        *ppOut = new char[*pOutLen + 1];
        strcpy(*ppOut, pRetStr);
        free(pRetStr);
    }
}

const void RsJson::CreateGenRsPJson(json_t** ppOutputJson, const int nRetCode, json_t *pOutDataJson, json_t *pPagingJson)
//void CreateCommonRspJson(const json_t *pInputJson, const int nErrorCode, json_t *pOutDataJson, json_t *pPagingJson, json_t** ppOutputJson)
{
	json_t *pJsonStatus = NULL;
    //json_t *pJsonIn = NULL;
    //json_t *pPagingJsonLocal = NULL;
	//char* pRetStr = NULL;
	
	pJsonStatus = json_object();
    CreateStatuMsgJson(pJsonStatus, nRetCode);
	
	json_object_set_new(*ppOutputJson, "Status", pJsonStatus);
	if (NULL != pOutDataJson)
	{
		json_object_set_new(*ppOutputJson, "Data", json_deep_copy(pOutDataJson));
	}

	/*if (NULL != pPagingJson)
	{
		pPagingJsonLocal = json_deep_copy(pPagingJson);
		json_t *pInputPaging = json_object_get(pInputJson, "Paging");
		json_object_set_new(pPagingJsonLocal, "QueryPageNo", json_deep_copy(json_object_get(pInputPaging, "QueryPageNo")));
		json_object_set_new(pPagingJsonLocal, "QueryPageLength", json_deep_copy(json_object_get(pInputPaging, "QueryPageLength")));
		json_object_set_new(*ppOutputJson, "Paging", pPagingJsonLocal);
	}*/
}


const void RsJson::CreateCustomsErrorRsPJson(json_t** ppOutputJson, const int nRetCode, const char* szCustomErrorMsg)
{
    json_t *pJsonStatus = NULL;

	pJsonStatus = json_object();
    CreateStatuMsgJson(pJsonStatus, nRetCode,szCustomErrorMsg);
	json_object_set_new(*ppOutputJson, "Status", pJsonStatus);
}

const void RsJson::CreateGenRsPString(const json_t* pInputJson, char** ppOut, int* pOutLen)
{
    char *pRetStr = json_dumps(pInputJson, JSON_INDENT(4));
    if (NULL != pRetStr)
    {
        *pOutLen = strlen(pRetStr);
        *ppOut = new char[*pOutLen + 1];
        strcpy(*ppOut, pRetStr);
        free(pRetStr);
    }
    else
    {
        RsJson::CreateRsErrRsPString(ppOut, pOutLen);
        return;
    }
}

const void RsJson::CreateCustomsErrorRsPString(const int nRetCode, const char* szCustomErrorMsg, char** ppOut, int* pOutLen)
{
    json_t* pOutputJson = json_object();
    json_t* pStatuMsg = json_object();
    
    json_object_set_new(pOutputJson, "Status", pStatuMsg);
    CreateStatuMsgJson(pStatuMsg, nRetCode, szCustomErrorMsg);

    char *pRetStr = json_dumps(pOutputJson, JSON_INDENT(4));
    
    if (NULL != pRetStr)
    {
        *pOutLen = strlen(pRetStr);
        *ppOut = new char[*pOutLen + 1];
        strcpy(*ppOut, pRetStr);
        free(pRetStr);
    }
    else RsJson::CreateRsErrRsPString(ppOut, pOutLen);
    
    json_decref(pOutputJson);
}