#ifndef TBUSERINFO_H
#define TBUSERINFO_H
#include "error.h"
#include "log.h"
#include "table.h"
#include "toolFun.h"
#include "toolFun.h"
#include <iostream>
#include <string>
#include <vector>

class TbUserInfo : public Table {
  public:
    typedef struct stctUserInfo {
        int *id = NULL;
        std::string *phone_number = NULL;
        std::string *email = NULL;
        std::string *passwd = NULL;
        double *balance = NULL;

        void newId(const int initValue, int num = 1) {
            this->id = new int[num];
            for (int i = 0; i < num; i++) {
                this->id[i] = initValue;
            }
        }
        void newPhoneNumber(const string initValue, int num = 1) {
            this->phone_number = new string[num];
            for (int i = 0; i < num; i++) {
                this->phone_number[i] = initValue;
            }
        }
        void newEmail(const string initValue, int num = 1) {
            this->email = new string[num];
            for (int i = 0; i < num; i++) {
                this->email[i] = initValue;
            }
        }
        void newPasswd(const string initValue, int num = 1) {
            this->passwd = new string[num];
            for (int i = 0; i < num; i++) {
                this->passwd[i] = initValue;
            }
        }
        void newBalance(const double initValue, int num = 1) {
            this->balance = new double[num];
            for (int i = 0; i < num; i++) {
                this->balance[i] = initValue;
            }
        }

        ~stctUserInfo() {
            LOG_DEBUG(0, "开始释放user_info内存");
            if (NULL != id)
                delete[] id;
            if (NULL != phone_number)
                delete[] phone_number;
            if (NULL != email)
                delete[] email;
            if (NULL != passwd)
                delete[] passwd;
            if (NULL != balance)
                delete[] balance;
        }
    } userInfo;

    TbUserInfo();
    ~TbUserInfo();

    //普通查找
    int selectAll(void *&res, int &num, int beginId = 0, const bool &gt = true);
    int updateID(void *&pushPtr, const int &id);
    int deleteID(const int &id);
    int insertOnce(void *&pushPtr);


  private:
    typedef struct pList {
        struct pList *next;
        userInfo *value;

        ~pList() {
            delete value;
            delete next;
        }
    } ptrList;

    ptrList *head;
    ptrList *last;
    void pushPtr(userInfo *ptr);
};

#endif