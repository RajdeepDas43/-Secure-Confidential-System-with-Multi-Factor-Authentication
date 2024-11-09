#include <openssl/rand.h>
#include <openssl/aes.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

class KeyManagement {
private:
    std::string keyFilePath;
    std::vector<unsigned char> key;
    const int KEY_SIZE = 32; // 256-bit key
    const int AES_BLOCK_SIZE = 16; // AES block size for CBC

    void generateKey() {
        key.resize(KEY_SIZE);
        if (!RAND_bytes(key.data(), KEY_SIZE)) {
            throw std::runtime_error("Failed to generate encryption key");
        }
    }

    std::string decryptData(const std::string &encryptedData, const std::vector<unsigned char> &decryptionKey) {
        AES_KEY decryptKey;
        AES_set_decrypt_key(decryptionKey.data(), KEY_SIZE * 8, &decryptKey);

        std::vector<unsigned char> iv(AES_BLOCK_SIZE, 0); // Initialize IV
        std::vector<unsigned char> decryptedData(encryptedData.size());

        AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(encryptedData.data()),
                        decryptedData.data(), encryptedData.size(), &decryptKey, iv.data(), AES_DECRYPT);

        return std::string(decryptedData.begin(), decryptedData.end());
    }

    std::string encryptData(const std::string &data, const std::vector<unsigned char> &encryptionKey) {
        AES_KEY encryptKey;
        AES_set_encrypt_key(encryptionKey.data(), KEY_SIZE * 8, &encryptKey);

        std::vector<unsigned char> iv(AES_BLOCK_SIZE, 0); // Initialize IV
        std::vector<unsigned char> encryptedData(data.size() + AES_BLOCK_SIZE);

        int encryptedSize = AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(data.data()),
                                            encryptedData.data(), data.size(), &encryptKey, iv.data(), AES_ENCRYPT);

        return std::string(encryptedData.begin(), encryptedData.begin() + encryptedSize);
    }

public:
    KeyManagement(const std::string &path) : keyFilePath(path) {
        if (!std::ifstream(keyFilePath)) {
            createAndStoreKey();
        } else {
            loadKey();
        }
    }

    void createAndStoreKey() {
        generateKey();
        std::ofstream keyFile(keyFilePath, std::ios::binary);
        keyFile.write(reinterpret_cast<char *>(key.data()), KEY_SIZE);
        keyFile.close();
    }

    void loadKey() {
        std::ifstream keyFile(keyFilePath, std::ios::binary);
        key.resize(KEY_SIZE);
        keyFile.read(reinterpret_cast<char *>(key.data()), KEY_SIZE);
    }

    void rotateKey() {
        std::vector<unsigned char> oldKey = key;  // Store the old key
        createAndStoreKey();                      // Generate and store a new key
        reencryptData(oldKey);                    // Re-encrypt data with the new key
    }

    void reencryptData(const std::vector<unsigned char> &oldKey) {
        std::cout << "Re-encrypting all data with the new key..." << std::endl;

        // List of files representing data entries to be re-encrypted.
        std::vector<std::string> dataFiles = {"data1.enc", "data2.enc"}; 

        for (const auto &filePath : dataFiles) {
            // Step 1: Read the encrypted data from the file
            std::ifstream inputFile(filePath, std::ios::binary);
            if (!inputFile) {
                std::cerr << "Error: Could not open file " << filePath << " for reading." << std::endl;
                continue;
            }
            std::string encryptedData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();

            // Step 2: Decrypt the data using the old key
            std::string decryptedData = decryptData(encryptedData, oldKey);

            // Step 3: Encrypt the data using the new key
            std::string reencryptedData = encryptData(decryptedData, key);

            // Step 4: Write the re-encrypted data back to the file
            std::ofstream outputFile(filePath, std::ios::binary);
            if (!outputFile) {
                std::cerr << "Error: Could not open file " << filePath << " for writing." << std::endl;
                continue;
            }
            outputFile.write(reencryptedData.c_str(), reencryptedData.size());
            outputFile.close();

            std::cout << "Re-encrypted data for " << filePath << " successfully." << std::endl;
        }
    }
};
