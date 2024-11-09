#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "KeyManagement.h"
#include "AESEncryption.h"
#include "EncryptionContext.h"

using boost::asio::ip::tcp;

class SecureFTPClient {
private:
    EncryptionContext context;
    KeyManagement keyManager;
    boost::asio::io_context io_context;
    tcp::resolver resolver;
    tcp::socket socket;

    void encryptFile(const std::string &filePath, const std::string &outputPath) {
        std::ifstream inFile(filePath, std::ios::binary);
        std::string fileContent((std::istreambuf_iterator<char>(inFile)),
                                std::istreambuf_iterator<char>());
        std::string encryptedContent = context.encryptData(fileContent);

        std::ofstream outFile(outputPath, std::ios::binary);
        outFile.write(encryptedContent.c_str(), encryptedContent.size());
    }

    void decryptFile(const std::string &filePath, const std::string &outputPath) {
        std::ifstream inFile(filePath, std::ios::binary);
        std::string encryptedContent((std::istreambuf_iterator<char>(inFile)),
                                     std::istreambuf_iterator<char>());
        std::string decryptedContent = context.decryptData(encryptedContent);

        std::ofstream outFile(outputPath, std::ios::binary);
        outFile.write(decryptedContent.c_str(), decryptedContent.size());
    }

    void connectToServer(const std::string &server, const std::string &port) {
        tcp::resolver::results_type endpoints = resolver.resolve(server, port);
        boost::asio::connect(socket, endpoints);
    }

    void sendCommand(const std::string &command) {
        boost::asio::write(socket, boost::asio::buffer(command + "\r\n"));
    }

    std::string readResponse() {
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");
        std::istream response_stream(&response);
        std::string response_line;
        std::getline(response_stream, response_line);
        return response_line;
    }

    void uploadFileToServer(const std::string &localPath, const std::string &remotePath) {
        sendCommand("STOR " + remotePath);
        std::string response = readResponse();
        if (response.substr(0, 3) != "150") {
            throw std::runtime_error("Failed to initiate file upload: " + response);
        }

        std::ifstream file(localPath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open local file for uploading");
        }

        char buffer[1024];
        while (file.read(buffer, sizeof(buffer)).gcount() > 0) {
            boost::asio::write(socket, boost::asio::buffer(buffer, file.gcount()));
        }

        file.close();
        response = readResponse();
        if (response.substr(0, 3) != "226") {
            throw std::runtime_error("Failed to complete file upload: " + response);
        }
    }

    void downloadFileFromServer(const std::string &remotePath, const std::string &localPath) {
        sendCommand("RETR " + remotePath);
        std::string response = readResponse();
        if (response.substr(0, 3) != "150") {
            throw std::runtime_error("Failed to initiate file download: " + response);
        }

        std::ofstream file(localPath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open local file for downloading");
        }

        char buffer[1024];
        boost::system::error_code error;
        while (boost::asio::read(socket, boost::asio::buffer(buffer), error)) {
            if (error == boost::asio::error::eof) {
                break; // End of file reached
            } else if (error) {
                throw boost::system::system_error(error);
            }
            file.write(buffer, sizeof(buffer));
        }

        file.close();
        response = readResponse();
        if (response.substr(0, 3) != "226") {
            throw std::runtime_error("Failed to complete file download: " + response);
        }
    }

public:
    SecureFTPClient(const std::string &keyFilePath)
        : keyManager(keyFilePath), resolver(io_context), socket(io_context) {
        keyManager.createAndStoreKey();
        context.setStrategy(new AESEncryption(keyManager));
    }

    void uploadFile(const std::string &server, const std::string &port, const std::string &filePath) {
        std::string encryptedPath = filePath + ".enc";
        encryptFile(filePath, encryptedPath);

        connectToServer(server, port);
        sendCommand("USER your-ftp-username");
        sendCommand("PASS your-ftp-password");
        uploadFileToServer(encryptedPath, "remote/path/" + encryptedPath);
        socket.close();

        std::cout << "File uploaded and encrypted successfully." << std::endl;
    }

    void downloadFile(const std::string &server, const std::string &port, const std::string &remotePath, const std::string &localPath) {
        connectToServer(server, port);
        sendCommand("USER your-ftp-username");
        sendCommand("PASS your-ftp-password");
        downloadFileFromServer(remotePath, localPath + ".enc");
        socket.close();

        decryptFile(localPath + ".enc", localPath);
        std::cout << "File downloaded and decrypted successfully." << std::endl;
    }
};

int main() {
    try {
        SecureFTPClient ftpClient("ftp_key.bin");

        ftpClient.uploadFile("ftp.server.com", "21", "file_to_upload.txt");
        ftpClient.downloadFile("ftp.server.com", "21", "remote_file.enc", "downloaded_file.txt");

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
