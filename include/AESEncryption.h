#ifndef AES_ENCRYPTION_H
#define AES_ENCRYPTION_H

#include "EncryptionContext.h"
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <string>
#include <vector>

class AESEncryption : public EncryptionStrategy {
private:
    std::vector<unsigned char> key;
    unsigned char iv[AES_BLOCK_SIZE]; // Initialization vector for AES

    void generateIV() {
        if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
            throw std::runtime_error("Failed to generate IV.");
        }
    }

public:
    AESEncryption(const std::vector<unsigned char> &key) : key(key) {
        generateIV();
    }

    std::string encrypt(const std::string &data) override;
    std::string decrypt(const std::string &data) override;
};

#endif // AES_ENCRYPTION_H
