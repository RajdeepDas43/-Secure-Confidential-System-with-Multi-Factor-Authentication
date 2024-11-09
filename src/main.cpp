#include <iostream>
#include <string>
#include "KeyManagement.h"
#include "MFA.h"
#include "SecureFTPClient.h"
#include "SnippetManager.h"
#include "LogManager.h"
#include "DataEntryManager.h"
#include "TaskQueue.h"

int main() {
    try {
        // Paths and initialization
        std::string keyFilePath = "encryption_key.bin";
        KeyManagement keyManager(keyFilePath);
        MFA mfa;

        // MFA Login
        std::cout << "----- Secure Login with MFA -----" << std::endl;
        if (!mfa.verifyTOTP("123456")) { // Simulate user TOTP input; replace with actual input in production
            std::cerr << "MFA verification failed." << std::endl;
            return 1;
        }
        std::cout << "Login successful." << std::endl;

        // Initialize Password Protected Locker
        PasswordProtectedLocker locker(keyFilePath, "locker.dat");
        locker.storeData("Confidential data to protect in the locker");

        // Snippet Management
        std::cout << "\n----- Snippet Manager -----" << std::endl;
        SnippetManager snippetManager(keyFilePath);
        std::string snippetCode = "int main() { return 0; }";
        snippetManager.storeSnippet("sample_snippet", snippetCode);
        std::string retrievedSnippet = snippetManager.retrieveSnippet("sample_snippet");
        std::cout << "Retrieved Snippet: " << retrievedSnippet << std::endl;

        // Log Management
        std::cout << "\n----- Log Manager -----" << std::endl;
        LogManager logManager(keyFilePath);
        logManager.logEvent("User performed login.");
        logManager.logEvent("User stored a snippet.");
        std::cout << "All Logs: " << std::endl;
        logManager.readLogs();

        // Data Entry Management
        std::cout << "\n----- Data Entry Manager -----" << std::endl;
        DataEntryManager dataEntryManager(keyFilePath);
        std::string dataEntry = "Sensitive user data";
        dataEntryManager.storeDataEntry("user_data", dataEntry);
        std::string retrievedDataEntry = dataEntryManager.retrieveDataEntry("user_data");
        std::cout << "Retrieved Data Entry: " << retrievedDataEntry << std::endl;

        // Secure FTP Client
        std::cout << "\n----- Secure FTP Client -----" << std::endl;
        SecureFTPClient ftpClient(keyFilePath);
        std::string server = "ftp.server.com";
        std::string port = "21";
        std::string filePath = "file_to_upload.txt";
        ftpClient.uploadFile(server, port, filePath);

        std::string remoteFilePath = "remote_file.enc";
        std::string localDownloadPath = "downloaded_file.txt";
        ftpClient.downloadFile(server, port, remoteFilePath, localDownloadPath);

        // Key Rotation
        std::cout << "\n----- Key Management: Rotating Key -----" << std::endl;
        keyManager.rotateKey();
        std::cout << "Key rotation complete." << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
