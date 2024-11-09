#ifndef ENCRYPTION_CONTEXT_H
#define ENCRYPTION_CONTEXT_H

#include <string>
#include <memory>

// Define a common interface for encryption strategies
class EncryptionStrategy {
public:
    virtual ~EncryptionStrategy() = default;
    virtual std::string encrypt(const std::string &data) = 0;
    virtual std::string decrypt(const std::string &data) = 0;
};

// The EncryptionContext class, which allows setting a strategy and using it for encryption and decryption
class EncryptionContext {
private:
    std::unique_ptr<EncryptionStrategy> strategy;

public:
    // Set the encryption strategy (dependency injection)
    void setStrategy(EncryptionStrategy *newStrategy) {
        strategy.reset(newStrategy);
    }

    // Encrypt data using the current strategy
    std::string encryptData(const std::string &data) {
        if (strategy) {
            return strategy->encrypt(data);
        }
        throw std::runtime_error("Encryption strategy not set.");
    }

    // Decrypt data using the current strategy
    std::string decryptData(const std::string &data) {
        if (strategy) {
            return strategy->decrypt(data);
        }
        throw std::runtime_error("Encryption strategy not set.");
    }
};

#endif // ENCRYPTION_CONTEXT_H
