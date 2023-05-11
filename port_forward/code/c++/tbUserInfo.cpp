#include "tbUserInfo.h"

using namespace std;
// select * from user_info where id>=10000005  order by id asc limit 2; //升序

TbUserInfo::TbUserInfo() {
    Table::name = "user_info";
    this->head = new ptrList();
    this->last = head;
}
TbUserInfo::~TbUserInfo() { delete this->head; }

int TbUserInfo::selectAll(void *&res, int &num, int beginId, const bool &gt) {
    int resCode = SUCCESS;
    string symbol = gt == true ? ">=" : "<=";
    // 拼接sql语句
    string sqlStr = "select id,phone_number,email,passwd,balance from " + TbUserInfo::Table::name +
                    " where id" + symbol + to_string(num) + "  order by id asc limit " +
                    to_string(num);

    // 存储sql执行结果
    vector<vector<string>> *selectRes = new vector<vector<string>>;

    // 执行sql语句
    if (resCode = Table::obDb->selectData(sqlStr, selectRes)) {
        LOG_ERROR(resCode, "sql exec failure:" + sqlStr);
        delete selectRes;
        goto FINISH;
    }

    num = selectRes->size();
    // 开辟空间，为存储做准备
    res = new userInfo();
    ((userInfo *)res)->newId(-1, num);
    ((userInfo *)res)->newPhoneNumber("", num);
    ((userInfo *)res)->newEmail("", num);
    ((userInfo *)res)->newPasswd("", num);
    ((userInfo *)res)->newBalance(-1, num);
    // 将数据转储 selectRes --> res
    for (int i = 0; i < num; i++) {
        ((userInfo *)res)->id[i] = ToolFun::stringToInt(selectRes->at(i)[0]);
        ((userInfo *)res)->phone_number[i] = selectRes->at(i)[1];
        ((userInfo *)res)->email[i] = selectRes->at(i)[1];
        ((userInfo *)res)->passwd[i] = selectRes->at(i)[1];
        ((userInfo *)res)->balance[i] = ToolFun::stringTodouble(selectRes->at(i)[1]);
    }

    delete selectRes;
    this->pushPtr((userInfo *)res);

FINISH:
    return resCode;
}
int TbUserInfo::updateID(void *&pushPtr, const int &id) {

    int resCode = SUCCESS;
    string sqlStr;
    // string sqlTmp = "SET ";
    vector<string> vecTmp;

    if (NULL != ((TbUserInfo::userInfo *)pushPtr)->email)
        vecTmp.push_back(("email=" + Table::dealStr(*((TbUserInfo::userInfo *)pushPtr)->email)));
    if (NULL != ((TbUserInfo::userInfo *)pushPtr)->balance)
        vecTmp.push_back(("balance=" + to_string(*((TbUserInfo::userInfo *)pushPtr)->balance)));
    if (NULL != ((TbUserInfo::userInfo *)pushPtr)->passwd)
        vecTmp.push_back(("passwd=" + Table::dealStr(*((TbUserInfo::userInfo *)pushPtr)->passwd)));
    if (NULL != ((TbUserInfo::userInfo *)pushPtr)->phone_number)
        vecTmp.push_back(
            ("phone_number=" + Table::dealStr(*((TbUserInfo::userInfo *)pushPtr)->phone_number)));

    if (0 == vecTmp.size())
        goto FINISH;
    // 拼接sql语句
    sqlStr = "UPDATE " + TbUserInfo::Table::name + " SET " + ToolFun::joint(vecTmp) +
             " where id=" + to_string(id);
    LOG_DEBUG(0, sqlStr);

    // 执行sql语句
    if (resCode = Table::obDb->updateData(sqlStr)) {
        LOG_ERROR(resCode, "sql exec failure:" + sqlStr);
        goto FINISH;
    }

FINISH:
    return resCode;
}
int TbUserInfo::deleteID(const int &id) {
    int resCode = SUCCESS;
    string sqlStr;

    // 拼接sql语句
    sqlStr = "DELETE FROM " + TbUserInfo::Table::name + " WHERE id=" + to_string(id);
    LOG_DEBUG(0, sqlStr);

    // 执行sql语句
    if (resCode = Table::obDb->deleteData(sqlStr)) {
        LOG_ERROR(resCode, "sql exec failure:" + sqlStr);
        goto FINISH;
    }

FINISH:
    return resCode;
}
int TbUserInfo::insertOnce(void *&pushPtr) {
    int resCode = SUCCESS;
    string sqlStr;
    // string sqlTmp = "SET ";
    vector<string> vecTmp;

    if (NULL != ((TbUserInfo::userInfo *)pushPtr)->phone_number &&
        NULL != ((TbUserInfo::userInfo *)pushPtr)->passwd &&
        NULL != ((TbUserInfo::userInfo *)pushPtr)->balance &&
        NULL != ((TbUserInfo::userInfo *)pushPtr)->email &&
        NULL != ((TbUserInfo::userInfo *)pushPtr)->id) {
        vecTmp.push_back(Table::dealStr(*((TbUserInfo::userInfo *)pushPtr)->phone_number));
        vecTmp.push_back(Table::dealStr(*((TbUserInfo::userInfo *)pushPtr)->email));
        vecTmp.push_back(Table::dealStr(*((TbUserInfo::userInfo *)pushPtr)->passwd));
        vecTmp.push_back(to_string(*((TbUserInfo::userInfo *)pushPtr)->balance));

    } else {
        resCode = ERR_MSQ_PTR_IS_NULL;
        LOG_ERROR(resCode, "*phone_number or *passwd or *balance or *email is NULL");
        goto FINISH;
    }

    // 拼接sql语句
    sqlStr = "INSERT INTO " + TbUserInfo::Table::name +
             "(phone_number,email,passwd,balance) "
             "VALUES(" +
             ToolFun::joint(vecTmp) + "); ";
    LOG_DEBUG(0, sqlStr);

    // 执行sql语句
    if (resCode = Table::obDb->insertData(sqlStr, *((TbUserInfo::userInfo *)pushPtr)->id)) {
        LOG_ERROR(resCode, "sql exec failure:" + sqlStr);
        goto FINISH;
    }

FINISH:
    return resCode;
}

void TbUserInfo::pushPtr(userInfo *ptr) {
    this->last->next = new ptrList();
    this->last = this->last->next;
    this->last->value = ptr;
}