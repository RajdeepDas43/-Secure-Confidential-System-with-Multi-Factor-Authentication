#ifndef KEY_MANAGEMENT_H
#define KEY_MANAGEMENT_H

#include <string>
#include <vector>

class KeyManagement {
private:
    std::string keyFilePath;
    std::vector<unsigned char> key;
    const int KEY_SIZE = 32;

    void generateKey();
    void reencryptData(const std::vector<unsigned char> &oldKey);

public:
    KeyManagement(const std::string &path);
    void createAndStoreKey();
    void loadKey();
    void rotateKey();
};

#endif // KEY_MANAGEMENT_H
