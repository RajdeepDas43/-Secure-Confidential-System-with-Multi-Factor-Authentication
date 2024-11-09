#ifndef SECURE_FTP_CLIENT_H
#define SECURE_FTP_CLIENT_H

#include <string>
#include <boost/asio.hpp>
#include "KeyManagement.h"
#include "EncryptionContext.h"

class SecureFTPClient {
private:
    EncryptionContext context;
    KeyManagement keyManager;
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::socket socket;

    void encryptFile(const std::string &filePath, const std::string &outputPath);
    void decryptFile(const std::string &filePath, const std::string &outputPath);
    void connectToServer(const std::string &server, const std::string &port);
    void sendCommand(const std::string &command);
    std::string readResponse();
    void uploadFileToServer(const std::string &localPath, const std::string &remotePath);
    void downloadFileFromServer(const std::string &remotePath, const std::string &localPath);

public:
    SecureFTPClient(const std::string &keyFilePath);
    void uploadFile(const std::string &server, const std::string &port, const std::string &filePath);
    void downloadFile(const std::string &server, const std::string &port, const std::string &remotePath, const std::string &localPath);
};

#endif // SECURE_FTP_CLIENT_H
