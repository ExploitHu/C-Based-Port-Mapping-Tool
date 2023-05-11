#ifndef TABLE_H
#define TABLE_H

#include "obDb.h"
#include "log.h"


class Table {
  private:
    
  public:
    string name;
    Table();
    ~Table();
    ObDb* obDb;
    /**
     * 查询所有字段
     * @param res 返回一个指针,其类型是 vector<struct>* .
     * @param num return result number
     * @param beginId The query starts at "beginId"
     * @param gt Is greater than or equal to beginId
     **/
    virtual int selectAll(void *&res, int &num, int beginId = 0, const bool& gt = true) = 0;
    /**
     * Updates the data with the specified id.
     * @param pushPtr push ptr,it should vector<struct>* type.
     * @param id
     **/
    virtual int updateID(void *&pushPtr, const int& id) = 0;
    /**
     * Deletes data with the specified id.
     * @param id The data for this id will be deleted
     **/
    virtual int deleteID(const int& id) = 0;
    /**
     * Inserting a data.
     * @param pushPtr The data to be inserted,it should vector<struct>* type.
     **/
    virtual int insertOnce(void *&pushPtr) = 0;

    //防止sql注入  sql语句中所有字符串都要调用函数处理  eg:  adsd-  ->  "adsd\-"
    string dealStr(const string &str);
};

#endif