#ifndef UIFUNCSYS_H
#define UIFUNCSYS_H

#include "jansson.h"
#include "err.h"

//测试接口
int Test(const json_t* pInputJson, json_t** ppOutputJson);
//开启一个端口映射
int PostPortForward(const json_t* pInputJson, json_t** ppOutputJson);
//删除一个端口映射
int ClosePortForward(const json_t* pInputJson, json_t** ppOutputJson);
//重启对应端口映射
int RestartPortForward(const json_t* pInputJson, json_t** ppOutputJson);
//更新对应端口映射信息
int UpdatePortForward(const json_t* pInputJson, json_t** ppOutputJson);
//查询某用户的映射配置
int GetPortForward(const json_t* pInputJson, json_t** ppOutputJson);
//注册
int SignIn(const json_t* pInputJson, json_t** ppOutputJson);
//登录
int Login(const json_t* pInputJson, json_t** ppOutputJson);
//卡密充值
int RechargeWithCardCode(const json_t* pInputJson, json_t** ppOutputJson);
//购买资源
int BuyRes(const json_t* pInputJson, json_t** ppOutputJson);
//查询用户余额
int GetUserBalance(const json_t* pInputJson, json_t** ppOutputJson);
//查询资源单价
int GetResUnitPrice(const json_t* pInputJson, json_t** ppOutputJson);
//查询资源状态
int GetResStatus(const json_t* pInputJson, json_t** ppOutputJson);
//获取账单
int GetBill(const json_t* pInputJson, json_t** ppOutputJson);
//更新用户密码
int UpdateUserPasswd(const json_t* pInputJson, json_t** ppOutputJson);
#endif