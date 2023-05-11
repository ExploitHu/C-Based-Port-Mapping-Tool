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


	//void funlisten(int port1, int port2);						//�������ܺ���
	//bool bindAndFunlisten(SOCKET s, int port);				//��socket�ĵ�ַ�ṹ
	int createSlaveThread();									//����һ��ӳ���߳�
	int closeThreadReleaseList();								//�ر��̡߳��ͷ�����
private:
	static WSADATA wsadata;
	static int objSum;

	char szRomoteIp[16];
	char szLocalIp[16];
	int nRomotePort;
	int nLocalPort;
	/*
	׼���ر�����(��״ֻ̬�ܱ�Ϊ-1)��	-3
	�ȴ��׽��ֵĿɶ��¼���			-2
	δ����(�رպ��״̬)��			-1
	�����ӣ�							0
	���Ӵ���							1
	�����쳣							2
	�̴߳����쳣						3
	*/
	int nStatus = -1;
	SOCKET sock1;
	SOCKET sock2;
	std::thread* pMainThread = NULL;

	Stu_ListThreadSock* pHeader = NULL;
	Stu_ListThreadSock* pLast = NULL;


	void tailInsertion(Stu_ThreadSock* pNew);					//β�巨����һ��Stu_ListThreadSock*
	void traverseAndFreeThread();								//���������ͷ����������߳�
	void forcedReleaseThread();									//��������ǿ���ͷ����е��߳�
	int  client_connect(int sockfd, char* server, int port);	//���ӷ����
	bool checkIP(char* str);									//���IP��ʽ�Ƿ���ȷ
	void datatrans(Stu_ThreadSock* stu_ThreadSock);				//����ת������
	int slave();												//slave����

};