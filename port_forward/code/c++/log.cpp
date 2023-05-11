#include "log.h"

Log Log::instance;

Log::~Log(){
    log4cpp::Category::shutdown();
}
Log::Log() {

    this->logLevel = PriorityLevel::DEBUG;

    // 1. 初始化一个Layout对象(格式)
    this->layout = new log4cpp::PatternLayout();
    // layout->setConversionPattern("%d <%p> %c %x: %m%n");
    layout->setConversionPattern("%d <%p> : %m%n");
    // 2. 初始化一个appender 对象
    this->appender =
        new log4cpp::RollingFileAppender("FileAppender", CONFIG.getMysqlLogPath(),
                                         CONFIG.getMaxLogFileSize(), CONFIG.getMaxLogBackupIndex());
    this->cmdBox = new log4cpp::OstreamAppender("osAppender", &cout);
    // 3. 把layout对象附着在appender对象上
    this->appender->setLayout(this->layout);

    /*
        // 4. 实例化一个category对象
        log4cpp::Category &warn_log = log4cpp::Category::getInstance("mywarn");
        // 5. 设置additivity为false，替换已有的appender
        warn_log.setAdditivity(false);
        // 5. 把appender对象附到category上
        warn_log.setAppender(this->appender);
        // 6. 设置category的优先级，低于此优先级的日志不被记录
        warn_log.setPriority(log4cpp::Priority::WARN);
        */
}
Log &Log::getInstance() { return instance; }

void Log::error(int code, std::string message, std::string file, int line) {
    std::string logMessage = "[" + file + ',' + to_string(line) + "]" + ":ERR(" +
                             to_string(code) + ") " + message;
    log4cpp::Category &log = log4cpp::Category::getInstance("log");
    log.setAdditivity(false);
    log.setAppender(this->appender);
    log.setPriority(this->logLevel);

    log.error(logMessage);
}
void Log::warn(int code, std::string message, std::string file, int line) {
    std::string logMessage = "[" + file + ',' + to_string(line) + "]" + ":WARN(" +
                            to_string(code) + ") " + message;
    log4cpp::Category &log = log4cpp::Category::getInstance("log");
    log.setAdditivity(false);
    log.setAppender(this->appender);
    log.setPriority(this->logLevel);

    log.warn(logMessage);
}
void Log::notice(int code, std::string message, std::string file, int line) {
    std::string logMessage = "[" + file + ',' + to_string(line) + "]" + ":NOTICE(" +
                             to_string(code) + ") " + message;
    log4cpp::Category &log = log4cpp::Category::getInstance("log");
    log.setAdditivity(false);
    log.setAppender(this->appender);
    log.setPriority(this->logLevel);

    log.notice(logMessage);
}
void Log::info(int code, std::string message, std::string file, int line) {
    
    std::string logMessage = "[" + file + ',' + to_string(line) + "]" + ":INFO(" +
                             to_string(code) + ") " + message;
    log4cpp::Category &log = log4cpp::Category::getInstance("log");
    log.setAdditivity(false);
    log.setAppender(this->appender);
    log.setPriority(this->logLevel);

    log.info(logMessage);
}
void Log::debug(int code, std::string message, std::string file, int line) {
    std::string logMessage = "[" + file + ',' + to_string(line) + "]" + ":DEBUG(" +
                            to_string(code)+ ") " + message;
    log4cpp::Category &log = log4cpp::Category::getInstance("log");
    log.setAdditivity(false);
    log.setAppender(this->cmdBox);
    log.setPriority(this->logLevel);

    log.debug(logMessage);
}