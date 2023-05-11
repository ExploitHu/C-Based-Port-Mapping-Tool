#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <winsock2.h> 
#include <io.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

typedef	struct Stu_ThreadSock_ {
	SOCKET	pS1;
	SOCKET	pS2;
	std::thread* pThread = NULL;
	bool bStatus = true;
}Stu_ThreadSock;

typedef	struct Stu_ListThreadSock_ {
	Stu_ThreadSock* value = NULL;
	struct Stu_ListThreadSock_* next = NULL;
}Stu_ListThreadSock;


class Lcx
{
public:
	Lcx();
	~Lcx();

	int num;

	void SetRomoteIp(const std::string szRomoteIp);
	void SetRomoteIp(const char* szRomoteIp);
	void SetLocalIp(const std::string szLocalIp);
	void SetLocalIp(const char* szLocalIp);
	void SetRomotePort(const int nRomotePort);
	void SetLocalPort(const int nLocalPort);
	void SetLocalAdPort(const std::string szLocalAdPort);
	void SetRomoteAdPort(const std::string szRomoteAdPort);


	//void funlisten(int port1, int port2);						//监听功能函数
	//bool bindAndFunlisten(SOCKET s, int port);				//绑定socket的地址结构
	int createSlaveThread();									//创建一个映射线程
	int closeThreadReleaseList();								//关闭线程、释放链表
private:
	static WSADATA wsadata;
	static int objSum;

	char szRomoteIp[16];
	char szLocalIp[16];
	int nRomotePort;
	int nLocalPort;
	/*
	准备关闭连接(此状态只能变为-1)：	-3
	等待套接字的可读事件：			-2
	未连接(关闭后的状态)：			-1
	已连接：							0
	连接错误							1
	连接异常							2
	线程创建异常						3
	*/
	int nStatus = -1;
	SOCKET sock1;
	SOCKET sock2;
	std::thread* pMainThread = NULL;

	Stu_ListThreadSock* pHeader = NULL;
	Stu_ListThreadSock* pLast = NULL;


	void tailInsertion(Stu_ThreadSock* pNew);					//尾插法插入一个Stu_ListThreadSock*
	void traverseAndFreeThread();								//遍历并且释放完成任务的线程
	void forcedReleaseThread();									//遍历并且强制释放所有的线程
	int  client_connect(int sockfd, char* server, int port);	//连接服务端
	bool checkIP(char* str);									//检查IP格式是否正确
	void datatrans(Stu_ThreadSock* stu_ThreadSock);				//数据转发函数
	int slave();												//slave函数

};