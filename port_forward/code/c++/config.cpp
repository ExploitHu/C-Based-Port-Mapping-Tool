#include "config.h"

Config Config::instance;

Config::Config() {
    this->logPath = "/port_forward/log/portForward.log";
    this->maxLogFileSize = 10 * 1024 * 1024;
    this->maxLogBackupIndex = 1;
    this->nPortStart = 20000;
    this->nPortEnd = 30000;
    this->szServerIP = "192.168.32.130";
    this->dPortUnitPrice = 10;
}

Config &Config::getInstance() { return instance; }

std::string Config::getMysqlLogPath() { return this->logPath; }
size_t Config::getMaxLogFileSize() { return this->maxLogFileSize; }
unsigned int Config::getMaxLogBackupIndex() { return this->maxLogBackupIndex; }
unsigned int Config::getPortStart() { return this->nPortStart; }
unsigned int Config::getPortEnd() { return this->nPortEnd; }
string Config::getServerIP() { return this->szServerIP;}
double Config::getPortUnitPrice() { return this->dPortUnitPrice;}
