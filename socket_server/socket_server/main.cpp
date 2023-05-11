#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <iconv.h>
#include <sstream>
#include "lcx.h"
/*window下面socket编程需要的头文件和库*/
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

static vector<Lcx*> vLcx;

int formatStr(string inputString,int &nAction,int &nNum, string &remoteAdPort, string& localAdPort)
{
    //inputString  = "0 23 1.2.3.4:23 0.0.0.0:99";
    std::stringstream ss(inputString);

    ss >> nAction >> nNum >> remoteAdPort >>  localAdPort ;

    cout << "action: " << nAction << endl;
    cout << "num: " << nNum << endl;
    cout << "remoteAdPort: " << remoteAdPort << endl;
    cout << "localAdPort: " << localAdPort << endl;

    if (localAdPort == "") return -1;
    return 0;
}

/*

int main(int argc, char** argv)
{
Lcx* pLcx1 = new Lcx();

pLcx1->SetLocalIp("127.0.0.1");
pLcx1->SetLocalPort(20010);
pLcx1->SetRomoteIp("192.168.1.7");
pLcx1->SetRomotePort(20001);

    this->SetLocalPort(port);
    this->SetLocalIp(ip);
    this->SetRomotePort(port);
    this->SetRomoteIp(ip);

pLcx1->createSlaveThread();

cout << "pLcx1->closeThreadReleaseList()" << pLcx1->closeThreadReleaseList() << endl;
delete pLcx1;

return 0;
}

*/


string dealMsg(string msg) {
    int nAction;
    int nNum;
    string remoteAdPort;
    string localAdPort;

    if (formatStr(msg, nAction, nNum, remoteAdPort, localAdPort))
    {
        cout << "消息格式不对：" << msg << endl;
        return "-1";
    }

    if (0 == nAction)
    {
        cout << "************************************************************************";
        cout << localAdPort << "  " << remoteAdPort << "\n\n\n\n\n\n\n\n\n\n";
        Lcx* pLcx1 = new Lcx();
        pLcx1->num = nNum;
        vLcx.push_back(pLcx1);
        pLcx1->SetLocalAdPort(localAdPort);
        pLcx1->SetRomoteAdPort(remoteAdPort);
        pLcx1->createSlaveThread();
        return "0";
    }
    if (1 == nAction) {

        for (auto it = vLcx.begin(); it != vLcx.end();) {
            if ((*it)->num == nNum) {
                (*it)->closeThreadReleaseList();
                //delete* it;
                it = vLcx.erase(it);
            }
            else {
                ++it;
            }
        }

        return "0";
    }
    return "-1";
}

//简单的封装一下标准库的thread
//就多了一个isFinished函数，用来判断当前thread是否执行完毕

// 将 GBK 编码的字符串转换为 UTF-8 编码
std::string gbk_to_utf8(const std::string& gbk) {
    iconv_t conv = iconv_open("UTF-8", "GBK");

    if (conv == (iconv_t)-1) {
        throw std::runtime_error("iconv_open failed");
    }

    const char* inbuf = gbk.c_str();
    size_t inlen = gbk.length();
    size_t outlen = gbk.length() * 2;    // 预估输出缓冲区大小

    char* outbuf = new char[outlen];
    char* outptr = outbuf;

    if (iconv(conv, const_cast<char**>(&inbuf), &inlen, &outptr, &outlen) == (size_t)-1) {
        delete[] outbuf;
        iconv_close(conv);
        throw std::runtime_error("iconv failed");
    }

    std::string utf8(outbuf, outptr - outbuf);

    delete[] outbuf;

    iconv_close(conv);

    return utf8;
}

// 将 UTF-8 编码的字符串转换为 GBK 编码
std::string utf8_to_gbk(const std::string& utf8_str)
{
    std::string gbk_str;
    iconv_t conv = iconv_open("GBK", "UTF-8");
    if (conv == (iconv_t)-1) {
        return "";
    }

    int inbytesleft = utf8_str.size();
    char* inbuf = const_cast<char*>(utf8_str.c_str());

    // 需要分配足够大的输出缓冲区
    int outbytesleft = inbytesleft * 2;
    char* outbuf = new char[outbytesleft];
    char* outptr = outbuf;

    if (iconv(conv, &inbuf, (size_t*)&inbytesleft, &outptr, (size_t*)&outbytesleft) == -1) {
        delete[] outbuf;
        outbuf = nullptr;
        iconv_close(conv);
        return "";
    }

    // 转换成功后构造 string 并释放内存
    gbk_str.assign(outbuf, outptr - outbuf);
    delete[] outbuf;
    outbuf = nullptr;

    iconv_close(conv);
    return gbk_str;
}

class CThread
{
public:
    explicit CThread() : m_thread(), m_isFinished(false) {}
    ~CThread() { if (m_thread.joinable()) m_thread.join(); }

    CThread(const CThread*) = delete;
    CThread& operator=(const CThread&) = delete;

    CThread(CThread&& thread) : m_thread(std::move(thread.m_thread)), m_isFinished(thread.m_isFinished) {}

    template<typename Fn, typename ... Args>
    void run(Fn f, Args ... args)
    {
        m_thread = std::thread([this](Fn fn, Args ... args) {
            fn(args ...);
            m_isFinished = true;
            }, f, args ...);
    }

    void join() { m_thread.join(); }

    bool isFinished() { return m_isFinished; }
private:
    std::thread m_thread;
    bool m_isFinished;
};

//sock和对应的通信线程
std::map<int, CThread> mapSockThread;

//和sock代表的客户通信
void communication(int sock)
{
    constexpr int maxSize = 2048;
    char buff[maxSize];

    while (true)
    {
        int recvBytes = recv(sock, buff, maxSize, 0);

        if (recvBytes <= 0)
            break;

        std::cout << "recv : " << utf8_to_gbk(std::string(buff, recvBytes)) << " from sock : " << sock << std::endl;
        std::string message = dealMsg(utf8_to_gbk(std::string(buff, recvBytes)));
        //std::string message = "哈哈哈fuck! do not disturb me!!!";

        std::cout << "*******************************************************************************" << std::endl;
        std::cout << "回复内容：" << message << "\t\t\t\t回复内容长度：" << message.size() << std::endl;
        std::cout << "*******************************************************************************" << std::endl;

        int sendBytes = send(sock, gbk_to_utf8(message).c_str(), gbk_to_utf8(message).size(), 0);
    }
}

//监听想连接serverSock的请求
void listenConnection(int serverSock)
{
    sockaddr_in remoteAddr;
    int size = sizeof(remoteAddr);

    while (true)
    {
        std::vector<int> vecSock;
        //找到所有已经通信结束了的
        for (auto i = mapSockThread.begin(); i != mapSockThread.end(); ++i)
        {
            if (i->second.isFinished())
                vecSock.push_back(i->first);
        }
        //清掉那些已经通信结束了的
        for (auto i = vecSock.begin(); i != vecSock.end(); ++i)
        {
            closesocket(*i);
            mapSockThread.erase(*i);
            std::cout << "********erase : " << *i << "**********\n";
        }

        int remoteSock = accept(serverSock, (sockaddr*)(&remoteAddr), &size);

        if (remoteSock != INVALID_SOCKET)
        {
            mapSockThread.insert({ remoteSock, CThread() });
            mapSockThread.at(remoteSock).run(communication, remoteSock);
        }

    }
}

int main()
{
    std::cout << "hello server ... " << std::endl;

    //window下的socket通信，必须调用这个函数来初始化相应的模块
    WSAData wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cout << "socket error ...\n";
        return 0;
    }

    sockaddr_in sockAddr;
    std::memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(8888);

    if (bind(sock, (const sockaddr*)(&sockAddr), sizeof(sockAddr)) == -1)
    {
        std::cout << "bind error ...\n";
        return 0;
    }

    if (listen(sock, 5) == -1)
    {
        std::cout << "listen error ...\n";
        return 0;
    }

    std::thread listenThread(listenConnection, sock);
    listenThread.join();

    //释放服务器资源
    closesocket(sock);

    //释放资源
    WSACleanup();

    return 0;
}