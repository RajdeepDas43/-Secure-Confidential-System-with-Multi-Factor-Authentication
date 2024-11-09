#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "KeyManagement.h"
#include "AESEncryption.h"
#include "EncryptionContext.h"

class DataEntryManager {
private:
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> conn;
    EncryptionContext context;
    KeyManagement keyManager;

public:
    DataEntryManager(const std::string &keyFilePath) : keyManager(keyFilePath) {
        // Initialize encryption context
        keyManager.createAndStoreKey();
        context.setStrategy(new AESEncryption(keyManager));

        // Initialize database connection
        driver = sql::mysql::get_mysql_driver_instance();
        conn.reset(driver->connect("tcp://127.0.0.1:3306", "secureUser", "securePassword"));
        conn->setSchema("SecureSystemDB");
    }

    void storeDataEntry(const std::string &entryName, const std::string &data) {
        std::string encryptedData = context.encryptData(data);

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO DataEntries (entry_name, encrypted_data) VALUES (?, ?)"));
        pstmt->setString(1, entryName);
        pstmt->setString(2, encryptedData);
        pstmt->executeUpdate();
    }

    std::string retrieveDataEntry(const std::string &entryName) {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT encrypted_data FROM DataEntries WHERE entry_name = ?"));
        pstmt->setString(1, entryName);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            std::string encryptedData = res->getString("encrypted_data");
            return context.decryptData(encryptedData);
        } else {
            return "Data entry not found.";
        }
    }
};
