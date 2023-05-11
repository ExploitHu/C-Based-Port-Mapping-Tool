#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <string>

#include "uiFuncSys.h"
#include "rsJson.h"
#include "toolFun.h"
#include "error.h"
#include "log.h"
#include "utmConst.h"
#include "config.h"
#include "obDb.h"

using namespace std;

int GetJsonFileArray(const string szFileName, json_t*& pjsonData)
{
    int nRet = SUCCESS;
    json_t *pUsePortRoot = NULL;
    json_error_t error;
    
    //读取当前端口端口使用情况文件
    pjsonData = json_load_file(FILE_DATA_USE_PORT, 0, &error);
    if (!pjsonData) 
    {
        nRet = ERR_FILE_READ;
        LOG_ERROR(nRet, (string)"读取文件错误，文件路径：" + FILE_DATA_USE_PORT);
        goto FINISH;
    }

    if(!json_is_array(pjsonData))
    {
        nRet = ERR_FILE_READ;
        LOG_ERROR(nRet, (string)"文件格式错误，文件路径：" + FILE_DATA_USE_PORT);
        goto FINISH;
    }

FINISH:
    return nRet;
}


//从nStartPort到nEndPort范围内获取nNum个闲置端口
int GetFreePort(int nNum, int nStartPort, int nEndPort, std::vector<int>* pUsePort)
{
    int nRet = SUCCESS;
    std::vector<int> vUsePort;
    json_t *pUsePortData = NULL;
    size_t index;
    json_t *pValue;

    //读取当前端口端口使用情况文件
    if(nRet = GetJsonFileArray(FILE_DATA_USE_PORT, pUsePortData)) goto FINISH;

    json_array_foreach(pUsePortData, index, pValue)  
        if (json_is_integer(pValue))  
            vUsePort.push_back(json_integer_value(pValue));
        else 
        {
            nRet = ERR_FILE_READ;
            goto FINISH;
        }

    for (int nPort = nStartPort, getFreePortNum = 0; nPort <= nEndPort && nNum > getFreePortNum; nPort++)
    {
        if(!ToolFun::UseCheckPort(nPort) && !ToolFun::Contains(vUsePort, nPort))
        {
            cout << "port:" << nPort << endl;
            pUsePort->push_back(nPort);
            getFreePortNum++;
        }
    }

    if(pUsePort->size() != nNum) 
    {
        nRet = WARN_INSUFFICIENT_PORTS;
        LOG_WARN(nRet, getWarnErrorMsg(nRet));
        goto FINISH;
    }

FINISH:
    if(pUsePortData) json_decref(pUsePortData);
    if(pValue) json_decref(pValue);
    return nRet;
}

int PostPortForwardFromDB(string szName,string szIntranetAddress,string szExternalNetworkAddress,int nStatus,int nUserId,string szExternalProgramAdPort, string szCmd, int &nId)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    string szSql = "INSERT INTO port_map (name, intranet_address,external_network_address,status,u_id,external_program_ad_port,cmd) VALUES";
    szSql += "('" + szName + "', '" + szIntranetAddress + "', '" + szExternalNetworkAddress + "', " + to_string(nStatus) + "," + to_string(nUserId) + ", '" + szExternalProgramAdPort + "', '" + szCmd + "');";
    LOG_INFO(SUCCESS, "exec sql:" + szSql);

    nRet = pObDb->insertData(szSql,nId); 
    delete pObDb;
    return nRet;
}


int GetPortForwardCmd(int nPortId, string &szCmd)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    vector<vector<string>> *selectRes = new vector<vector<string>>;

    string szSql = "select cmd from port_map where port_id = " + to_string(nPortId);
    LOG_INFO(SUCCESS, "exec sql:" + szSql);

    //nRet = pObDb->selectData(szSql); 
    // 执行sql语句
    if (nRet = pObDb->selectData(szSql, selectRes)) 
    {
        LOG_ERROR(nRet, "sql exec failure:" + szSql);
        delete selectRes;
        goto FINISH;
    }

    if (0 < selectRes->size() || 0 < selectRes->at(0).size())
        szCmd = selectRes->at(0)[0];
    else 
        szCmd = "";

FINISH:
    delete selectRes;
    delete pObDb;
    return nRet;
}

int UpdatePortForwardStatusFromDB(const int nPortId, const int nStatus)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    string szSql = "UPDATE port_map SET status = " + to_string(nStatus) + " WHERE port_id = " + to_string(nPortId);
    LOG_INFO(SUCCESS, "exec sql:" + szSql);

    nRet = pObDb->updateData(szSql); 
    delete pObDb;
    return nRet;
}

int UpdatePortForwardFromDB(int nId, string* szExternalNetworkAddress,string* szExternalProgramAdPort, string* szCmd, string* szIntranetAddress, string* szName)
{
    if(!szExternalNetworkAddress && !szExternalProgramAdPort && !szCmd && !szIntranetAddress && !szName)
        return SUCCESS;

    vector<string> vStr;
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    string szSql = "";

    vStr.push_back("status  = 0 ");
    if(szExternalNetworkAddress) vStr.push_back("external_network_address  = '" + *szExternalNetworkAddress + "'");
    if(szExternalProgramAdPort) vStr.push_back("external_program_ad_port = '" + *szExternalProgramAdPort + "'");
    if(szCmd) vStr.push_back("cmd = '" + *szCmd + "'");
    if(szIntranetAddress) vStr.push_back("intranet_address  = '" + *szIntranetAddress + "'");
    if(szName) vStr.push_back("name  = '" + *szName + "'");

    szSql = "UPDATE port_map SET " + ToolFun::joint(vStr) + " WHERE port_id = " + to_string(nId);

    LOG_INFO(SUCCESS, "exec sql:" + szSql);

    nRet = pObDb->updateData(szSql); 
    delete pObDb;
    return nRet;
}


int GetPortForwardFromDB(int nUserId, json_t* &pJsonArray)
{
    if(!pJsonArray || !json_is_array(pJsonArray)) return ERR_PTR_IS_NULL;

    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    vector<vector<string>> *selectRes = new vector<vector<string>>;
    string szSql = "SELECT port_id,name,intranet_address,UNIX_TIMESTAMP(created_time) as created_time,";
    szSql +=  "external_network_address,external_program_ad_port,UNIX_TIMESTAMP(end_time) as end_time ,money,bill_id,status";
    szSql +=  " FROM port_map WHERE u_id = '" + to_string(nUserId) + "'";

    LOG_INFO(SUCCESS, "exec sql:" + szSql);

    nRet = pObDb->selectData(szSql, selectRes); 
    for (int i = 0; i < selectRes->size(); i++)
    {
        if(10 != selectRes->at(i).size())
        {
            nRet = ERR_UNKNOWN;
            break;
        }
        json_t* pTmpJson = NULL;
        pTmpJson = json_object();
        json_object_set_new(pTmpJson, "Id", json_integer(ToolFun::stringToInt(selectRes->at(i)[0])));
        json_object_set_new(pTmpJson, "Name", json_string(selectRes->at(i)[1].c_str()));
        json_object_set_new(pTmpJson, "IntranetAddress", json_string(selectRes->at(i)[2].c_str()));
        json_object_set_new(pTmpJson, "CreateTime", json_integer(ToolFun::stringToInt(selectRes->at(i)[3])));
        //,, ,,,
        json_object_set_new(pTmpJson, "ExternalNetworkAddress", json_string(selectRes->at(i)[4].c_str()));
        json_object_set_new(pTmpJson, "ExternalProgramAdPort", json_string(selectRes->at(i)[5].c_str()));
        json_object_set_new(pTmpJson, "EndTime", json_integer(ToolFun::stringToInt(selectRes->at(i)[6])));
        json_object_set_new(pTmpJson, "Money", json_integer(ToolFun::stringToInt(selectRes->at(i)[7])));
        json_object_set_new(pTmpJson, "BillId", json_integer(ToolFun::stringToInt(selectRes->at(i)[8])));
        json_object_set_new(pTmpJson, "Status", json_integer(ToolFun::stringToInt(selectRes->at(i)[9])));
        json_array_append_new(pJsonArray, pTmpJson);
    }

    delete pObDb;
    delete selectRes;
    return nRet;
}

int CheckSignInMsgFromDB(string szEmail, string szPhone)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    vector<vector<string>> *selectRes = new vector<vector<string>>;
    if("" != szPhone)
    {
        string szSql1 = "SELECT id FROM user_info WHERE phone_number = '" + szPhone + "' LIMIT 1";
        LOG_INFO(SUCCESS, "exec sql:" + szSql1);
        nRet = pObDb->selectData(szSql1, selectRes); 
    }

    if("" != szEmail)
    {
        string szSql2 = "SELECT id FROM user_info WHERE email = '" + szEmail + "' LIMIT 1";
        LOG_INFO(SUCCESS, "exec sql:" + szSql2);
        nRet = pObDb->selectData(szSql2, selectRes); 
    }

    if (selectRes->size() > 0) nRet = WARN_USER_REGISTERED;

    delete selectRes;
    delete pObDb;
    return nRet;
}
int SignInFromDB(string szEmail, string szPhone, string szPasswd,int& nId)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    char szSql[256] = {0};
    if("" != szEmail && "" != szPasswd)
    {
        sprintf(szSql, "INSERT INTO user_info (email, phone_number,passwd) VALUES('%s', '%s','%s')",szEmail.c_str(), szPhone.c_str(), szPasswd.c_str());
    }
    else if("" != szEmail)
    {
        sprintf(szSql, "INSERT INTO user_info (email,passwd) VALUES('%s', '%s')",szEmail.c_str(), szPasswd.c_str());
    }
    else if("" != szPhone)
    {
        sprintf(szSql, "INSERT INTO user_info (phone_number,passwd) VALUES('%s', '%s')",szPhone.c_str(), szPasswd.c_str());
    }
    else
    {
        nRet = ERR_UNKNOWN;
        goto FINISH;
    }

    nRet = pObDb->insertData(string(szSql), nId); 
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql));

FINISH:
    delete pObDb;
    return nRet;
}

int LoginFromDB(string szEmail,string szPhone,string  szPasswd,int& nId)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    vector<vector<string>> *selectRes = new vector<vector<string>>;
    char szSql[256] ={0};

    if("" != szEmail)
    {
        sprintf(szSql, "SELECT id FROM user_info WHERE email = '%s' and passwd = '%s'",szEmail.c_str(), szPasswd.c_str());
    }
    else if("" != szPhone)
    {
        sprintf(szSql, "SELECT id FROM user_info WHERE phone_number = '%s' and passwd = '%s'",szPhone.c_str(), szPasswd.c_str());
    }
    else
    {
        nRet = ERR_UNKNOWN;
        goto FINISH;
    }

    pObDb->selectData(string(szSql), selectRes);
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql));

    if (1 != selectRes->size() || 1 != selectRes->at(0).size())
    {
        nRet = WARN_ACCOUNT_OR_PASSWORD;
        goto FINISH;
    }
    nId = ToolFun::stringToInt(selectRes->at(0)[0]);

FINISH:
    delete selectRes;
    delete pObDb;
    return nRet;
}

int CheckCardSecretFromDB(string szCardSecret, double& dMoney)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    vector<vector<string>> *selectRes = new vector<vector<string>>;

    string szSql = "SELECT status,money FROM card_secret WHERE card_secret = '" + szCardSecret + "'";
    LOG_INFO(SUCCESS, "exec sql:" + szSql);
    nRet = pObDb->selectData(szSql, selectRes); 

    if (1 != selectRes->size() || 2 != selectRes->at(0).size())
    {
        nRet = WARN_CARD_SECRET;
        goto FINISH;
    }
    if(ToolFun::stringToInt(selectRes->at(0)[0]))
    {
        nRet = WARN_CARD_SECRET_FAILURE;
        goto FINISH;
    }

    dMoney = ToolFun::stringTodouble(selectRes->at(0)[1]);

FINISH:
    delete selectRes;
    delete pObDb;
    return nRet;
}

int RechargeWithCardCodeFromDB(string szUId, string szCardSecret, double dMoney)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    char szSql1[256] = {0};
    char szSql2[256] = {0};
    char szSql3[256] = {0};

    sprintf(szSql1, "UPDATE card_secret SET status = 1 WHERE card_secret = '%s'", szCardSecret.c_str());
    sprintf(szSql2, "UPDATE user_info SET balance = balance + %f WHERE id = '%s'", dMoney, szUId.c_str());
    sprintf(szSql3, "INSERT INTO user_bill (bill_type,money,recharge_card_key,u_id) VALUES(0,'%f', '%s', '%s')", dMoney, szCardSecret.c_str(),szUId.c_str());
    pObDb->BeginTransaction();
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql1));
    if(nRet = pObDb->updateData(string(szSql1)))
    {
        pObDb->RollbackTransaction();
        goto FINISH;
    }  
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql2));
    if(nRet = pObDb->updateData(string(szSql2)))
    {
        pObDb->RollbackTransaction();
        goto FINISH;
    } 
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql3));
    if(nRet = pObDb->updateData(string(szSql3)))
    {
        pObDb->RollbackTransaction();
        goto FINISH;
    } 
    pObDb->CommitTransaction();
FINISH:
    delete pObDb;
    return nRet;
}

int GetUserBalanceFromDB(string szUId, double& dBalance)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    vector<vector<string>> *selectRes = new vector<vector<string>>;

    string szSql = "SELECT balance FROM user_info WHERE id = '" + szUId + "'";
    LOG_INFO(SUCCESS, "exec sql:" + szSql);
    nRet = pObDb->selectData(szSql, selectRes); 

    if (1 != selectRes->size() || 1 != selectRes->at(0).size())
    {
        nRet = ERR_UNKNOWN;
        goto FINISH;
    }

    dBalance = ToolFun::stringToInt(selectRes->at(0)[0]);

FINISH:
    delete selectRes;
    delete pObDb;
    return nRet;
}

int BuyResFromDB(string szUId, vector<vector<double>> vvResMsg, double dMoney, vector<int>& vId)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    char szSql1[256] = {0};
    char szSql2[256] = {0};
    char szSql3[256] = {0};
    int nBillId = -1;
//FROM_UNIXTIME(UNIX_TIMESTAMP(NOW()) + %d)

    pObDb->BeginTransaction();
    sprintf(szSql1, "INSERT INTO user_bill (bill_type,money,u_id) VALUES(1,'%f', '%s')", dMoney, szUId.c_str());
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql1));
    if(nRet = pObDb->insertData(string(szSql1), nBillId))
    {
        pObDb->RollbackTransaction();
        goto FINISH;
    }  

    for (int i = 0; i < vvResMsg.size(); i++) 
    {
        int nLeaseTime = vvResMsg[i][1];
        double dTmpMoney = vvResMsg[i][0];
        int nId = -1;
        //vvResMsg[i]
        sprintf(szSql2, "INSERT INTO port_map (end_time,u_id,bill_id,money) VALUES(FROM_UNIXTIME(UNIX_TIMESTAMP(NOW()) + %d),'%s',%d,%f)", nLeaseTime, szUId.c_str(),nBillId, dTmpMoney);
        LOG_INFO(SUCCESS, "exec sql:" + string(szSql2));
        if(nRet = pObDb->insertData(string(szSql2), nId))
        {
            pObDb->RollbackTransaction();
            goto FINISH;
        } 
        vId.push_back(nId);
    }

    sprintf(szSql3, "UPDATE user_info SET balance = balance - %f WHERE id = '%s'", dMoney, szUId.c_str());
    if(nRet = pObDb->updateData(string(szSql3)))
    {
        pObDb->RollbackTransaction();
        goto FINISH;
    }  

    pObDb->CommitTransaction();
FINISH:
    delete pObDb;
    return nRet;
}

int GetResStatusFromDB(string szUId,int & ntotal,int & nUse)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    char szSql1[256] = {0};
    char szSql2[256] = {0};
    vector<vector<string>> *selectRes1 = new vector<vector<string>>;
    vector<vector<string>> *selectRes2 = new vector<vector<string>>;

//FROM_UNIXTIME(UNIX_TIMESTAMP(NOW()) + %d)
    sprintf(szSql1, "SELECT COUNT(*) FROM port_map WHERE u_id='%s' and status = 0;", szUId.c_str());
    sprintf(szSql2, "SELECT COUNT(*) FROM port_map WHERE u_id='%s' ", szUId.c_str());


    LOG_INFO(SUCCESS, "exec sql:" + string(szSql1));
    if(nRet = pObDb->selectData(string(szSql1),selectRes1))
    {
        nRet = ERR_UNKNOWN;
        goto FINISH;
    }  
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql2));
    if(nRet = pObDb->selectData(string(szSql2),selectRes2))
    {
        nRet = ERR_UNKNOWN;
        goto FINISH;
    }  

    if(1 != selectRes1->size() || 1 != selectRes1[0].size()
    ||1 != selectRes2->size() || 1 != selectRes2[0].size())
    {
        nRet = ERR_UNKNOWN;
        goto FINISH;
    }

    nUse = ToolFun::stringToInt(selectRes1->at(0)[0]);
    ntotal = ToolFun::stringToInt(selectRes2->at(0)[0]);
    

FINISH:
    delete pObDb;
    return nRet;
}

/*
[
    {
        "Bill":{
            "Id":213455,
            "Money":123,
            "CreatedTime":1234567,
            "Bill_type":0
        },
        "Res":[
            {
                "Money":123,
                "EndTime":1234567
            },
            {
                "Money":123,
                "EndTime":1234567
            }
        ]
    },
    {
        "Bill":{
            "Id":213455,
            "Money":123,
            "CreatedTime":1234567,
            "Bill_type":0
        },
        "Res":[
            {
                "Money":123,
                "EndTime":1234567
            },
            {
                "Money":123,
                "EndTime":1234567
            }
        ]
    }
]
*/
int GetBillFromDB(string szUId, json_t* pOutJson)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    char szSql1[256] = {0};
    char szSql2[256] = {0};
    vector<vector<string>> *selectRes1 = new vector<vector<string>>;
    vector<vector<string>> *selectRes2 = new vector<vector<string>>;
    int nBill = -1;
    int nMoney = -1;
    int nCreatedTime = -1;
    int nBillType = -1;
//FROM_UNIXTIME(UNIX_TIMESTAMP(NOW()) + %d)
    sprintf(szSql1, "SELECT bill_id, money, UNIX_TIMESTAMP(created_time) as created_time, bill_type FROM user_bill WHERE u_id='%s'", szUId.c_str());
    //sprintf(szSql2, "SELECT COUNT(*) FROM port_map WHERE u_id='%s' ", szUId.c_str());


    LOG_INFO(SUCCESS, "exec sql:" + string(szSql1));
    if(nRet = pObDb->selectData(string(szSql1),selectRes1))
    {
        nRet = ERR_UNKNOWN;
        goto FINISH;
    }  
    
    for (int i = 0; i < selectRes1->size(); i++)
    {
        int nBillId = -1;
        int nMoney = -1;
        int nCreatedTime = -1;
        int nBillType = -1;
        json_t* pTmp = NULL;// json_object();
        json_t* pTmpBill = NULL;// json_object();
        json_t* pTmpResArray = NULL;// json_array();
        if(4 != selectRes1->at(i).size())
        {
            return ERR_UNKNOWN;
            goto FINISH;
        }
        nBillId = ToolFun::stringToInt( selectRes1->at(i)[0]);
        nMoney = ToolFun::stringToInt( selectRes1->at(i)[1]);
        nCreatedTime = ToolFun::stringToInt( selectRes1->at(i)[2]);
        nBillType = ToolFun::stringToInt( selectRes1->at(i)[3]);
        if(0 == nBillType)
        {
            pTmpBill = json_object();
            json_object_set_new(pTmpBill, "BillId", json_integer(nBillId));
            json_object_set_new(pTmpBill, "Money", json_integer(nMoney));
            json_object_set_new(pTmpBill, "CreatedTime", json_integer(nCreatedTime));
            json_object_set_new(pTmpBill, "BillType", json_integer(nBillType));
            pTmp = json_object();
            pTmpResArray = json_array();
            json_object_set_new(pTmp, "Bill", pTmpBill);
            json_object_set_new(pTmp, "Res", pTmpResArray);
            json_array_append_new(pOutJson,pTmp);
        }
        if(1 == nBillType)
        {
            pTmpBill = json_object();
            json_object_set_new(pTmpBill, "BillId", json_integer(nBillId));
            json_object_set_new(pTmpBill, "Money", json_integer(nMoney));
            json_object_set_new(pTmpBill, "CreatedTime", json_integer(nCreatedTime));
            json_object_set_new(pTmpBill, "BillType", json_integer(nBillType));
            pTmp = json_object();
            pTmpResArray = json_array();
            json_object_set_new(pTmp, "Bill", pTmpBill);
            //处理Res
            memset(szSql2, 0, sizeof(szSql2));
            selectRes2->clear();
            sprintf(szSql2, "SELECT money, UNIX_TIMESTAMP(end_time) as end_time FROM port_map WHERE bill_id='%d' ", nBillId);
            LOG_INFO(SUCCESS, "exec sql:" + string(szSql2));
            if(nRet = pObDb->selectData(string(szSql2),selectRes2))
            {
                json_decref(pTmp);
                json_decref(pTmpResArray);
                nRet = ERR_UNKNOWN;
                goto FINISH;
            }  
            for (int i = 0; i < selectRes2->size(); i++)
            {
                json_t* pTmpRes = NULL;
                int nResMoney = -1;
                int nEndTime = -1;
                if(2 != selectRes2->at(i).size())
                {
                    json_decref(pTmp);
                    json_decref(pTmpResArray);
                    return ERR_UNKNOWN;
                    goto FINISH;
                }
                nResMoney = ToolFun::stringToInt( selectRes2->at(i)[0]);
                nEndTime = ToolFun::stringToInt( selectRes2->at(i)[1]);
                pTmpRes = json_object();
                json_object_set_new(pTmpRes, "Money", json_integer(nResMoney));
                json_object_set_new(pTmpRes, "EndTime", json_integer(nEndTime));

                json_array_append_new(pTmpResArray, pTmpRes);
            }


            json_object_set_new(pTmp, "Res", pTmpResArray);
            json_array_append_new(pOutJson,pTmp);
        }
    }
    

    

FINISH:
    delete selectRes1;
    delete selectRes2;
    delete pObDb;
    return nRet;
}

int UpdateUserPasswdFromDB(string szUserId,string  szOldPasswd, string  szNewPasswd)
{
    int nRet = SUCCESS;
    ObDb* pObDb = new ObDb();
    vector<vector<string>> *selectRes = new vector<vector<string>>;
    char szSql1[256] ={0};
    char szSql2[256] ={0};

    sprintf(szSql1, "SELECT id FROM user_info WHERE id = '%s' and passwd = '%s'",szUserId.c_str(), szOldPasswd.c_str());

    if(nRet = pObDb->selectData(string(szSql1), selectRes)) goto FINISH;
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql1));

    if (1 != selectRes->size() || 1 != selectRes->at(0).size())
    {
        nRet = WARN_PASSWORD;
        goto FINISH;
    }
    
    sprintf(szSql2, "UPDATE user_info SET passwd = '%s' WHERE id = '%s'",szNewPasswd.c_str(), szUserId.c_str());

    if(nRet = pObDb->updateData(string(szSql2))) goto FINISH;
    LOG_INFO(SUCCESS, "exec sql:" + string(szSql2));

FINISH:
    delete selectRes;
    delete pObDb;
    return nRet;
}
/****************************************************************************************************************
*                                            ____              _                                                *
*                                            | __ )  ___  __ _(_)_ __                                           *
*                                            |  _ \ / _ \/ _` | | '_ \                                          *
*                                            | |_) |  __/ (_| | | | | |                                         *
*                                            |____/ \___|\__, |_|_| |_|                                         *
*                                                        |___/                                                  *   
*****************************************************************************************************************/
int Test(const json_t* pInputJson, json_t** ppOutputJson)
{
    json_t* pOutputJson = json_object();

    std::cout << "this is ->  int Test(const json_t* pInputJson, json_t** ppOutputJson)\n";

    json_object_set_new(pOutputJson, "data", json_integer(1));
    
    RsJson::CreateGenRsPJson(ppOutputJson, 0, pOutputJson);

    return 0;
}

int PostPortForward(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入PostPortForward\n"; 
    int nRet = SUCCESS;
    json_t* pOutJson = NULL;
    std::vector<int>* pUsePort = new std::vector<int>(); 
    FILE* pf = NULL;
    string szCmd;
    char buf[4096] = {0};
    int nUserId = 0;
    char szName[64] = {0};
    char szIntranetAddress[24] = {0};
    int nId = -1;
    string szNohupCmd = "";

    string szExternalNetworkAddress = "";
    string szExternalProgramAdPort = "";

    ToolFun::PrintJson(pInputJson);

    if(!ToolFun::GetIntFromJson(json_object_get(pInputJson, "UserId"), nUserId)
    ||!ToolFun::GetStrFromJson(json_object_get(pInputJson, "IntranetAddress"), szIntranetAddress, sizeof(szIntranetAddress))
    ||!ToolFun::GetStrFromJson(json_object_get(pInputJson, "Name"), szName, sizeof(szName)))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    if(nRet = GetFreePort(2, Config::getInstance().getPortStart(), Config::getInstance().getPortEnd(), pUsePort))
        goto FINISH;

    szCmd = "lcx -listen " + std::to_string(pUsePort->at(0)) + " " + std::to_string(pUsePort->at(1));
    szNohupCmd = ToolFun::GetNohupCmd("/port_forward/code/c++/res/" + szCmd);
    LOG_INFO(SUCCESS,"exce:" + szNohupCmd);

    if (ToolFun::ExecStateOfSystem(::system(szNohupCmd.c_str()), 0))
    {
        nRet = ERR_EXEC_FAIL;
        goto FINISH;
    }

    szExternalNetworkAddress = string(CONFIG.getServerIP()+":" + to_string(pUsePort->at(1)));
    szExternalProgramAdPort = string(CONFIG.getServerIP()+":" + to_string(pUsePort->at(0)));


    //写入数据库
    PostPortForwardFromDB(string(szName), string(szIntranetAddress), szExternalNetworkAddress, 0,nUserId, szExternalProgramAdPort, szCmd, nId);

    pOutJson = json_object();
    json_object_set_new(pOutJson, "NetworkAdPort", json_string(szExternalNetworkAddress.c_str()));
    json_object_set_new(pOutJson, "ProgramAdPort", json_string(szExternalProgramAdPort.c_str()));
    json_object_set_new(pOutJson, "Id", json_integer(nId));
    

    ToolFun::PrintJson(pOutJson);

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    delete pUsePort;
    return nRet;
}

int ClosePortForward(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入DelPortForward\n"; 
    int nRet = SUCCESS;
    int nId = -1;
    string szCmd;

    ToolFun::PrintJson(pInputJson);

    if(!ToolFun::GetIntFromJson(json_object_get(pInputJson, "Id"), nId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    //终止进程
    //查询命令
    GetPortForwardCmd(nId,szCmd);
    //kill
    if("" != szCmd) ToolFun::killProcessUseCmd(szCmd);

    //更新DB中的状态
    UpdatePortForwardStatusFromDB(nId, 1);

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);
    return nRet;
}

int RestartPortForward(const json_t* pInputJson, json_t** ppOutputJson)
{
    int nRet = SUCCESS;
    int nId = -1;
    string szCmd;
    std::vector<int>* pUsePort = new std::vector<int>(); 
    string szExternalNetworkAddress = "";
    string szExternalProgramAdPort = "";
    string szNohupCmd = "";
    json_t* pOutJson = NULL;


    if(!ToolFun::GetIntFromJson(json_object_get(pInputJson, "Id"), nId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    //终止进程
    //查询命令
    GetPortForwardCmd(nId,szCmd);
    //kill
    if("" != szCmd) ToolFun::killProcessUseCmd(szCmd);

    //重启映射
    //在DB中更新映射信息
    if(nRet = GetFreePort(2, Config::getInstance().getPortStart(), Config::getInstance().getPortEnd(), pUsePort))
        goto FINISH;

    szCmd = "lcx -listen " + std::to_string(pUsePort->at(0)) + " " + std::to_string(pUsePort->at(1));
    szNohupCmd = ToolFun::GetNohupCmd("/port_forward/code/c++/res/" + szCmd);
    LOG_INFO(SUCCESS,"exce:" + szNohupCmd);

    if (ToolFun::ExecStateOfSystem(::system(szNohupCmd.c_str()), 0))
    {
        nRet = ERR_EXEC_FAIL;
        goto FINISH;
    }

    szExternalNetworkAddress = string(CONFIG.getServerIP()+":" + to_string(pUsePort->at(1)));
    szExternalProgramAdPort = string(CONFIG.getServerIP()+":" + to_string(pUsePort->at(0)));

    //写入数据库
    UpdatePortForwardFromDB(nId, &szExternalNetworkAddress, &szExternalProgramAdPort, &szCmd, NULL, NULL);

    pOutJson = json_object();
    json_object_set_new(pOutJson, "NetworkAdPort", json_string(szExternalNetworkAddress.c_str()));
    json_object_set_new(pOutJson, "ProgramAdPort", json_string(szExternalProgramAdPort.c_str()));
    json_object_set_new(pOutJson, "Id", json_integer(nId));


FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    delete pUsePort;
    return nRet;
}

int UpdatePortForward(const json_t* pInputJson, json_t** ppOutputJson)
{
    int nRet = SUCCESS;
    int nId = -1;
    string szName = "";
    string szIntranetAddress = "";

    ToolFun::GetStrFromJson(json_object_get(pInputJson, "Name"), szName);
    ToolFun::GetStrFromJson(json_object_get(pInputJson, "IntranetAddress"), szIntranetAddress);

    if("" == szName && "" == szIntranetAddress)
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }
    if(!ToolFun::GetIntFromJson(json_object_get(pInputJson, "Id"), nId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    //写入数据库
    UpdatePortForwardFromDB(nId, NULL, NULL, NULL, &szIntranetAddress, &szName);

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    return nRet;
}

int GetPortForward(const json_t* pInputJson, json_t** ppOutputJson)
{
    int nRet = SUCCESS;
    int nId = -1;
    json_t* pOutJson = NULL;

    ToolFun::PrintJson(pInputJson);

    if(!ToolFun::GetIntFromJson(json_object_get(pInputJson, "UserId"), nId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }
    pOutJson = json_array();
    nRet = GetPortForwardFromDB(nId, pOutJson);

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}

int SignIn(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入SignIn\n"; 
    int nRet = SUCCESS;
    string szEmail = "";
    string szPhone = "";
    string szPasswd = "";
    int nId = -1;
    json_t* pOutJson = NULL;

    ToolFun::GetStrFromJson(json_object_get(pInputJson, "Email"), szEmail);
    ToolFun::GetStrFromJson(json_object_get(pInputJson, "Phone"),szPhone);

    if("" == szEmail && "" == szPhone)
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }
    if(!ToolFun::GetStrFromJson(json_object_get(pInputJson, "Passwd"),szPasswd))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    if(nRet = CheckSignInMsgFromDB(szEmail, szPhone)) goto FINISH;

    if(nRet =SignInFromDB(szEmail, szPhone, szPasswd, nId)) goto FINISH;

    pOutJson = json_object();
    json_object_set_new(pOutJson, "Id", json_integer(nId));

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}

int Login(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入Login\n"; 
    int nRet = SUCCESS;
    string szEmail = "";
    string szPhone = "";
    string szPasswd = "";
    int nId = -1;
    json_t* pOutJson = NULL;

    ToolFun::GetStrFromJson(json_object_get(pInputJson, "Email"), szEmail);
    ToolFun::GetStrFromJson(json_object_get(pInputJson, "Phone"),szPhone);

    if("" == szEmail && "" == szPhone)
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }
    if(!ToolFun::GetStrFromJson(json_object_get(pInputJson, "Passwd"),szPasswd))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    if(nRet = LoginFromDB(szEmail, szPhone, szPasswd, nId)) goto FINISH;

    pOutJson = json_object();
    json_object_set_new(pOutJson, "Id", json_integer(nId));

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}

int RechargeWithCardCode(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入RechargeWithCardCode\n"; 
    int nRet = SUCCESS;
    string szCardSecret = "";
    string szUId = "";
    json_t* pOutJson = NULL;
    double dBalance = 0;
    double dCardSecretMoney = 0;

    if(!ToolFun::GetStrFromJson(json_object_get(pInputJson, "CardSecret"), szCardSecret)
    ||!ToolFun::GetStrFromJson(json_object_get(pInputJson, "UserId"), szUId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }
    if(nRet = CheckCardSecretFromDB(szCardSecret, dCardSecretMoney)) goto FINISH;
    if(nRet = RechargeWithCardCodeFromDB(szUId, szCardSecret, dCardSecretMoney)) goto FINISH;
    if(nRet = GetUserBalanceFromDB(szUId, dBalance)) goto FINISH;
    pOutJson = json_object();
    json_object_set_new(pOutJson, "Balance", json_real(dBalance));
    json_object_set_new(pOutJson, "Money", json_real(dCardSecretMoney));

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}

int BuyRes(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入BuyRes\n"; 
    int nRet = SUCCESS;
    string szUId = "";
    json_t* pOutJson = NULL;
    json_t* pOutJsonArray = NULL;
    json_t* pInJsonArray = NULL;
    vector<vector<double>> vvResMsg;
    double dMoney = 0;
    vector<int> vId;
    double dBalance = 0;

    pInJsonArray = json_object_get(pInputJson, "Port");
    if(!pInJsonArray 
    || !json_is_array(pInJsonArray)
    ||!ToolFun::GetStrFromJson(json_object_get(pInputJson, "UserId"), szUId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    for (int i = 0; i < json_array_size(pInJsonArray); i++) {
        json_t *element = json_array_get(pInJsonArray, i);
        int nLeaseTime = 0;
        vector<double> vTmp;

        ToolFun::GetIntFromJson(json_object_get(element, "LeaseTime"), nLeaseTime);
        ToolFun::PrintJson(element);
        vTmp.push_back(CONFIG.getPortUnitPrice());
        vTmp.push_back(nLeaseTime);
        vvResMsg.push_back(vTmp);
    }

    //计算订单金额
    dMoney = vvResMsg.size() * CONFIG.getPortUnitPrice();
    if(nRet = BuyResFromDB(szUId, vvResMsg, dMoney, vId)) goto FINISH;
    
    pOutJsonArray = json_array();
    for (const auto& value : vId) 
    {
        json_array_append_new(pOutJsonArray, json_integer(value));
    }

    if(nRet = GetUserBalanceFromDB(szUId, dBalance)) goto FINISH;

    pOutJson = json_object();
    json_object_set_new(pOutJson, "ResId", pOutJsonArray);
    json_object_set_new(pOutJson, "Balance", json_real(dBalance));

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}

int GetUserBalance(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入GetUserBalance\n"; 
    int nRet = SUCCESS;
    string szUId = "";
    json_t* pOutJson = NULL;
    double dBalance = 0;

    if(!ToolFun::GetStrFromJson(json_object_get(pInputJson, "UserId"), szUId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    if(nRet = GetUserBalanceFromDB(szUId, dBalance)) goto FINISH;

    pOutJson = json_object();
    json_object_set_new(pOutJson, "Balance", json_real(dBalance));

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}

int GetResUnitPrice(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入GetResUnitPrice\n"; 
    int nRet = SUCCESS;
    json_t* pOutJson = NULL;

    pOutJson = json_object();
    json_object_set_new(pOutJson, "PortUnitPrice", json_real(CONFIG.getPortUnitPrice()));

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}

int GetResStatus(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入GetResUnitPrice\n"; 
    int nRet = SUCCESS;
    json_t* pOutJson = NULL;
    string szUId = "";
    int ntotal = 0;
    int nUse = 0;
    
    if(!ToolFun::GetStrFromJson(json_object_get(pInputJson, "UserId"), szUId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }
    if(nRet = GetResStatusFromDB(szUId, ntotal, nUse)) goto FINISH;
    pOutJson = json_object();
    json_object_set_new(pOutJson, "Total", json_integer(ntotal));
    json_object_set_new(pOutJson, "Use", json_integer(nUse));

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}

int GetBill(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入GetBill\n"; 
    int nRet = SUCCESS;
    json_t* pOutJson = NULL;
    string szUId = "";
    int ntotal = 0;
    int nUse = 0;
    
    if(!ToolFun::GetStrFromJson(json_object_get(pInputJson, "UserId"), szUId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }
    
    pOutJson = json_array();
    if(nRet = GetBillFromDB(szUId, pOutJson)) goto FINISH;

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}


int UpdateUserPasswd(const json_t* pInputJson, json_t** ppOutputJson)
{
    cout << "进入UpdateUserPasswd\n"; 
    int nRet = SUCCESS;
    json_t* pOutJson = NULL;
    string szUId = "";
    string szNewPasswd = "";
    string szOldPasswd = "";
    
    
    if(!ToolFun::GetStrFromJson(json_object_get(pInputJson, "UserId"), szUId)
    ||!ToolFun::GetStrFromJson(json_object_get(pInputJson, "NewPasswd"), szNewPasswd)
    ||!ToolFun::GetStrFromJson(json_object_get(pInputJson, "OldPasswd"), szOldPasswd))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }
    
    pOutJson = json_array();
    if(nRet = UpdateUserPasswdFromDB(szUId, szOldPasswd, szNewPasswd)) goto FINISH;

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}




int BuyRes11(const json_t* pInputJson, json_t** ppOutputJson)
{
    int nRet = SUCCESS;
    string szUId = "";
    json_t* pOutJson = NULL;
    json_t* pOutJsonArray = NULL;
    json_t* pInJsonArray = NULL;
    vector<vector<double>> vvResMsg;
    vector<int> vId;
    double dMoney = 0,dBalance = 0;

    pInJsonArray = json_object_get(pInputJson, "Port");
    if(!pInJsonArray 
    || !json_is_array(pInJsonArray)
    ||!ToolFun::GetStrFromJson(json_object_get(pInputJson, "UserId"), szUId))
    {
        nRet = WARN_JSON_FORMAT;
        goto FINISH;
    }

    for (int i = 0; i < json_array_size(pInJsonArray); i++) {
        json_t *element = json_array_get(pInJsonArray, i);
        int nLeaseTime = 0;
        vector<double> vTmp;

        ToolFun::GetIntFromJson(json_object_get(element, "LeaseTime"), nLeaseTime);
        ToolFun::PrintJson(element);
        vTmp.push_back(CONFIG.getPortUnitPrice());
        vTmp.push_back(nLeaseTime);
        vvResMsg.push_back(vTmp);
    }

    //计算订单金额
    dMoney = vvResMsg.size() * CONFIG.getPortUnitPrice();
    if(nRet = BuyResFromDB(szUId, vvResMsg, dMoney, vId)) goto FINISH;
    
    pOutJsonArray = json_array();
    for (const auto& value : vId) 
    {
        json_array_append_new(pOutJsonArray, json_integer(value));
    }

    if(nRet = GetUserBalanceFromDB(szUId, dBalance)) goto FINISH;

    pOutJson = json_object();
    json_object_set_new(pOutJson, "ResId", pOutJsonArray);
    json_object_set_new(pOutJson, "Balance", json_real(dBalance));

FINISH:
    if(SUCCESS == nRet) RsJson::CreateGenRsPJson(ppOutputJson, nRet, pOutJson);
    else RsJson::CreateGenRsPJson(ppOutputJson, nRet);

    if(pOutJson) json_decref(pOutJson);
    return nRet;
}