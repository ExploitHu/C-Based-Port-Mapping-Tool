#ifndef OBDB_H
#define OBDB_H
#include "error.h"
#include "log.h"
#include "toolFun.h"
#include <iostream>
#include <mysql/mysql.h>
#include <string>
#include <vector>

using namespace std;

/// @brief 操作数据库类
class ObDb {
  private:
    MYSQL *conn_ptr;

    // 要连接的数据库信息
    const char *host = "localhost";
    const char *user = "root";
    const char *passwd = "091849";
    const char *db = "port_forward";

  public:
    // 增
    int insertData(string sqlStr, int &id);
    // 删
    int deleteData(string sqlStr);
    // 改
    int updateData(string sqlStr);
    // 查
    int selectData(string sqlStr, vector<vector<string>> *&res);


    int testInt = 1;

    int BeginTransaction();
    int RollbackTransaction();
    int CommitTransaction();

    ObDb();
    ~ObDb();
};

#endif