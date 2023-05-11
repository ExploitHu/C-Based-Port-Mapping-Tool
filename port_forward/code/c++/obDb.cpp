#include "obDb.h"

ObDb::ObDb() {
    // 初始化一个MYSQL*对象
    this->conn_ptr = mysql_init(NULL);
    if (this->conn_ptr == NULL) {
        LOG_WARN(ERR_MSQ_INSUFFICIENT_MEMORY, "memory overflow");
        return;
    }
    this->conn_ptr = mysql_real_connect(this->conn_ptr, this->host, this->user, this->passwd,
                                        this->db, 0, NULL, 0);

    LOG_INFO(SUCCESS, "ObDb() init success");
}

ObDb::~ObDb() { mysql_close(this->conn_ptr); }

// 增
int ObDb::insertData(string sqlStr, int &id) {
    int resCode = SUCCESS;

    // 执行sql语句
    if (0 != mysql_query(this->conn_ptr, ToolFun::stringToChar(sqlStr))) {
        resCode = ERR_MSQ_FUNCTION_CALL_DEFEATED;
        LOG_ERROR(resCode, "mysql_query call defeated(" + sqlStr + ")");
        goto FINISH;
    }
    id = mysql_insert_id(this->conn_ptr);
    LOG_INFO(resCode, sqlStr);
FINISH:
    return resCode;
}
// 删
int ObDb::deleteData(string sqlStr) {
    int resCode = SUCCESS;

    // 执行sql语句
    if (0 != mysql_query(this->conn_ptr, ToolFun::stringToChar(sqlStr))) {
        resCode = ERR_MSQ_FUNCTION_CALL_DEFEATED;
        LOG_ERROR(resCode, "mysql_query call defeated(" + sqlStr + ")");
        goto FINISH;
    }

    LOG_INFO(resCode, sqlStr);
FINISH:
    return resCode;
}
// 改
int ObDb::updateData(string sqlStr) {
    int resCode = SUCCESS;

    // 执行sql语句
    if (resCode = mysql_query(this->conn_ptr, ToolFun::stringToChar(sqlStr))) {
        LOG_ERROR(resCode, "mysql_query call defeated(" + sqlStr + ")");
        resCode = ERR_MSQ_FUNCTION_CALL_DEFEATED;
        goto FINISH;
    }

    LOG_INFO(resCode, sqlStr);
FINISH:
    return resCode;
}
// 查
int ObDb::selectData(string sqlStr, vector<vector<string>> *&res) {
    int resCode = SUCCESS;
    MYSQL_RES *sqlResPtr;
    MYSQL_ROW record;
    int columns = 0;

    // res容器不能为空
    if (NULL == res) {
        resCode = ERR_MSQ_PTR_IS_NULL;
        LOG_ERROR(resCode, "res == NULL");
        goto FINISH;
    }

    // 执行sql语句
    if (0 != mysql_query(this->conn_ptr, ToolFun::stringToChar(sqlStr))) {
        resCode = ERR_MSQ_FUNCTION_CALL_DEFEATED;
        LOG_ERROR(resCode, "mysql_query call defeated(" + sqlStr + ")");
        goto FINISH;
    }

    // 获取执行结果，将结果存储到sqlResPtr
    sqlResPtr = mysql_store_result(conn_ptr);

    // 将结果转储到res
    columns = mysql_num_fields(sqlResPtr);

    while ((record = mysql_fetch_row(sqlResPtr))) {

        vector<string> rowData;
        for (int i = 0; i < columns; i++) {
            rowData.push_back(record[i]);
        }
        res->push_back(rowData);
    }

    LOG_INFO(resCode, sqlStr);

FINISH:
    mysql_free_result(sqlResPtr);
    return resCode;
}

int ObDb::BeginTransaction()
{
    int resCode = SUCCESS;
    const string sqlStr = "begin";

    // 执行sql语句
    if (0 != mysql_query(this->conn_ptr, sqlStr.c_str())) {
        resCode = ERR_MSQ_FUNCTION_CALL_DEFEATED;
        LOG_ERROR(resCode, "mysql_query call defeated(" + sqlStr + ")");
        goto FINISH;
    }

    LOG_INFO(resCode, sqlStr);
FINISH:
    return resCode;
}

int ObDb::CommitTransaction()
{
    int resCode = SUCCESS;
    const string sqlStr = "commit";

    // 执行sql语句
    if (0 != mysql_query(this->conn_ptr, sqlStr.c_str())) {
        resCode = ERR_MSQ_FUNCTION_CALL_DEFEATED;
        LOG_ERROR(resCode, "mysql_query call defeated(" + sqlStr + ")");
        goto FINISH;
    }

    LOG_INFO(resCode, sqlStr);
FINISH:
    return resCode;
}


int ObDb::RollbackTransaction()
{
    int resCode = SUCCESS;
    const string sqlStr = "rollback";

    // 执行sql语句
    if (0 != mysql_query(this->conn_ptr, sqlStr.c_str())) {
        resCode = ERR_MSQ_FUNCTION_CALL_DEFEATED;
        LOG_ERROR(resCode, "mysql_query call defeated(" + sqlStr + ")");
        goto FINISH;
    }

    LOG_INFO(resCode, sqlStr);
FINISH:
    return resCode;
}