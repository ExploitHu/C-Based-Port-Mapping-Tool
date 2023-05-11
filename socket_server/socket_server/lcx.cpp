#include "lcx.h"
#include <sstream>
using namespace std;

int Lcx::objSum = 0;
WSADATA Lcx::wsadata;


void splitAddress(const string& address, string& ip, int& port)
{
	size_t pos = address.find(':');
	if (pos != string::npos) {
		ip = address.substr(0, pos);
		string portString = address.substr(pos + 1);
		stringstream ss(portString);
		ss >> port;
	}
	else {
		ip = address;
		port = 0;
	}
}

Lcx::Lcx()
{
	//�߳���  start
	if (!objSum)
	{
		Lcx::wsadata;
		WSAStartup(MAKEWORD(1, 1), &Lcx::wsadata);
	}
	Lcx::objSum++;
	//�߳���  end
}

Lcx::~Lcx()
{
	//�߳���  start
	Lcx::objSum--;
	Lcx::objSum++;
	if (!objSum) WSACleanup();
	//�߳���  end

	//�ر��̣߳��ͷ�������
	this->closeThreadReleaseList();

	if (this->pHeader)
	{
		int nSum = 0;
		Stu_ListThreadSock* pTmp = this->pHeader;
		while (pTmp)
		{
			nSum++;
			pTmp = pTmp->next;
		}
		if (0 != nSum)
		{
			std::cout << "δ�ͷ���Դ" << nSum << ",����ǿ���ͷ���Դ\n";
			this->forcedReleaseThread();
		}
	}

}

/************************************************************************/
/*                                                                      */
/*							slave ����ģ��								*/
/*                                                                      */
/************************************************************************/
void Lcx::SetRomoteIp(const std::string szRomoteIp)
{
	strcpy(this->szRomoteIp, szRomoteIp.c_str());
}
void Lcx::SetRomoteIp(const char* szRomoteIp)
{
	strcpy(this->szRomoteIp, szRomoteIp);
}
void Lcx::SetLocalIp(const std::string szLocalIp)
{
	strcpy(this->szLocalIp, szLocalIp.c_str());
}
void Lcx::SetLocalIp(const char* szLocalIp)
{
	strcpy(this->szLocalIp, szLocalIp);
}
void Lcx::SetRomotePort(const int nRomotePort)
{
	this->nRomotePort = nRomotePort;
}
void Lcx::SetLocalPort(const int nLocalPort)
{
	this->nLocalPort = nLocalPort;
}


void Lcx::tailInsertion(Stu_ThreadSock* pNew)
{
	Stu_ListThreadSock* pTmp = new Stu_ListThreadSock;
	pTmp->value = pNew;
	pTmp->next = NULL;
	if (!this->pHeader)
	{
		this->pHeader = pTmp;
		this->pLast = pTmp;
	}
	else
	{
		this->pLast->next = pTmp;
		this->pLast = pTmp;
	}
}
void Lcx::traverseAndFreeThread()
{
	Stu_ListThreadSock* pTmp1 = NULL;
	Stu_ListThreadSock* pTmp2 = this->pHeader;
	while (pTmp2)
	{
		if (!pTmp2->value->bStatus)
		{
			//pTmp2����ͷ
			if (pTmp1)
			{
				pTmp1->next = pTmp2->next;
				//�ͷ���Դ
				pTmp2->value->pThread->detach();
				delete pTmp2->value->pThread;
				delete pTmp2;

				pTmp2 = pTmp1->next;
			}
			//pTmp2��ͷ
			else
			{
				Stu_ListThreadSock* pTmpTmp = pTmp2->next;
				//�ͷ���Դ
				pTmp2->value->pThread->detach();
				delete pTmp2->value->pThread;
				delete pTmp2;
				this->pHeader = pTmpTmp;
				pTmp2 = pTmpTmp;
			}
			cout << "�ͷ�һ��\n";
		}
		this->pLast = pTmp2;
		pTmp1 = pTmp2;
		pTmp2 = pTmp2->next;
		cout << "����������" << endl;
	}
}


void Lcx::forcedReleaseThread()
{
	Stu_ListThreadSock* pTmp1 = NULL;
	Stu_ListThreadSock* pTmp2 = this->pHeader;
	while (pTmp2)
	{
		pTmp1 = pTmp2->next;
		pTmp2->value->pThread->detach();
		delete pTmp2->value->pThread;
		delete pTmp2;
		pTmp2 = NULL;
		cout << "�ͷ�һ��\n";
		pTmp2 = pTmp1;
	}
	this->pHeader = NULL;
}


void Lcx::datatrans(Stu_ThreadSock* stu_ThreadSock)
{
	char host_slave[20] = { 0 };
	char host_hacker[20] = { 0 };
	sockaddr_in	addr = { 0 };
	SOCKET	pS1 = stu_ThreadSock->pS1;	//���ܵ���slave������
	SOCKET	pS2 = stu_ThreadSock->pS2;	//���ͳ�ȥ��socket
	int	sentacount1 = 0;
	int readsize;
	int sizeofaddr = sizeof(sockaddr);
	int maxfd = max(pS1, pS2) + 1;
	fd_set	readfd;
	fd_set	writefd;
	bool flag = false;
	timeval	timeset;

	timeset.tv_sec = 300;
	timeset.tv_usec = 0;

	if (getpeername(pS1, (sockaddr*)&addr, &sizeofaddr))
	{

		strcpy_s(host_slave, inet_ntoa(addr.sin_addr));
		int port_slave = ntohs(addr.sin_port);
	}
	if (getpeername(pS2, (sockaddr*)&addr, &sizeofaddr))
	{

		strcpy_s(host_hacker, inet_ntoa(addr.sin_addr));
		int port_hacker = ntohs(addr.sin_port);
	}
	cout << "[+] Start Transport (" << host_slave << "<-> " << host_hacker << ") ......" << endl;
	char RecvBuffer[20480];
	char SendBuffer[20480];



	while (TRUE)
	{
		readsize = 0;
		FD_ZERO(&readfd);
		FD_ZERO(&writefd);
		FD_SET((UINT)pS1, &readfd);
		FD_SET((UINT)pS2, &readfd);
		FD_SET((UINT)pS1, &writefd);
		FD_SET((UINT)pS2, &writefd);

		int result = select(maxfd, &readfd, &writefd, NULL, &timeset);
		if (result < 0 && (errno != EINTR))
		{
			cout << "[-] Select error." << endl;
			break;
		}
		else if (result == 0)
		{
			cout << "[-] Socket time out." << endl;
			break;
		}

		//û����û��ʱ
		if (FD_ISSET(pS1, &readfd) && flag)
		{
			readsize = recv(pS1, RecvBuffer, 20480, 0);//����host�����󡣡�
			if (readsize == -1)
			{
				break;
			}
			if (readsize == SOCKET_ERROR || readsize == 0)
			{
				cout << "readsize == SOCKET_ERROR || readsize == 0" << endl;
				break;
			}
			memcpy(SendBuffer, RecvBuffer, readsize);
			memset(RecvBuffer, 0, 20480);
		}


		if (FD_ISSET(pS1, &writefd) && flag && (readsize > 0))
		{

			int sendsize = send(pS2, SendBuffer, readsize, 0);//����slave
			if (sendsize == 0)
			{
				break;
			}
			if (sendsize < 0 && (errno != EINTR))
			{
				cout << "[-] Send to s2 unknow error." << endl;
				break;
			}

			memset(SendBuffer, 0, 20480);
			cout << " Send " << sendsize << " bytes " << endl;

		}
		if (FD_ISSET(pS2, &readfd) && (!flag))
		{
			readsize = recv(pS2, RecvBuffer, 20480, 0);//����slave��������
			memcpy(SendBuffer, RecvBuffer, readsize);
			cout << " [+] Recv " << readsize << " bytes from host." << endl;
			memset(RecvBuffer, 0, 20480);

			if (0 == readsize) break;
		}
		if (FD_ISSET(pS1, &writefd) && (!flag) && (readsize > 0))
		{
			readsize = send(pS1, SendBuffer, readsize, 0);//����host
			if (readsize == 0)
			{
				break;
			}
			if (readsize < 0)
			{
				cout << "[-] Send to s2 unknow error." << endl;
				break;
			}
			cout << " Send " << readsize << " bytes " << endl;
			memset(SendBuffer, 0, 20480);

		}

		flag = !flag;

		Sleep(5);
		cout << "--------------------------------------while\n";
	}
	closesocket(pS1);
	closesocket(pS2);
	cout << "[+] OK! I Closed The Two Socket." << endl;
	stu_ThreadSock->bStatus = false;
}

int Lcx::slave()
{
	Stu_ThreadSock* stu_ThreadSock = NULL;
	fd_set		fd;
	char		buffer[20480];
	int			l = -1;
	timeval	timeset;

	timeset.tv_sec = 1;
	timeset.tv_usec = 0;

	//����״̬ ����
	if (-3 != this->nStatus) this->nStatus = 0;
	while (TRUE)
	{
		//��ֹ����
		if (-3 == this->nStatus)
		{
			this->nStatus = -1;
			return 0;
		}

		//�����׽���
		sock1 = socket(AF_INET, SOCK_STREAM, 0);
		sock2 = socket(AF_INET, SOCK_STREAM, 0);

		cout << "[+] Make a Connection to " << this->szRomoteIp << "on port:" << this->szLocalIp << "...." << endl;
		if (sock1 < 0 || sock1 < 0)
		{
			if (-3 != this->nStatus) this->nStatus = 1;
			cout << "[-] Create socket error" << endl;
			return this->nStatus;
		}
		fflush(stdout);
		if (client_connect(sock1, this->szRomoteIp, this->nRomotePort))
		{
			if (-3 != this->nStatus) this->nStatus = 2;
			closesocket(sock1);
			closesocket(sock2);
			continue;/*�������ѭ��*/
		}

		memset(buffer, 0, 20480);

		while (1)
		{
			//��sock����,����set��
			FD_ZERO(&fd);
			FD_SET(sock1, &fd);

			//����
			cout << "==================================================================\n";

			//select�¼�	�� д �쳣
			if (select(sock1 + 1, &fd, NULL, NULL, &timeset) == SOCKET_ERROR)
			{
				if (-3 != this->nStatus) this->nStatus = 2;
				if (errno == WSAEINTR) continue;
				break;
			}

			//FD_ISSET����ֵ>0 ��ʾSET��Ŀɶ�д
			if (FD_ISSET(sock1, &fd))
			{
				l = recv(sock1, buffer, 20480, 0);
				break;
			}

			//��ֹ����
			if (-3 == this->nStatus)
			{
				this->nStatus = -1;
				return 0;
			}
		}

		if (l <= 0)
		{
			if (-3 != this->nStatus) this->nStatus = 2;
			cout << "[-] There is a error...Create a new connection." << endl;
			continue;
		}
		while (1)
		{
			cout << "[+] Connect OK!    \n[+] xlcTeam congratulations!" << endl;
			cout << "[+] Make a Connection to " << this->szRomoteIp << "on port:" << this->szLocalIp << "...." << endl;
			fflush(stdout);
			if (client_connect(sock2, this->szLocalIp, this->nLocalPort))
			{
				if (-3 != this->nStatus) this->nStatus = 2;
				closesocket(sock1);
				closesocket(sock2);
				continue;
			}

			if (send(sock2, buffer, l, 0) == SOCKET_ERROR)
			{
				if (-3 != this->nStatus) this->nStatus = 2;
				cout << "[-] Send failed." << endl;
				continue;
			}

			l = 0;
			memset(buffer, 0, 20480);
			break;
		}

		if (-3 != this->nStatus)  this->nStatus = 0;
		cout << "[+] All Connect OK!" << endl;
		stu_ThreadSock = new Stu_ThreadSock;
		stu_ThreadSock->pS1 = sock1;
		stu_ThreadSock->pS2 = sock2;

		stu_ThreadSock->pThread = new std::thread(&Lcx::datatrans, this, stu_ThreadSock);

		if (stu_ThreadSock->pThread == NULL)
		{
			if (-3 != this->nStatus) this->nStatus = 3;
			TerminateThread(stu_ThreadSock, 0);
			delete stu_ThreadSock;
			stu_ThreadSock = NULL;
		}
		else
		{
			this->tailInsertion(stu_ThreadSock);
		}
		this->traverseAndFreeThread();
	}

	return this->nStatus;
}

//���IP��ַ��ʽ�Ƿ���ȷ
bool Lcx::checkIP(char* str)
{
	if (INADDR_NONE == inet_addr(str))
		return FALSE;
	return true;
}

int Lcx::client_connect(int sockfd, char* server, int port)
{					/*sock*/		/*Զ��IP*/	/*Զ�̶˿�*/
	struct sockaddr_in cliaddr;
	struct hostent* host;

	if (!(host = gethostbyname(server)))	//���Զ��������IP
	{
		//   printf("[-] Gethostbyname(%s) error:%s\n",server,strerror(errno));
		return 1;
	}
	//����ַ�ṹ��ֵ
	memset(&cliaddr, 0, sizeof(struct sockaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(port);/*Զ�̶˿�*/
	cliaddr.sin_addr = *((struct in_addr*)host->h_addr);//host ip

	//ȥ����Զ������listen������
	if (connect(sockfd, (struct sockaddr*)&cliaddr, sizeof(struct sockaddr)) < 0)
	{
		//        printf("[-] Connect error.\r\n");
		return 1;
	}
	return 0;
}

int Lcx::createSlaveThread()
{
	if (NULL != this->pMainThread || -1 != this->nStatus) return -1;
	this->pMainThread = new std::thread(&Lcx::slave, this);
}

int Lcx::closeThreadReleaseList()
{
	cout << num <<" ׼���ر��߳�          *****************************" << endl;
	int nRet = 0;
	//׼���ر��߳�
	if (NULL != this->pMainThread)
	{
		this->nStatus = -3;
		int nCheckStatus = 4;	//�����4��
		while (-1 == this->nStatus && nCheckStatus) Sleep(0.5 * 1000);

		if (0 == nCheckStatus) nRet = -1;

		//��ʱδ�ͷ�ǿ���˳�
		this->pMainThread->detach();
		delete this->pMainThread;
		this->pMainThread = NULL;
	}

	int i = 3;	//����ͷ����� 
	while (!this->pHeader)
	{
		traverseAndFreeThread();
		if (!this->pHeader || !--i) break;
		Sleep(0.5 * 1000);
	}

	if (this->pHeader)
	{
		nRet = -2;
		int nSum = 0;
		Stu_ListThreadSock* pTmp = this->pHeader;
		while (pTmp)
		{
			nSum++;
			pTmp = pTmp->next;
		}

		std::cout << "�ڴ�й¶����(" << nSum << ")" << endl;
	}

	return nRet;
}


void Lcx::SetLocalAdPort(const std::string szLocalAdPort)
{
	string ip;
	int port;
	splitAddress(szLocalAdPort, ip, port);
	this->SetLocalPort(port);
	this->SetLocalIp(ip);

}

void Lcx::SetRomoteAdPort(const std::string szRomoteAdPort)
{

	string ip;
	int port;
	splitAddress(szRomoteAdPort, ip, port);
	this->SetRomotePort(port);
	this->SetRomoteIp(ip);

}