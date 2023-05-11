#include <iostream>
#include <sstream>

std::string outPointerAddress(void* ptr)
{
    // 将地址转换为字符串
    std::stringstream ss;
    ss << (void*)ptr;
    return ss.str();
}


int main()
{

    const char* szTmp = "aaa";
    std::cout << "call func(MyFreeMemory),Buf address is:" + outPointerAddress((void*)szTmp) << std::endl;

    return 0;
}