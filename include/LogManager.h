#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <string>
#include <memory>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include "KeyManagement.h"
#include "EncryptionContext.h"

class LogManager {
private:
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> conn;
    EncryptionContext context;
    KeyManagement keyManager;

public:
    LogManager(const std::string &keyFilePath);
    void logEvent(const std::string &eventDescription);
    void readLogs();
};

#endif // LOG_MANAGER_H
