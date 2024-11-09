#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "KeyManagement.h"
#include "AESEncryption.h"
#include "EncryptionContext.h"

class LogManager {
private:
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> conn;
    EncryptionContext context;
    KeyManagement keyManager;

public:
    LogManager(const std::string &keyFilePath) : keyManager(keyFilePath) {
        // Initialize encryption context
        keyManager.createAndStoreKey();
        context.setStrategy(new AESEncryption(keyManager));

        // Initialize database connection
        driver = sql::mysql::get_mysql_driver_instance();
        conn.reset(driver->connect("tcp://127.0.0.1:3306", "secureUser", "securePassword"));
        conn->setSchema("SecureSystemDB");
    }

    void logEvent(const std::string &eventDescription) {
        std::string encryptedLog = context.encryptData(eventDescription);

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO Logs (log_entry) VALUES (?)"));
        pstmt->setString(1, encryptedLog);
        pstmt->executeUpdate();
    }

    void readLogs() {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT log_entry FROM Logs"));

        while (res->next()) {
            std::string encryptedLog = res->getString("log_entry");
            std::cout << context.decryptData(encryptedLog) << std::endl;
        }
    }
};
