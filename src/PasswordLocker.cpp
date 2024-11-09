#include "EncryptionContext.h"
#include "AESEncryption.h"
#include "KeyManagement.h"
#include "TaskQueue.h"
#include <iostream>
#include <fstream>

class PasswordProtectedLocker {
private:
    EncryptionContext context;
    KeyManagement keyManager;
    std::string lockerFilePath;
    TaskQueue &taskQueue;

    void authenticateUser() {
        std::string password;
        std::cout << "Enter locker password: ";
        std::cin >> password;
        if (password != "securepassword") {
            throw std::runtime_error("Authentication failed");
        }
    }

    void encryptAndStoreData(const std::string &data) {
        std::string encryptedData = context.encryptData(data);
        std::ofstream lockerFile(lockerFilePath, std::ios::binary);
        lockerFile.write(encryptedData.c_str(), encryptedData.size());
        lockerFile.close();
    }

public:
    PasswordProtectedLocker(TaskQueue &queue, const std::string &keyFilePath, const std::string &lockerPath)
        : taskQueue(queue), keyManager(keyFilePath), lockerFilePath(lockerPath) {
        keyManager.createAndStoreKey();
        context.setStrategy(new AESEncryption(keyManager));
    }

    void storeData(const std::string &data) {
        authenticateUser();
        taskQueue.pushTask([this, data]() { encryptAndStoreData(data); });
    }
};

int main() {
    TaskQueue taskQueue;
    std::vector<std::thread> workers;

    // Start worker threads
    for (int i = 0; i < 4; ++i) {
        workers.push_back(std::thread(worker, std::ref(taskQueue)));
    }

    PasswordProtectedLocker locker(taskQueue, "locker_key.bin", "locker.dat");

    locker.storeData("Sensitive information");
    locker.retrieveData();

    // Join threads (this example won't terminate due to the infinite loop in workers)
    for (auto &worker : workers) {
        worker.join();
    }

    return 0;
}