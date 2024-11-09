#include <iostream>
#include <thread>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "KeyManagement.h"
#include "AESEncryption.h"
#include "EncryptionContext.h"
#include "TaskQueue.h"  // Assuming TaskQueue is implemented as shown previously

class FileCopyUtility {
private:
    EncryptionContext context;
    KeyManagement keyManager;
    TaskQueue &taskQueue;

    void encryptAndCopyFile(const boost::filesystem::path &srcPath, const boost::filesystem::path &destPath) {
        std::ifstream inFile(srcPath.string(), std::ios::binary);
        std::string fileContent((std::istreambuf_iterator<char>(inFile)),
                                std::istreambuf_iterator<char>());
        std::string encryptedContent = context.encryptData(fileContent);

        std::ofstream outFile(destPath.string(), std::ios::binary);
        outFile.write(encryptedContent.c_str(), encryptedContent.size());
    }

public:
    FileCopyUtility(TaskQueue &queue, const std::string &keyFilePath)
        : taskQueue(queue), keyManager(keyFilePath) {
        keyManager.createAndStoreKey();
        context.setStrategy(new AESEncryption(keyManager));
    }

    void bulkCopy(const std::string &srcDir, const std::string &destDir) {
        boost::filesystem::create_directory(destDir);

        for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(srcDir), {})) {
            boost::filesystem::path destPath = destDir / entry.path().filename();
            taskQueue.pushTask([this, srcPath = entry.path(), destPath]() {
                encryptAndCopyFile(srcPath, destPath);
            });
        }

        std::cout << "Bulk file copy tasks added to queue." << std::endl;
    }

    void backupFiles(const std::string &srcDir, const std::string &backupDir) {
        bulkCopy(srcDir, backupDir);
        std::cout << "Backup tasks added to queue." << std::endl;
    }
};

int main() {
    TaskQueue taskQueue;
    std::vector<std::thread> workers;

    // Start worker threads
    for (int i = 0; i < 4; ++i) {
        workers.push_back(std::thread(worker, std::ref(taskQueue)));
    }

    FileCopyUtility fileCopyUtility(taskQueue, "filecopy_key.bin");

    fileCopyUtility.bulkCopy("source_directory", "destination_directory");
    fileCopyUtility.backupFiles("source_directory", "backup_directory");

    // Join threads (this example won't terminate due to the infinite loop in workers)
    for (auto &worker : workers) {
        worker.join();
    }

    return 0;
}