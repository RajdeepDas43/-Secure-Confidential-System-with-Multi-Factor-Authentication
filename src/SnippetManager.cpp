#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "KeyManagement.h"
#include "AESEncryption.h"
#include "EncryptionContext.h"

class SnippetManager {
private:
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> conn;
    EncryptionContext context;
    KeyManagement keyManager;

public:
    SnippetManager(const std::string &keyFilePath) : keyManager(keyFilePath) {
        // Initialize encryption context
        keyManager.createAndStoreKey();
        context.setStrategy(new AESEncryption(keyManager));

        // Initialize database connection
        driver = sql::mysql::get_mysql_driver_instance();
        conn.reset(driver->connect("tcp://127.0.0.1:3306", "secureUser", "securePassword"));
        conn->setSchema("SecureSystemDB");
    }

    void storeSnippet(const std::string &name, const std::string &code) {
        std::string encryptedCode = context.encryptData(code);

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO Snippets (snippet_name, encrypted_code) VALUES (?, ?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, encryptedCode);
        pstmt->executeUpdate();
    }

    std::string retrieveSnippet(const std::string &name) {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT encrypted_code FROM Snippets WHERE snippet_name = ?"));
        pstmt->setString(1, name);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            std::string encryptedCode = res->getString("encrypted_code");
            return context.decryptData(encryptedCode);
        } else {
            return "Snippet not found.";
        }
    }
};
