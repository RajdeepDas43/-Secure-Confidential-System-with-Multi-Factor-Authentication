#ifndef DATA_ENTRY_MANAGER_H
#define DATA_ENTRY_MANAGER_H

#include <string>
#include <memory>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include "KeyManagement.h"
#include "EncryptionContext.h"

class DataEntryManager {
private:
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> conn;
    EncryptionContext context;
    KeyManagement keyManager;

public:
    DataEntryManager(const std::string &keyFilePath);
    void storeDataEntry(const std::string &entryName, const std::string &data);
    std::string retrieveDataEntry(const std::string &entryName);
};

#endif // DATA_ENTRY_MANAGER_H
