#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <string>

using namespace std;

#define CONFIG Config::getInstance()

/// @brief 配置文件信息类
class Config {
  private:
    // 构造函数
    Config();
    //// 析构函数
    //~Config();
    // 单例指针
    static Config instance;
    //日志路径
    std::string logPath;
    //日志文件最大字节
    size_t maxLogFileSize;
    //最大回滚日志数
    unsigned int maxLogBackupIndex;
    //端口映射使用的端口 （起始）
    unsigned int nPortStart;
    //端口映射使用的端口 （最后）
    unsigned int nPortEnd;
    //端口映射使用的端口 （最后）
    string szServerIP;
    //端口单价
    double dPortUnitPrice;
  public:
    // get方法
    static Config &getInstance();
    std::string getMysqlLogPath();
    size_t getMaxLogFileSize();
    unsigned int getMaxLogBackupIndex();
    unsigned int getPortStart();
    unsigned int getPortEnd();
    string getServerIP();
    double getPortUnitPrice();
};

#endif