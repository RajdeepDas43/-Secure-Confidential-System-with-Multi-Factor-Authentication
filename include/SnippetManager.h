#ifndef SNIPPET_MANAGER_H
#define SNIPPET_MANAGER_H

#include <string>
#include <memory>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include "KeyManagement.h"
#include "EncryptionContext.h"

class SnippetManager {
private:
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> conn;
    EncryptionContext context;
    KeyManagement keyManager;

public:
    SnippetManager(const std::string &keyFilePath);
    void storeSnippet(const std::string &name, const std::string &code);
    std::string retrieveSnippet(const std::string &name);
};

#endif // SNIPPET_MANAGER_H
