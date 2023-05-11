#ifndef LOG_H
#define LOG_H
#include "config.h"
#include "toolFun.h"
#include <iostream>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <string>


#define LOG Log::getInstance()
#define LOG_ERROR(code, message) LOG.error(code, message, __FILE__, __LINE__)
#define LOG_WARN(code, message) LOG.warn(code, message, __FILE__, __LINE__)
#define LOG_NOTICE(code, message) LOG.notice(code, message, __FILE__, __LINE__)
#define LOG_INFO(code, message) LOG.info(code, message, __FILE__, __LINE__)
#define LOG_DEBUG(code, message) LOG.debug(code, message, __FILE__, __LINE__)
/* 写入日志宏
#define LOG Log::getInstance()
#define LOG_ERROR(code,message)   LOG.error(code,message, __FILE__, __LINE__)
#define LOG_WARN(code,message)    LOG.warn(code,message, __FILE__, __LINE__)
#define LOG_NOTICE(code,message)  LOG.notice(code,message, __FILE__, __LINE__)
#define LOG_INFO(code,message)    LOG.info(code,message, __FILE__, __LINE__)
#define LOG_DEBUG(code,message)   LOG.debug(code,message, __FILE__, __LINE__)
*/

/// @brief 日志类
class Log {
  private:
    // 日志等级枚举
    typedef enum { ERROR = 300, WARN = 400, NOTICE = 500, INFO = 600, DEBUG = 700 } PriorityLevel;

    // 构造函数
    Log();
    // 析构函数
    ~Log();
    // 单例指针
    static Log instance;
    // 日志等级
    PriorityLevel logLevel;

    // log4cpp
    log4cpp::PatternLayout *layout;
    log4cpp::Appender *appender;
    log4cpp::Appender *cmdBox;

  public:
    // get方法
    static Log &getInstance();

    // 写入日志
    void error(int code, std::string message, std::string file, int line);
    void warn(int code, std::string message, std::string file, int line);
    void notice(int code, std::string message, std::string file, int line);
    void info(int code, std::string message, std::string file, int line);
    void debug(int code, std::string message, std::string file, int line);
  
};

#endif