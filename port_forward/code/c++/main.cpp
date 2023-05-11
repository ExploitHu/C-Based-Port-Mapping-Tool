#include "config.h"
#include "obDb.h"
#include "tbUserInfo.h"
#include <iostream>
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "uiFuncSys.h"
#include "uiFunc.h"
#define OBDB ObDb::getInstance()

using namespace std;

int main() {
/*
    ObDb *obdb = new ObDb();

    vector<vector<string>> *vec = new vector<vector<string>>;

    int a;
    obdb->selectData("select * from user_info where id>=10000005  order by id asc limit 2",vec);
    //obdb->insertData("INSERT INTO user_info(phone_number,email,passwd,balance) VALUES(\"13957974452\",\"81121190@qq.com\",\"testtest\",68);",a);
    //obdb->updateData("UPDATE user_info SET email=\"000\",passwd=\"======\" WHERE id=10000025;");
    cout << a << endl;
    for (int i = 0; i < vec->size(); i++) {
        for (vector<string>::iterator it = vec->at(i).begin(); it != vec->at(i).end(); it++) {
            cout << *it << "\t";
        }
        cout << endl;
    }

    delete vec;
    delete obdb;
*/
    // cout << Config::getInstance().getMysqlLogPath() << endl;
    // cout << ObDb::getInstance().a << endl;
    cout << "hello world" << endl;

    TbUserInfo* tbUserInfo = new TbUserInfo();
    void* data;
    int num = 10;
    tbUserInfo->selectAll(data, num);
    for(int i = 0; i < num; i++){

        cout << ((TbUserInfo::userInfo*)data)->id[i] << "   " << ((TbUserInfo::userInfo*)data)->phone_number[i] << endl;
    }

    //delete (TbUserInfo::userInfo*)data;

    //TbUserInfo::userInfo* tmpPtr = new TbUserInfo::userInfo;
    void* data1  = new TbUserInfo::userInfo;
    //((TbUserInfo::userInfo*)data1)->phone_number = new string("11557799662");
    //((TbUserInfo::userInfo*)data1)->email = new string("test.@qq.com");
    //((TbUserInfo::userInfo*)data1)->passwd = new string("testPassWd");
    //((TbUserInfo::userInfo*)data1)->balance = new double(99.75);

    ((TbUserInfo::userInfo*)data1)->newPhoneNumber("11557799662");
    ((TbUserInfo::userInfo*)data1)->newEmail("test.@qq.-com");
    ((TbUserInfo::userInfo*)data1)->newPasswd("testPassWd");
    ((TbUserInfo::userInfo*)data1)->newBalance(99.75);
    ((TbUserInfo::userInfo*)data1)->newId(-1);
    int tmpId= 10000012;
    tbUserInfo->updateID(data1,tmpId);

    //tbUserInfo->deleteID(10000004);
    tbUserInfo->insertOnce(data1);
    LOG_DEBUG(0,to_string(*((TbUserInfo::userInfo*)data1)->id) + "这是插入数据的id");

    delete (TbUserInfo::userInfo*)data1;
    delete tbUserInfo;

    json_t* pTmpJson = json_object();
    json_object_set_new(pTmpJson, "test",json_integer(1));


    Test(pTmpJson, nullptr);  
    json_delete(pTmpJson);

    //char** ppChar = {"aaa","bbb"};

    //ExecUI("Test",NULL,NULL);

    LOG_DEBUG(0,"正常退出！");
    return 0;
}

/*
int main(int argc, char *argv[]) {
    MYSQL *conn_ptr;
    MYSQL_RES *resPtr;
    MYSQL_ROW record;
    conn_ptr = mysql_init(NULL); // 初始化

    if (!conn_ptr) {
        fprintf(stderr, "mysql_init failed\n");
        return EXIT_FAILURE;
    }

    conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "091849", "port_forward", 0, NULL,
0);

    if(0 != mysql_query(conn_ptr, "select * from user_info")){
        printf("mysql_query == 0\n");
        return -1;
    }

    resPtr = mysql_store_result(conn_ptr);

    printf("print--------------------\n");
    while ((record = mysql_fetch_row(resPtr))) {

        printf("%s - %s - %s - %s - %s \n", record[0], record[1],record[2],record[3],record[4]);
    }
    mysql_close(conn_ptr);

    return EXIT_SUCCESS;
}*/