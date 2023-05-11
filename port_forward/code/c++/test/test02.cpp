#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <vector>
#include "jansson.h"
#include <vector>
#include <algorithm>

#define SUCCESS 0
//检查端口是否被使用
bool UseCheckPort(int nPort) 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(nPort);

    int status = bind(sock, (struct sockaddr *) &addr, sizeof(addr));

    if (status < 0)
    {
        close(sock);
        return true;
    }
  
    close(sock);
    return false;
}

//检查startPort - endPort范围内的端口是否被使用，并将其加入到pUsePort里
void UseCheckRangePort(int nStartPort, int nEndPort, std::vector<int>* pUsePort) 
{
    for (int tmpPort = nStartPort; tmpPort <= nEndPort; tmpPort++) 
    {
        if(UseCheckPort(tmpPort))
            pUsePort->push_back(tmpPort);
    }
}



template <typename T>
bool contains(std::vector<T>& vec, const T& element) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}



int main()
{
    auto start = std::chrono::high_resolution_clock::now();
/********************************************************************************************************/
    /*
    std::vector<int>* pUsePort = new std::vector<int>();
    UseCheckRangePort(1, 10000, pUsePort);

    for (auto nPort = pUsePort->begin(); nPort != pUsePort->end(); ++nPort) 
    {
        //std::cout << *it << " ";
        std::cout << "使用中：" <<  *nPort << std::endl;
    }
    */
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int element = 3;
    bool is_contained = contains(vec, element);
    if (is_contained) {
        std::cout << "Vector contains " << element << std::endl;
    } else {
        std::cout << "Vector does not contain " << element << std::endl;
    }



/********************************************************************************************************/
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Function took " << duration.count() << " milliseconds to run" << std::endl;

    return 0;
}